#pragma once
#include "GameplayEffectExecutionCalculation.h"


#include "SiraExecCalc_Damage.generated.h"

UCLASS()
class USiraExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USiraExecCalc_Damage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};

