#include "Manager/SiraBlueprintLibrary.h"

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
