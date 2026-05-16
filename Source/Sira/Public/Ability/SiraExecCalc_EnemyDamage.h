#pragma once

#include "GameplayEffectExecutionCalculation.h"


#include "SiraExecCalc_EnemyDamage.generated.h"

UCLASS()
class USiraExecCalc_EnemyDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USiraExecCalc_EnemyDamage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};