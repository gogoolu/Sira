// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnemy.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "Component/SiraAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/SiraGameplayTags.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"

// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	if (auto MovementComp = GetCharacterMovement())
	{
		MovementComp->MaxWalkSpeed = 300.0f;
	}
	SiraAttributeComponent = CreateDefaultSubobject<USiraAttributeComponent>(TEXT("Attribute"));
}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SiraAttributeComponent->Init();
}

// Called every frame
// void ANormalEnemy::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

void ANormalEnemy::OnDead(FGameplayTag Tag, int32 Count)
{
	if (Count > 0)
	{
		LifeState = EEnemyLifeState::PendingDeath;
		if (AAIController* AIController = Cast<AAIController>(GetOwner()))
		{
			if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
			{
				BrainComp->StopLogic("Death");
			}
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		// GetMesh()->SetSimulatePhysics(true);
		Die();
	}
}

void ANormalEnemy::Die()
{
	LifeState = EEnemyLifeState::Dead;
	Destroy();
}


