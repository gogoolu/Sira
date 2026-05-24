#include "SiraGamePlay/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Pool.h"
#include "Player/SiraPlayer.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	Collision = CreateDefaultSubobject<USphereComponent>("SphereComponent");

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionProfileName("Projectile");
	Collision->InitSphereRadius(15.f);
	Collision->SetNotifyRigidBodyCollision(true);
	
	RootComponent = Collision;
	
	TrailNiagara = CreateDefaultSubobject<UNiagaraComponent>("TrailNiagara");
	TrailNiagara->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	PlayHitImpact(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	DestroyProjectile();
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}
	
	if (!HitGEClass)
	{
		DestroyProjectile();
		return;
	}
	
	auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	auto SourceASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	if (!TargetASC || !SourceASC)
	{
		DestroyProjectile();
		return;
	}
	if (TargetASC->HasMatchingGameplayTag(SelfTag))
	{
		return;
	}
	auto EffectContext = SourceASC->MakeEffectContext();
	auto GESpecHandle = SourceASC->MakeOutgoingSpec(HitGEClass, 1.f, EffectContext);
	if (GESpecHandle.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), TargetASC);
	}
	if (Penetrate <= 0)
	{
		DestroyProjectile();
	}
	Penetrate--;
	PlayHitImpact(GetActorLocation(), GetActorRotation());
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
	ResetProjectile();
}

void AProjectile::SetTrailNiagara(UNiagaraSystem* InNiagara)
{
	TrailNiagara->SetAsset(InNiagara);
}

void AProjectile::SetImpactNiagara(UNiagaraSystem* InNiagara)
{
	ImpactNiagara = InNiagara;
}

void AProjectile::ResetProjectile()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
	GetWorld()->GetTimerManager().SetTimer(Handle, [WeakThis = MakeWeakObjectPtr(this)]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->DestroyProjectile();
		}
	}, LifeTime, false);
	Penetrate = 1;
	TrailNiagara->Activate();
}

void AProjectile::SetTag(FGameplayTag InSelfTag)
{
	SelfTag = InSelfTag;
}

TSubclassOf<UGameplayEffect> AProjectile::GetHitGEClass() const
{
	return HitGEClass;
}

void AProjectile::SetPenetrate(int32 InPenetrate)
{
	Penetrate = InPenetrate;
}

void AProjectile::SetProjectileVelocity(const FVector& InVelocity)
{
	ProjectileMovement->Velocity = InVelocity;
}

void AProjectile::SetProjectileMaxSpeed(float InMaxSpeed)
{
	ProjectileMovement->MaxSpeed = InMaxSpeed;
}

void AProjectile::DestroyProjectile()
{
	TrailNiagara->DeactivateImmediate();
	GetWorld()->GetTimerManager().ClearTimer(Handle);
	auto Pool = GetWorld()->GetSubsystem<UProjectilePool>();
	Pool->Recycle(this);
}

void AProjectile::PlayHitImpact(const FVector& InImpactPoint, const FRotator& InImpactRotation) const
{
	if (ImpactNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactNiagara,
			InImpactPoint,
			InImpactRotation
		);
	}
	
	if (ImpactEmitter)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
			ImpactEmitter,
			InImpactPoint,
			InImpactRotation
		);
	} 
}

