#include "Ability/SiraExecCalc_Damage.h"

#include "SiraLog.h"
#include "AttributeSet/BaseAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FlatDamageBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePercent);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	
	static const FDamageStatics& GetDamageStatics()
	{
		static FDamageStatics Statics;
		return Statics;
	}
	
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttr, BaseDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerModAttr, FlatDamageBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerModAttr, DamagePercent, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerModAttr, CriticalRate, Source, false);
	}
};



USiraExecCalc_Damage::USiraExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(FDamageStatics::GetDamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(FDamageStatics::GetDamageStatics().FlatDamageBonusDef);
	RelevantAttributesToCapture.Add(FDamageStatics::GetDamageStatics().DamagePercentDef);
	RelevantAttributesToCapture.Add(FDamageStatics::GetDamageStatics().CriticalRateDef);
}

void USiraExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;

	float BaseDamage = 0.f;
	float FlatBonus = 0.f;
	float DamagePercent = 0.f;
	float CriticalRate = 0.f;

	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FDamageStatics::GetDamageStatics().BaseDamageDef,
		EvalParams,
		BaseDamage
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_Damage::Execute_Implementation] fail to get BaseDamage"))
	}

	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FDamageStatics::GetDamageStatics().FlatDamageBonusDef,
		EvalParams,
		FlatBonus
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_Damage::Execute_Implementation] fail to get FlatDamageBonus"))
	}
	
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FDamageStatics::GetDamageStatics().DamagePercentDef,
		EvalParams,
		DamagePercent
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_Damage::Execute_Implementation] fail to get DamagePercent"))
	}
	
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FDamageStatics::GetDamageStatics().CriticalRateDef,
		EvalParams,
		CriticalRate
	))
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraExecCalc_Damage::Execute_Implementation] fail to get CriticalRate"))
	}
	
	// 暴击增伤倍率
	float CriticalModification = 2;
	
	float Damage = (BaseDamage + FlatBonus) * (1.f + DamagePercent);
	Damage = FMath::FRand() <= CriticalRate ? Damage * CriticalModification : Damage;
	
	// 伤害最低为 1
	Damage = FMath::Max(Damage, 1.f);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UEnemyAttr::GetHPAttribute(),
			EGameplayModOp::Additive,
			-Damage
		)
	);
}
