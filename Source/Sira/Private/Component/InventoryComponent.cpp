#include "Component/InventoryComponent.h"

#include "GameplayEffectTypes.h"
#include "SiraLog.h"
#include "DataTable/SiraDataTable.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "Manager/SiraConfigSubsystem.h"

UInventoryComponent::UInventoryComponent()
{
}

int64 UInventoryComponent::GetCoins() const
{
	return Coins;
}

int64 UInventoryComponent::AddCoins(int32 Num)
{
	Coins += Num;
	OnCoinsChanged.Broadcast(Coins);
	return Coins;
}

int32 UInventoryComponent::GetItemCount(FName ItemID) const
{
	const int32* Count = Items.Find(ItemID);
	return Count? *Count : 0;
}

void UInventoryComponent::AddItem(FName ItemID, int32 Count)
{
	ApplyItemBuff(ItemID, Count);
	Items.Add(ItemID, GetItemCount(ItemID) + Count);
	OnInventoryItemsChanged.Broadcast();
}

void UInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
	RemoveItemBuff(ItemID, Count);
	if (int32* CurrentCount = Items.Find(ItemID))
	{
		Items.Add(ItemID, FMath::Max(*CurrentCount - Count, 0));
	}
	OnInventoryItemsChanged.Broadcast();
}

void UInventoryComponent::GetItemIconAsync(FName ItemID, TFunction<void(UTexture2D*)> OnLoaded)
{
	// 已缓存
	if (auto Cache = CachedIcons.Find(ItemID))
	{
		OnLoaded(*Cache);
		return;
	}
	
	auto Config = GetWorld()->GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	// 配置不存在
	auto Item = Config->GetInventoryItem(ItemID);
	if (!Item)
	{
		OnLoaded(nullptr);
		UE_LOG(LogSira, Warning, TEXT("[UInventoryComponent::GetItemIconAsync] Do not Find Inventory Item, ItemID: %s"), *ItemID.ToString());
		return;
	}

	// 首次加载
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	Streamable.RequestAsyncLoad(
		Item->Icon.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([
			WeakThis = MakeWeakObjectPtr<UInventoryComponent>(this),
			&SoftPtr = Item->Icon,
			&Cache = CachedIcons,
			Callback = MoveTemp(OnLoaded),
			ItemID]()
		{
			if (!WeakThis.IsValid())
			{
				UE_LOG(LogSira, Log, 
					TEXT("[UInventoryComponent::GetItemIconAsync] InventoryComponent has been GC, Stop Call Callback, ItemID: %s"),
					*ItemID.ToString());
				
				return;
			}
			if (UTexture2D* LoadedIcon = SoftPtr.Get())
			{
				// 缓存，并触发所有的回调
				Cache.Add(ItemID, LoadedIcon);
				Callback(LoadedIcon);
			}
		})
	);
}

TArray<FName> UInventoryComponent::GetAllItemID() const
{
	TArray<FName> Result;
	for (auto [ItemID, Count] : Items)
	{
		if (Count > 0)
		{
			Result.Add(ItemID);
		}
	}
	return Result;
}

void UInventoryComponent::K2_GetItemIconAsync(FName ItemID, FOnInventoryIconLoaded OnLoaded)
{
	GetItemIconAsync(ItemID, [OnLoaded](UTexture2D* LoadedIcon)
	{
		OnLoaded.ExecuteIfBound(LoadedIcon);
	});
}

void UInventoryComponent::ApplyItemBuff(FName ItemID, int32 Count)
{
	auto Config = GetWorld()->GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	if (auto Item = Config->GetInventoryItem(ItemID))
	{
		FGameplayEffectContextHandle Context;
		Context.AddSourceObject(this);
		Context.AddInstigator(GetOwner(), GetOwner());
		
		if (auto Player = Cast<ASiraPlayer>(GetOwner()))
		{
			if (auto ASC = Player->GetAbilitySystemComponent())
			{
				auto SpecHandle = ASC->MakeOutgoingSpec(Item->BuffEffect, 1.f, Context);
				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					auto& Handles = ActivatedEffects.FindOrAdd(ItemID);
					Handles.Add(Handle);
				}
			}
		}
	}
}

void UInventoryComponent::RemoveItemBuff(FName ItemID, int32 Count)
{
	if (Count <= 0)
	{
		return;
	}
	if (auto Player = Cast<ASiraPlayer>(GetOwner()))
	{
		if (auto ASC = Player->GetAbilitySystemComponent())
		{
			if (auto Handles = ActivatedEffects.Find(ItemID))
			{
				Count = FMath::Min(Count, Handles->Num());
				for (int32 Index = 0; Index < Count; ++Index)
				{
					if (!ASC->RemoveActiveGameplayEffect(Handles->Last(Index)))
					{
						UE_LOG(LogSira, Warning, TEXT("[UInventoryComponent::RemoveItemBuff] Remove Buff Failed, Handle is Invalid, ItemID: %s"), *ItemID.ToString());
					}
				}
				Handles->RemoveAtSwap(Handles->Num() - Count, Count);
			}
		}
	}
}
