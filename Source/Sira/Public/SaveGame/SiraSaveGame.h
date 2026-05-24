#pragma once

#include "GameFramework/SaveGame.h"
#include "SiraSaveGame.generated.h"

USTRUCT()
struct FPlayerSaveAttr
{
	GENERATED_BODY()
	
	float HP;
	float MaxHP;
	float BaseDamage;
	float AttackSpeedPercent;
	float Armor;
	float Looting;
	float HPRegen;
};


UCLASS()
class USiraSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	int32 Seed;
	
	UPROPERTY()
	int32 BaseSeed;
	
	UPROPERTY()
	int32 CurrentLevel;
	
	UPROPERTY()
	FPlayerSaveAttr PlayerAttr;
	
	UPROPERTY()
	TArray<int32> InventoryItems;
};
