#pragma once
#include "SiraGamePlay/Projectile.h"

UCLASS()
class AExplosionProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
};
