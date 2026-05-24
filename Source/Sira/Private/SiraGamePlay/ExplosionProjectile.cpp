#include "SiraGamePlay/ExplosionProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Enemy/NormalEnemy.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

AExplosionProjectile::AExplosionProjectile()
{
}

void AExplosionProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosionProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}
	Explode();
}

void AExplosionProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AExplosionProjectile::Explode(const FRotator& Rotation)
{
	if (ExplosionSounds.Num() > 0)
	{
		auto RandomSound = ExplosionSounds[FMath::RandRange(0, ExplosionSounds.Num() - 1)];
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RandomSound, GetActorLocation());
	}
	TArray<FOverlapResult> Overlaps;
	auto SourceASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	if (!SourceASC || !GetHitGEClass())
	{
		DestroyProjectile();
		return;
	}
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	PlayHitImpact(GetActorLocation(), Rotation);
	if (GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_GameTraceChannel5,
		Sphere
	))
	{
		// results中可能有多个相同的 Actor，需要去重
		TArray<AActor*> CalculatedActors; 
		for (const auto& Result : Overlaps)
		{
			auto HitActor = Result.GetActor();
			if (!CalculatedActors.Contains(HitActor))
			{
				CalculatedActors.Add(HitActor);
				if (auto Enemy = Cast<ANormalEnemy>(HitActor))
				{
					UGameplayEffect* HitGE = GetHitGEClass()->GetDefaultObject<UGameplayEffect>();
					SourceASC->ApplyGameplayEffectToTarget(HitGE, Enemy->AbilitySystemComponent);
				}
			}
			
		}
	}
	DestroyProjectile();
}
