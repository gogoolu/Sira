#pragma once
#include "SiraGameplayAbility.h"
#include "Abilities/GameplayAbility.h"


#include "GameplayAbility_PrimaryAttack.generated.h"

class UNiagaraSystem;

UCLASS()
class UGameplayAbility_PrimaryAttack : public USiraGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	
private:
	
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();
	
	void Shoot(const FVector& StartLocation, const FVector& EndLocation, int32 Penetrate = 1);
	void GenBulletTrailStartWithBarrel(const FVector& EndLocation);
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TSubclassOf<UGameplayEffect> HitGEClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TObjectPtr<UAnimMontage> PrimaryAttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TObjectPtr<UNiagaraSystem> TrailNiagara;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sira")
	TArray<TObjectPtr<USoundWave>> ShotSounds;
};
