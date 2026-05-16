#include "Ability/SiraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Manager/SiraGameplayTags.h"

void USiraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	auto SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, GetAbilityLevel());

	// 设置动态CD时间
	SpecHandle.Data->SetSetByCallerMagnitude(
		SiraGameplayTags::TAG_Data_CoolDown,
		GetCoolDownDuration()
	);
	
	SpecHandle.Data->DynamicGrantedTags.AddTag(CoolDownTag);
	
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

const FGameplayTagContainer* USiraGameplayAbility::GetCooldownTags() const
{
	TempCooldownTags.Reset();
	auto ParentTags = Super::GetCooldownTags();
	
	TempCooldownTags.AppendTags(*ParentTags);
	
	// 移除占位符 Tag
	TempCooldownTags.RemoveTag(SiraGameplayTags::TAG_CoolDown_Placeholder);
	if (CoolDownTag.IsValid())
	{
		TempCooldownTags.AddTag(CoolDownTag);
	}

	return &TempCooldownTags;
}

float USiraGameplayAbility::GetCoolDownDuration() const
{
	if (auto ASC = GetAbilitySystemComponentFromActorInfo())
	{
		float AttackSpeedPercent = ASC->GetNumericAttribute(UPlayerAttr::GetAttackSpeedPercentAttribute());
		return BaseCoolDown / (1.f + AttackSpeedPercent);
	}
	return BaseCoolDown;
}
