#pragma once


#include "SiraGlobalEventSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyTakeDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDead);

UCLASS()
class USiraGlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyTakeDamage OnEnemyTakeDamage;
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDead OnEnemyDead;
	
	// UPROPERTY(BlueprintAssignable)
	
};
