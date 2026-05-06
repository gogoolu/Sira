#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugUserWidget.generated.h"


UCLASS()
class UDebugUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void HandleCommand(const FString& Command);
	UPROPERTY(BlueprintReadWrite)
	FString LastCommand;
};
