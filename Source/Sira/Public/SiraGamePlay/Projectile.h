#pragma once
#include "GameplayTagContainer.h"


#include "Projectile.generated.h"


class UGameplayEffect;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AProjectile();
	
	void SetPenetrate(int32 InPenetrate);
	void SetProjectileVelocity(const FVector& InVelocity);
	void SetProjectileMaxSpeed(float InMaxSpeed);
	void SetTrailNiagara(UNiagaraSystem* InNiagara);
	void SetImpactNiagara(UNiagaraSystem* InNiagara);
	void ResetProjectile();
	void SetTag(FGameplayTag InSelfTag);
	TSubclassOf<UGameplayEffect> GetHitGEClass() const;

protected:
	void PlayHitImpact(const FVector& InImpactPoint, const FRotator& InImpactRotation) const;
	void DestroyProjectile();
	virtual void BeginPlay() override;
	
private:
	
	
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
	
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	int32 Penetrate = 0;
	
	FTimerHandle Handle;
	FGameplayTag SelfTag;
	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> TrailNiagara;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactNiagara;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactEmitter;
	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> HitGEClass;
	
	UPROPERTY(EditAnywhere)
	float LifeTime = 5.f; // s
};
