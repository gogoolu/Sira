#pragma once
#include "SiraGamePlay/Projectile.h"

#include "ExplosionProjectile.generated.h"

UCLASS()
class AExplosionProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AExplosionProjectile();
	
private:
	
	virtual void BeginPlay() override;
	
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	void Explode(const FRotator& Rotation = FRotator::ZeroRotator);
	
	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> ExplosionSounds;
};
