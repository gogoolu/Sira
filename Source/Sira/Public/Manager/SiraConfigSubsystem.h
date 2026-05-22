#pragma once
#include "DataTable/SiraDataTable.h"


#include "SiraConfigSubsystem.generated.h"

struct FLevelConfig;
struct FDropConfig;
struct FEnemyData;
struct FInventoryItem;
struct FUniquePanelRow;

UCLASS()
class USiraConfigSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USiraConfigSubsystem();
	
	// 全局唯一UI配置表
	const FUniquePanelRow* GetUniquePanel(FName Key) const;
	
	// 物品信息配置表
	UFUNCTION(BlueprintCallable)
	const FInventoryItem& K2_GetInventoryItem(FName Key) const;
	const FInventoryItem* GetInventoryItem(FName Key) const;
	const FInventoryItem* GetInventoryItem(int32 Key) const;
	
	
	// 敌人配置表
	const FEnemyConfig* GetEnemyConfig(FName Key) const;
	const FEnemyConfig* GetEnemyConfig(int32 Key) const;
	
	// 敌人掉落物配置表
	const FDropConfig* GetDropConfig(FName Key) const;
	const FDropConfig* GetDropConfig(int32 Key) const;
	
	// 关卡配置表
	const FLevelConfig* GetLevelConfig(int32 Key) const;
	int32 GetMaxLevel() const;
	
private:
	
	int32 MaxLevel = 0;
	
	// =========== DataTable ============
	
	// 全局唯一UI配置表
	UPROPERTY()
	TObjectPtr<UDataTable> UniquePanel;
	
	// 物品信息配置表
	UPROPERTY()
	TObjectPtr<UDataTable> InventoryItemConfig;
	
	// 敌人配置表
	UPROPERTY()
	TObjectPtr<UDataTable> EnemyConfig;
	
	// 敌人掉落物配置表
	UPROPERTY()
	TObjectPtr<UDataTable> DropConfig;
	
	// 关卡配置表
	UPROPERTY()
	TObjectPtr<UDataTable> LevelConfig;
	
};
