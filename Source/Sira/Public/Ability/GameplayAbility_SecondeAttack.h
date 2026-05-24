#pragma once

#include "SiraGameplayAbility.h"


#include "GameplayAbility_SecondeAttack.generated.h"

class AProjectile;

UCLASS()
class UGameplayAbility_SecondeAttack : public USiraGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();

	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TSubclassOf<AProjectile> ProjectileClass; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TArray<TObjectPtr<USoundWave>> ShotSounds;
};
