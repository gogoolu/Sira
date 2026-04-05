// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/BaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Manager/SiraGameplayTags.h"


void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		if (auto ASC = GetOwningAbilitySystemComponent())
		{
			if (FMath::IsNearlyZero(GetHP()) && !ASC->HasMatchingGameplayTag(FSiraGameplayTags::GetTag_LifeState_Dead()))
			{
				ASC->AddLooseGameplayTag(FSiraGameplayTags::GetTag_LifeState_Dead());
			}
		}
	}
	
}
