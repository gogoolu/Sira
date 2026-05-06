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
