#pragma once

#include "SiraSaveGameSubsystem.generated.h"


UCLASS()
class USiraSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SaveGameData();
	
	UFUNCTION(BlueprintCallable)
	void LoadGameData();
};
