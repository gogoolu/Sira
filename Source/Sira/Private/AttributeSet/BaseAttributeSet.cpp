// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/BaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SiraGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/NormalEnemy.h"
#include "Manager/SiraGameplayTags.h"


void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		if (auto ASC = GetOwningAbilitySystemComponent())
		{
			if (FMath::IsNearlyZero(GetHP()) && !ASC->HasMatchingGameplayTag(SiraGameplayTags::TAG_LifeState_Dead))
			{
				ASC->AddLooseGameplayTag(SiraGameplayTags::TAG_LifeState_Dead);
			}
			
			
			if (ASC->HasMatchingGameplayTag(SiraGameplayTags::TAG_Character_Enemy) && Data.EvaluatedData.Magnitude < 0)
			{
				// 敌人受伤
				if (auto GameMode = Cast<ASiraGameMode>(GetWorld()->GetAuthGameMode()))
				{
					float Damage = -Data.EvaluatedData.Magnitude;
					GameMode->OnEnemyTakeDamage.Broadcast(Damage);
					
					if (ANormalEnemy* Enemy = Cast<ANormalEnemy>(ASC->GetOwnerActor()))
					{
						FVector DamageNumberLocation = Enemy->GetActorLocation() + FVector{0, 0, Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
						GameMode->ShowDamageNumber(Damage, DamageNumberLocation);
					}
					
				}
			}
		}
	}
}
