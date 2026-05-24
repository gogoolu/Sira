#pragma once

#include "SiraGamePlay/DropItem.h"

#include "Pool.generated.h"

class AProjectile;

UCLASS()
class USiraDropItemPool : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	ADropItem* Get();
	ADropItem* Get(const FVector& Location);
	void Recycle(ADropItem* DropItem);
	void RecycleAll();
	
private:
	
	TArray<TObjectPtr<ADropItem>> Pool;
};


UCLASS()
class UProjectilePool : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RecycleAll();
	void Recycle(AProjectile* Projectile);
	// void Get(TSubclassOf<AProjectile> ProjectileClass);
	AProjectile* Get(TSubclassOf<AProjectile> ProjectileClass, const FActorSpawnParameters& Parameters = {}, 
		const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);
	
private:
	
	TMap<TSubclassOf<AProjectile>, TArray<AProjectile*>> Pool;
};