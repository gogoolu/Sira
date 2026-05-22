// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnemy.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "Component/AttributeBillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/SiraAIController.h"
#include "Enemy/SiraEnemyPool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/Pool.h"
#include "Manager/SiraConfigSubsystem.h"
#include "Manager/SiraGameplayTags.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"

// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AIControllerClass = ASiraAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	if (auto MovementComp = GetCharacterMovement())
	{
		MovementComp->MaxWalkSpeed = 300.0f;
	}
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	AttributeBillboard = CreateDefaultSubobject<UAttributeBillboardComponent>("AttributeBillboard");
	AttributeBillboard->SetupAttachment(GetCapsuleComponent());
	
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("EnemyPreset");
}

UAbilitySystemComponent* ANormalEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 ANormalEnemy::GetEnemyID() const
{
	return EnemyID;
}

void ANormalEnemy::MeleeAttack()
{
	// OnMeleeAttackFinished.AddLambda(MoveTemp(Callback));
	AbilitySystemComponent->TryActivateAbility(MeleeAttackSpecHandle);
}

void ANormalEnemy::RangedAttack()
{
	// OnRangedAttackFinished.AddLambda(MoveTemp(Callback));
	AbilitySystemComponent->TryActivateAbility(RangedAttackSpecHandle);
}

bool ANormalEnemy::IsAttacking() const
{
	auto MeleeSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(MeleeAttackSpecHandle);
	if (MeleeSpec && MeleeSpec->IsActive())
	{
		return true;
	}
	auto RangedSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(RangedAttackSpecHandle);
	if (RangedSpec && RangedSpec->IsActive())
	{
		return true;
	}
	return false;
}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitEnemy();
	AbilitySystemComponent->RegisterGameplayTagEvent(SiraGameplayTags::TAG_LifeState_Dead).AddUObject(this, &ThisClass::OnDead);
	MeleeAttackSpecHandle = AbilitySystemComponent->GiveAbility(MeleeGA);
	RangedAttackSpecHandle = AbilitySystemComponent->GiveAbility(RangedGA);
	
	AbilitySystemComponent->OnAbilityEnded.AddWeakLambda(this, [this] (const FAbilityEndedData& EndedData)
	{
		if (EndedData.AbilitySpecHandle == MeleeAttackSpecHandle)
		{
			OnMeleeAttackFinished.Broadcast();
		} else if (EndedData.AbilitySpecHandle == RangedAttackSpecHandle)
		{
			OnRangedAttackFinished.Broadcast();
		}
	});
}

void ANormalEnemy::OnDead(FGameplayTag Tag, int32 Count)
{
	if (Count > 0)
	{
		if (AAIController* AIController = Cast<AAIController>(GetOwner()))
		{
			if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
			{
				BrainComp->StopLogic("Death");
			}
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		DropItem();
		Die();
	}
}

void ANormalEnemy::Die()
{
	auto EnemyPool = GetWorld()->GetSubsystem<USiraEnemyPool>();
	EnemyPool->RecycleEnemy(this);
}

void ANormalEnemy::InitEnemy()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveStartAbilities();
	
	// 添加敌人标签
	AbilitySystemComponent->AddLooseGameplayTag(SiraGameplayTags::TAG_Character_Enemy);
	
	if (AAIController* AIController = Cast<AAIController>(GetOwner()))
	{
		if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
		{
			// BrainComp->StopLogic("Death");
			BrainComp->StartLogic();
		}
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	TriggerUpdateAttributeBar();
}

void ANormalEnemy::ResetGASState()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	// AbilitySystemComponent->CancelAllAbilities();
	AbilitySystemComponent->RemoveActiveEffects(FGameplayEffectQuery());
	AbilitySystemComponent->RemoveAllGameplayCues();

	// remove tags
	FGameplayTagContainer PendingRemoveTags;
	AbilitySystemComponent->GetOwnedGameplayTags(PendingRemoveTags);
	for (const FGameplayTag& Tag : PendingRemoveTags)
	{
		while (AbilitySystemComponent->HasMatchingGameplayTag(Tag))
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(Tag);
		}
	}
	
	for (auto& [StartAttributeSet, StartingTable] : AbilitySystemComponent->DefaultStartingData)
	{
		AbilitySystemComponent->InitStats(StartAttributeSet, StartingTable);
	}
	
	AbilitySystemComponent->ClearActorInfo();
}

void ANormalEnemy::DropItem() const
{
	auto Config = GetWorld()->GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	auto EnemyConfig = Config->GetEnemyConfig(GetEnemyID());
	if (!EnemyConfig)
	{
		return;
	}
	if (FMath::FRand() <= EnemyConfig->DropRate)
	{
		auto Pool = GetWorld()->GetSubsystem<USiraDropItemPool>();
		if (auto DropConfig = Config->GetDropConfig(EnemyConfig->DropConfigID))
		{
			if (auto DropItemActor = Pool->Get(GetActorLocation()))
			{
				auto Index = FMath::RandRange(0, DropConfig->DropItems.Num() - 1);
				DropItemActor->SetDropInfo(DropConfig->DropItems[Index]);
			}
		}
	}
}

void ANormalEnemy::TriggerUpdateAttributeBar() const
{
	AttributeBillboard->UpdateAttributeBarByASC(AbilitySystemComponent);
}

void ANormalEnemy::GiveStartAbilities_Implementation()
{
}


