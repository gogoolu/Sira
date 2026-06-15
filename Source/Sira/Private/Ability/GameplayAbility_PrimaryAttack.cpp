#include "Ability/GameplayAbility_PrimaryAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Enemy/NormalEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/SiraBlueprintLibrary.h"

void UGameplayAbility_PrimaryAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* Task =
	UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
	PrimaryAttackMontage
	);
	
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
	
	Task->ReadyForActivation();
	
	auto SiraPlayer = USiraBlueprintLibrary::GetSiraPlayer(GetWorld());
	auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!SiraPlayer || !CameraManager)
	{
		return;
	}
	
	// int32 Penetrate = SiraPlayer->AbilitySystemComponent->GetNumericAttribute(UPlayerAttr::)
	
	auto StartLocation = SiraPlayer->GetPlayerShootStartPoint();
	auto EndLocation = StartLocation + (CameraManager->GetCameraRotation().Vector() * 10000.f);
	
	Shoot(StartLocation, EndLocation);
}

void UGameplayAbility_PrimaryAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_PrimaryAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_PrimaryAttack::Shoot(const FVector& StartLocation, const FVector& EndLocation, int32 Penetrate)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwningActorFromActorInfo());
	TArray<FHitResult> HitResults;
	if (bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		ECC_GameTraceChannel5,
		Params
	))
	{
		auto SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC)
		{
			return;
		}
		GenBulletTrailStartWithBarrel(HitResults[0].Location);
		
		TArray<AActor*> HandledActors;
		for (const FHitResult& Hit : HitResults)
		{
			if (Penetrate < 0)
			{
				break;
			}
			auto HitActor = Hit.GetActor();
			if (!HandledActors.Contains(HitActor))
			{
				HandledActors.Add(HitActor);
				auto HitEnemy = Cast<ANormalEnemy>(HitActor);
				if (!HitEnemy)
				{
					break;
				}
				if (ImpactEmitter)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
						ImpactEmitter,
						Hit.ImpactPoint,
						Hit.Normal.ToOrientationRotator()
					);
				} 
			
				UGameplayEffect* HitGE = HitGEClass->GetDefaultObject<UGameplayEffect>();
				SourceASC->ApplyGameplayEffectToTarget(HitGE, HitEnemy->AbilitySystemComponent);
			
				Penetrate--;
			}
		}
	}
	
}

void UGameplayAbility_PrimaryAttack::GenBulletTrailStartWithBarrel(const FVector& EndLocation)
{
	if (auto SiraPlayer = USiraBlueprintLibrary::GetSiraPlayer(GetWorld()))
	{
		auto BarrelTransform = SiraPlayer->GetMesh()->GetSocketTransform("SMG_Barrel");
		if (ShotSounds.Num() > 0)
		{
			auto RandomSound = ShotSounds[FMath::RandRange(0, ShotSounds.Num() - 1)];
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RandomSound, BarrelTransform.GetLocation());
		}
		
		auto BulletTrail = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TrailNiagara,
			BarrelTransform.GetLocation(),
			BarrelTransform.GetRotation().Rotator()
		);
		BulletTrail->SetVectorParameter("Start", BarrelTransform.GetLocation());
		BulletTrail->SetVectorParameter("Direction", (EndLocation - BarrelTransform.GetLocation()).GetSafeNormal());
	}
}
