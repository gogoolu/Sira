#include "Manager/SiraBlueprintLibrary.h"

#include "SiraLog.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"

ASiraPlayerController* USiraBlueprintLibrary::GetSiraPlayerController(const UObject* WorldContextObject)
{
	return Cast<ASiraPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

ASiraPlayer* USiraBlueprintLibrary::GetSiraPlayer(const UObject* WorldContextObject)
{
	return Cast<ASiraPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}

FGameplayAttribute USiraBlueprintLibrary::MakeGameplayAttribute(FGameplayAttribute Attribute)
{
	return Attribute;
}

float USiraBlueprintLibrary::GetSiraPlayerAttribute(const ASiraPlayer* TargetPlayer, FGameplayAttribute Attribute)
{
	if (IsValid(TargetPlayer))
	{
		if (auto ASC = TargetPlayer->GetAbilitySystemComponent())
		{
			return ASC->GetNumericAttribute(Attribute);
		}
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraBlueprintLibrary::GetSiraPlayerAttribute] TargetPlayer or ASC is Invalid, TargetPlayer: [%s]"), *TargetPlayer->GetName());
	return 0.0f;
}

FString USiraBlueprintLibrary::GetAttributeName(const FGameplayAttribute& Attribute)
{
	return Attribute.AttributeName;
}

int32 USiraBlueprintLibrary::GetActorsCount(const UObject* WorldContextObject, TSubclassOf<AActor> Class)
{
	int32 Result = 0;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (TActorIterator<AActor> It(World, Class); It; ++It)
		{
			Result++;
		}
	}
	return Result;
}

void USiraBlueprintLibrary::SimplyShowMouse(const UObject* WorldContextObject, bool bShow)
{
	if (auto PC = GetSiraPlayerController(WorldContextObject))
	{
		PC->SetShowMouseCursor(bShow);
		if (bShow)
		{
			PC->SetInputMode(FInputModeGameAndUI {});
		} else
		{
			PC->SetInputMode(FInputModeGameOnly {});
		}
	}
}

APlayerCameraManager* USiraBlueprintLibrary::GetCameraManger(const UObject* WorldContextObject)
{
	if (auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		return PC->PlayerCameraManager;
	}
	return nullptr;
}


TArray<FAttributeDisplayInfo> USiraBlueprintLibrary::GetAllPlayerAttributeDisplayInfos()
{
	TArray<FAttributeDisplayInfo> Attributes;
	Attributes.Add({UPlayerAttr::GetHPAttribute(), FText::FromString(TEXT("HP"))});
	Attributes.Add({UPlayerAttr::GetMaxHPAttribute(), FText::FromString(TEXT("MaxHP"))});
	Attributes.Add({UPlayerAttr::GetBaseDamageAttribute(), FText::FromString(TEXT("基础伤害"))});
	Attributes.Add({UPlayerAttr::GetAttackSpeedPercentAttribute(), FText::FromString(TEXT("%攻击速度"))});
	Attributes.Add({UPlayerAttr::GetArmorAttribute(), FText::FromString(TEXT("护甲"))});
	Attributes.Add({UPlayerAttr::GetHPRegenAttribute(), FText::FromString(TEXT("生命回复"))});
	
	Attributes.Add({UPlayerModAttr::GetFlatDamageBonusAttribute(), FText::FromString(TEXT("+基础伤害加成"))});
	Attributes.Add({UPlayerModAttr::GetDamagePercentAttribute(), FText::FromString(TEXT("%伤害百分比加成"))});
	Attributes.Add({UPlayerModAttr::GetCriticalRateAttribute(), FText::FromString(TEXT("%暴击率"))});
	
	return Attributes;
}

bool USiraBlueprintLibrary::IsPercentAttribute(const FGameplayAttribute& Attribute)
{
	if (Attribute == UPlayerAttr::GetAttackSpeedPercentAttribute())
	{
		return true;
	}
	if (Attribute == UPlayerModAttr::GetCriticalRateAttribute())
	{
		return true;
	}
	if (Attribute == UPlayerModAttr::GetDamagePercentAttribute())
	{
		return true;
	}
	
	return false;
}
