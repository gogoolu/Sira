#pragma once
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "SiraDataTable.generated.h"

class ANormalEnemy;
class UGameplayEffect;
class USiraUniquePanel;


//  ==================== 全局唯一UI配置表 ================
USTRUCT(BlueprintType)
struct FUniquePanelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UIName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<USiraUniquePanel> UniquePanelSoftClass;
#if WITH_EDITOR
private:
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};


// ===================== 物品信息配置表 ===================
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()
	
	static FInventoryItem None;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	int32 ItemID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> BuffEffect = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;
};

// ==================== 敌人配置表 ====================
USTRUCT(BlueprintType)
struct FEnemyConfig : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropCoins = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropRate = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropConfigID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ANormalEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;

#if WITH_EDITOR
private:
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};

// ===================== 敌人掉落物表 ========================
USTRUCT(BlueprintType)
struct FDropConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = 0;
	
	// InventoryItem ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> DropItems;
	
};


// ===================== 关卡配置表 ==========================
USTRUCT(BlueprintType)
struct FEnemyWeightInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight = 0;
};

USTRUCT(BlueprintType)
struct FLevelConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelID = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxEnemyNum = 10;

	// 敌人种类以及生成权重
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyWeightInfo> Enemies;
};