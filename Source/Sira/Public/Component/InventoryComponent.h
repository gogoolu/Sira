#pragma once


#include "InventoryComponent.generated.h"

struct FStreamableHandle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoinsNumChanged, int64, Coins);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryItemsChanged);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryIconLoaded, UTexture2D*, Icon);

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInventoryComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int64 GetCoins() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int64 AddCoins(int32 Num);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCount(FName ItemID) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemID, int32 Count = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemID, int32 Count = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "GetItemIconAsync")
	void K2_GetItemIconAsync(FName ItemID, FOnInventoryIconLoaded Loaded);
	
	void GetItemIconAsync(FName ItemID, TFunction<void(UTexture2D*)> OnLoaded);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FName> GetAllItemID() const;
	
	UPROPERTY(BlueprintAssignable)
	FCoinsNumChanged OnCoinsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryItemsChanged OnInventoryItemsChanged;
	
private:
	
	void ApplyItemBuff(FName ItemID, int32 Count);
	void RemoveItemBuff(FName ItemID, int32 Count);
	
	// 背包中的物品, ItemID -> Count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<FName, int32> Items;
	
	// ItemID -> GameEffects
	TMap<FName, TArray<FActiveGameplayEffectHandle>> ActivatedEffects;
	
	// 钱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int64 Coins = 0;

	
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UTexture2D>> CachedIcons;
};
