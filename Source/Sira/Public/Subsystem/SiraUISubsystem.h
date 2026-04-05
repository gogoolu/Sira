#pragma once

#include "CoreMinimal.h"
#include "SiraUISubsystem.generated.h"

UCLASS()
class USiraUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
