// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiraCharacter.h"

#include "NormalEnemy.generated.h"

class USiraAttributeComponent;

UENUM()
enum class EEnemyLifeState : uint8
{
	Alive,
	PendingDeath,
	Dead,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPendingDeath);

UCLASS()
class SIRA_API ANormalEnemy : public ASiraCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemy();
	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	
private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Mark pending die and play montage
	virtual void OnDead(FGameplayTag Tag, int32 Count) override;
	
	// bind Death AnimNotify, real destroy
	virtual void Die();
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	EEnemyLifeState LifeState = EEnemyLifeState::Alive;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USiraAttributeComponent> SiraAttributeComponent;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
