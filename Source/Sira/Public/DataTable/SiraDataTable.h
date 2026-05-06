#pragma once

#include "SiraDataTable.generated.h"

class USiraUniquePanel;

UENUM(BlueprintType)
enum class ESiraLevelType : uint8
{
	Normal,
	Boss,
	Store,
};

USTRUCT(BlueprintType)
struct FLevelInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESiraLevelType LevelType = ESiraLevelType::Normal;

	// level style, decide terrain gen
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelStyle = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxEnemyNum = 10;
};

USTRUCT(BlueprintType)
struct FUniquePanelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UIName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<USiraUniquePanel> UniquePanelSoftClass;

private:
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
};
