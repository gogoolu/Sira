#pragma once

#include "AIController.h"

#include "SiraAIController.generated.h"

UCLASS()
class ASiraAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn);
};
