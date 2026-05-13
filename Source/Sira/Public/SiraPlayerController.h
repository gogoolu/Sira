#pragma once
#include "CoreMinimal.h"
#include "SiraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class ASiraPlayerController : public APlayerController 
{
	GENERATED_BODY()
	
public:
	ASiraPlayerController();
	
	
private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	
	void TogglePauseGame();
	
	// input ==================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> BasicGameMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> GameplayMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> PauseAction;
	
};
