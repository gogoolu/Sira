#pragma once

#include "Player/SiraPlayer.h"
#include "SiraPlayerController.h"
#include "SiraBlueprintLibrary.generated.h"


UCLASS(meta=(ScriptName="SiraBlueprintLibrary"))
class USiraBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ASiraPlayerController* GetSiraPlayerController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ASiraPlayer* GetSiraPlayer(const UObject* WorldContextObject);
	
	// this stupid function just use for handle reference parameter in blueprint function
	UFUNCTION(BlueprintPure, Category="Attribute")
	static FGameplayAttribute MakeGameplayAttribute(FGameplayAttribute Attribute);
	
	UFUNCTION(BlueprintPure, Category="Attribute")
	static float GetSiraPlayerAttribute(const ASiraPlayer* TargetPlayer, FGameplayAttribute Attribute);
};
