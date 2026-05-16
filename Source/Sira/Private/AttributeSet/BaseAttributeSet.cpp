// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/BaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SiraGameMode.h"
#include "SiraLog.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/NormalEnemy.h"
#include "Manager/SiraGameplayTags.h"
#include "Manager/SiraGlobalEventSubsystem.h"


void UEnemyAttr::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		
		USiraGlobalEventSubsystem* GlobalEventSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USiraGlobalEventSubsystem>();
		if (!GlobalEventSubsystem)
		{
			UE_LOG(LogSira, Error, TEXT("[UEnemyAttr::PostGameplayEffectExecute] GlobalEventSubsystem is nullptr!!"));
			return;
		}
		
		if (auto ASC = GetOwningAbilitySystemComponent())
		{
			if (ASC->HasMatchingGameplayTag(SiraGameplayTags::TAG_Character_Enemy) && Data.EvaluatedData.Magnitude < 0)
			{
				// 敌人受伤
				float Damage = -Data.EvaluatedData.Magnitude;
				GlobalEventSubsystem->OnEnemyTakeDamage.Broadcast(Damage);
				
				if (ANormalEnemy* Enemy = Cast<ANormalEnemy>(ASC->GetOwnerActor()))
				{
					if (auto GameMode = Cast<ASiraGameMode>(GetWorld()->GetAuthGameMode()))
					{
						FVector DamageNumberLocation = Enemy->GetActorLocation() + FVector{0, 0, Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
						GameMode->ShowDamageNumber(Damage, DamageNumberLocation);
					}
				}
			}
			
			if (FMath::IsNearlyZero(GetHP()) && !ASC->HasMatchingGameplayTag(SiraGameplayTags::TAG_LifeState_Dead))
			{
				// 敌人死亡
				GlobalEventSubsystem->OnEnemyDead.Broadcast();
				ASC->AddLooseGameplayTag(SiraGameplayTags::TAG_LifeState_Dead);
			}
		}
	} else if (Data.EvaluatedData.Attribute == GetMaxHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		
		// 血量上限最小为 1
		SetMaxHP(FMath::Max(GetMaxHP(), 1.f));
	}
}

void UPlayerAttr::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		if (auto ASC = GetOwningAbilitySystemComponent())
		{
			if (FMath::IsNearlyZero(GetHP()) && !ASC->HasMatchingGameplayTag(SiraGameplayTags::TAG_LifeState_Dead))
			{
				ASC->AddLooseGameplayTag(SiraGameplayTags::TAG_LifeState_Dead);
			}
		}
	} else if (Data.EvaluatedData.Attribute == GetMaxHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
		// 血量上限最小为 1
		SetMaxHP(FMath::Max(GetMaxHP(), 1.f));
	}
	
}

void UPlayerModAttr::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}
