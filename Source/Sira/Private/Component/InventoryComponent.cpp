#include "Component/InventoryComponent.h"

#include "GameplayEffectTypes.h"
#include "SiraLog.h"
#include "DataTable/SiraDataTable.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Manager/SiraBlueprintLibrary.h"

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
	Items.Add(ItemID, Count);
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

TSharedPtr<FStreamableHandle> UInventoryComponent::GetItemIconAsync(FName ItemID, TUniqueFunction<void(UTexture2D*)> OnLoaded)
{
	// 已缓存
	if (auto Cache = CachedIcons.Find(ItemID))
	{
		OnLoaded(*Cache);
		return nullptr;
	}
	 
	// 配置不存在
	auto Item = InventoryItemConfig->FindRow<FInventoryItem>(ItemID, "");
	if (!Item)
	{
		OnLoaded(nullptr);
		UE_LOG(LogSira, Warning, TEXT("[UInventoryComponent::GetItemIconAsync] Do not Find Inventory Item, ItemID: %s"), *ItemID.ToString());
		return nullptr;
	}

	// 资源未加载，加入回调队列中
	PendingCallbacks.FindOrAdd(ItemID).Add(MoveTemp(OnLoaded));
	
	// 如果该资源正在加载，直接退出等待加载完成后的回调
	if (LoadingHandles.Contains(ItemID))
	{
		return nullptr;
	}
	
	// 首次加载
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	auto Handle = Streamable.RequestAsyncLoad(
		Item->Icon.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([
			WeakThis = TWeakObjectPtr<UInventoryComponent>(this),
			&SoftPtr = Item->Icon, 
			&Cache = CachedIcons,
			&Callbacks = PendingCallbacks,
			&InnerLoadingHandles = LoadingHandles,
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
				if (auto ItemCallbacks = Callbacks.Find(ItemID))
				{
					for (auto& Callback : *ItemCallbacks)
					{
						Callback(LoadedIcon);
					}
				}
				
				Callbacks.Remove(ItemID);
			}
			// 删除loadingHandles
			InnerLoadingHandles.Remove(ItemID);
		})
	);
	LoadingHandles.Add(ItemID, Handle);
	return Handle;
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
	if (auto Item = InventoryItemConfig->FindRow<FInventoryItem>(ItemID, ""))
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
