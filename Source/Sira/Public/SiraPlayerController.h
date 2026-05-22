#pragma once
#include "CoreMinimal.h"
#include "SiraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class USiraUniquePanel;

UCLASS()
class ASiraPlayerController : public APlayerController 
{
	GENERATED_BODY()
	
public:
	ASiraPlayerController();
	
	
	UFUNCTION(BlueprintCallable)
	USiraUniquePanel* GetOrCreateWidget(FName Key);
	
private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseGame();
	
	
	
	// input ==================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> BasicGameMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> GameplayMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY()
	TMap<FName, TObjectPtr<USiraUniquePanel>> WidgetMap;

	UPROPERTY()
	TMap<FName, TSubclassOf<USiraUniquePanel>> WidgetClassMap;
	
};
