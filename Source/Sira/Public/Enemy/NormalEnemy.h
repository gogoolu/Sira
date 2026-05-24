// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiraCharacter.h"

#include "NormalEnemy.generated.h"

class AProjectile;
class UAttributeBillboardComponent;
class USiraAttributeComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPendingDeath);
DECLARE_MULTICAST_DELEGATE(FOnEnemyAttackFinished)

UCLASS()
class SIRA_API ANormalEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANormalEnemy();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	int32 GetEnemyID() const;
	
	void MeleeAttack();
	
	void RangedAttack();
	void SpawnProjectile();
	
	UFUNCTION(BlueprintCallable)
	bool IsAttacking() const;
	
	virtual void OnDead(FGameplayTag Tag, int32 Count);
	
	virtual void Die();

	// 在 BeginPlay 以及从对象池中取出来时调用
	virtual void InitEnemy();
	
	// 回收敌人到对象池时， 重置 ASC 状态 
	void ResetGASState();

	// 生成掉落物
	void DropItem() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void GiveStartAbilities();
	
	// 触发更新血条
	void TriggerUpdateAttributeBar() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeBillboardComponent> AttributeBillboard;
	
	FOnEnemyAttackFinished OnMeleeAttackFinished;
	FOnEnemyAttackFinished OnRangedAttackFinished;
	
private:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	int32 EnemyID;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<UGameplayAbility> MeleeGA;
	FGameplayAbilitySpecHandle MeleeAttackSpecHandle;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<UGameplayAbility> RangedGA;
	FGameplayAbilitySpecHandle RangedAttackSpecHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<AProjectile> ProjectileClass;
};
