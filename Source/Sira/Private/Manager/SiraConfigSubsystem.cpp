#include "Manager/SiraConfigSubsystem.h"

#include "SiraLog.h"
#include "DataTable/SiraDataTable.h"

USiraConfigSubsystem::USiraConfigSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> InventoryItemFinder(TEXT("/Game/DataTable/DT_InventoryItem.DT_InventoryItem"));
	if (InventoryItemFinder.Succeeded())
	{
		InventoryItemConfig = InventoryItemFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> UniquePanelFinder(TEXT("/Game/DataTable/DT_UniquePanel.DT_UniquePanel"));
	if (UniquePanelFinder.Succeeded())
	{
		UniquePanel = UniquePanelFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyConfigFinder(TEXT("/Game/DataTable/DT_EnemyConfig.DT_EnemyConfig"));
	if (EnemyConfigFinder.Succeeded())
	{
		EnemyConfig = EnemyConfigFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DropConfigFinder(TEXT("/Game/DataTable/DT_DropConfig.DT_DropConfig"));
	if (DropConfigFinder.Succeeded())
	{
		DropConfig = DropConfigFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> LevelConfigFinder(TEXT("/Game/DataTable/DT_LevelConfig.DT_LevelConfig"));
	if (LevelConfigFinder.Succeeded())
	{
		LevelConfig = LevelConfigFinder.Object;
		MaxLevel = LevelConfig->GetRowMap().Num();
	}
}

const FInventoryItem& USiraConfigSubsystem::K2_GetInventoryItem(FName Key) const
{
	if (auto Found = GetInventoryItem(Key))
	{
		return *Found;
	}
	return FInventoryItem::None;
}

const FInventoryItem* USiraConfigSubsystem::GetInventoryItem(FName Key) const
{
	if (InventoryItemConfig)
	{
		return InventoryItemConfig->FindRow<FInventoryItem>(Key, "");
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraConfigSubsystem::GetInventoryItem] InventoryItem is nullptr, Key: %s"), *Key.ToString());
	return nullptr;
}

const FInventoryItem* USiraConfigSubsystem::GetInventoryItem(int32 Key) const
{
	return GetInventoryItem(FName(FString::FromInt(Key)));
}

const FDropConfig* USiraConfigSubsystem::GetDropConfig(FName Key) const
{
	if (DropConfig)
	{
		return DropConfig->FindRow<FDropConfig>(Key, "");
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraConfigSubsystem::GetDropConfig] DropConfig is nullptr, Key: %s"), *Key.ToString());
	return nullptr;
}

const FDropConfig* USiraConfigSubsystem::GetDropConfig(int32 Key) const
{
	return GetDropConfig(FName(FString::FromInt(Key)));
}

const FLevelConfig* USiraConfigSubsystem::GetLevelConfig(int32 Key) const
{
	if (LevelConfig)
	{
		return LevelConfig->FindRow<FLevelConfig>(FName(FString::FromInt(Key)), "");
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraConfigSubsystem::GetLevelConfig] DropConfig is nullptr, Key: %d"), Key);
	return nullptr;
}

int32 USiraConfigSubsystem::GetMaxLevel() const
{
	return MaxLevel;
}

const FUniquePanelRow* USiraConfigSubsystem::GetUniquePanel(FName Key) const
{
	if (UniquePanel)
	{
		return UniquePanel->FindRow<FUniquePanelRow>(Key, "");
	}
	
	UE_LOG(LogSira, Warning, TEXT("[USiraConfigSubsystem::GetUniquePanel] UniquePanel is nullptr, Key: %s"), *Key.ToString());
	return nullptr;
}

const FEnemyConfig* USiraConfigSubsystem::GetEnemyConfig(FName Key) const
{
	if (EnemyConfig)
	{
		return EnemyConfig->FindRow<FEnemyConfig>(Key, "");
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraConfigSubsystem::GetEnemyConfig] EnemyData is nullptr, Key: %s"), *Key.ToString());
	return nullptr;
}

const FEnemyConfig* USiraConfigSubsystem::GetEnemyConfig(int32 Key) const
{
	return GetEnemyConfig(FName(FString::FromInt(Key)));
}
