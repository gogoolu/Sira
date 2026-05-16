#pragma once
#include "Abilities/GameplayAbility.h"


#include "SiraGameplayAbility.generated.h"

UCLASS(BlueprintType)
class USiraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	
	UFUNCTION(BlueprintCallable)
	float GetCoolDownDuration() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiraCoolDown", meta = (AllowPrivateAccess = true))
	FGameplayTag CoolDownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SiraCoolDown", meta = (AllowPrivateAccess = true))
	float BaseCoolDown = 1.f;
	
	UPROPERTY(Transient)
	mutable FGameplayTagContainer TempCooldownTags;
};
