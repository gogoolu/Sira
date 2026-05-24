#include "Ability/GameplayAbility_SecondeAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Pool.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "SiraGamePlay/Projectile.h"

void UGameplayAbility_SecondeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
		AttackMontage
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
	auto Pool = GetWorld()->GetSubsystem<UProjectilePool>();
	FActorSpawnParameters Parameters;
	Parameters.Owner = SiraPlayer;
	Parameters.Instigator = SiraPlayer;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto StartLocation = SiraPlayer->GetPlayerShootStartPoint();
	auto Direction = CameraManager->GetCameraRotation();
	if (auto Projectile = Pool->Get(ProjectileClass, Parameters, StartLocation, Direction))
	{
		Projectile->SetProjectileMaxSpeed(2000.f);
		Projectile->SetProjectileVelocity(Direction.Vector().GetSafeNormal() * 2000.f);
		if (ShotSounds.Num() > 0)
		{
			auto RandomSound = ShotSounds[FMath::RandRange(0, ShotSounds.Num() - 1)];
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RandomSound, SiraPlayer->GetActorLocation());
		}
	}
}

void UGameplayAbility_SecondeAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_SecondeAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}