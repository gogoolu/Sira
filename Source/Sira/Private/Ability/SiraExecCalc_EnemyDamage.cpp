#include "Ability/SiraExecCalc_EnemyDamage.h"

#include "SiraLog.h"
#include "AttributeSet/BaseAttributeSet.h"


struct FEnemyDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePercent);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	static const FEnemyDamageStatics& GetDamageStatics()
	{
		static FEnemyDamageStatics Statics;
		return Statics;
	}
	
	FEnemyDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnemyAttr, BaseDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnemyAttr, DamagePercent, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttr, Armor, Target, false);
	}
};



USiraExecCalc_EnemyDamage::USiraExecCalc_EnemyDamage()
{
	RelevantAttributesToCapture.Add(FEnemyDamageStatics::GetDamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(FEnemyDamageStatics::GetDamageStatics().DamagePercentDef);
	RelevantAttributesToCapture.Add(FEnemyDamageStatics::GetDamageStatics().ArmorDef);
}

void USiraExecCalc_EnemyDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;

	float BaseDamage = 0.f;
	float DamagePercent = 0.f;
	float TargetArmor = 0.f;

	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FEnemyDamageStatics::GetDamageStatics().BaseDamageDef,
		EvalParams,
		BaseDamage
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_EnemyDamage::Execute_Implementation] fail to get BaseDamage"))
	}

	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FEnemyDamageStatics::GetDamageStatics().DamagePercentDef,
		EvalParams,
		DamagePercent
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_EnemyDamage::Execute_Implementation] fail to get DamagePercent"))
	}
	
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FEnemyDamageStatics::GetDamageStatics().ArmorDef,
		EvalParams,
		TargetArmor
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_EnemyDamage::Execute_Implementation] fail to get DamagePercent"))
	}
	
	auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_EnemyDamage::Execute_Implementation] Source Owner: %s, Target Owner: %s"), *SourceASC->GetOwner()->GetName(), *TargetASC->GetOwner()->GetName());
	
	float Damage = BaseDamage * (1.f + DamagePercent);
	if (TargetArmor > 0.f)
	{
		Damage *= 1.f / (1.f + TargetArmor / 15.f);
	}
	
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UPlayerAttr::GetHPAttribute(),
			EGameplayModOp::Additive,
			-Damage
		)
	);
}
