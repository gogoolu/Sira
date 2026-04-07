#include "Manager/SiraBlueprintLibrary.h"

#include "SiraLog.h"
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
