#pragma once
#include "CoreMinimal.h"
#include "SiraPlayerController.generated.h"


class UInputAction;

UCLASS()
class ASiraPlayerController : public APlayerController 
{
	GENERATED_BODY()
public:
	ASiraPlayerController();
private:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	// TObjectPtr<class UInputMappingContext> DefaultMappingContext;
 //    
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	// TObjectPtr<UInputAction> JumpAction;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	// TObjectPtr<UInputAction> MoveAction;
 //    
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	// TObjectPtr<UInputAction> LookAction;
 //    
	// virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
};
