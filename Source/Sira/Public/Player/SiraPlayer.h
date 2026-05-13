// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiraCharacter.h"
#include "SiraPlayer.generated.h"


class UNiagaraSystem;
class UInventoryComponent;
class UInputComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, 
	const FGameplayAttribute&, Attribute, 
	float, NewValue,
	float, OldValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UCLASS()
class SIRA_API ASiraPlayer : public ASiraCharacter
{
	GENERATED_BODY()
	
public:
	
	ASiraPlayer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	FVector GetPlayerShootStartPoint() const;
	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;
	
	// ================== INPUT ====================
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction; 

	// ================ Move Sliding =============
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetSlidingMode(bool InSlidingMode);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetSlidingMode() const;
	
	// ================ GAS Attributes Change Event ==========
	void BindAttributeEvent();
	void HandleAttributeChanged(const FOnAttributeChangeData& Data) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnAttributeChanged OnHPChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOnAttributeChanged OnMaxHPChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	FOnPlayerDead OnPlayerDead;
private:
	
	// ================ Movement ==================
	// Action Response
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	bool bIsSlidingMode;
	int64 LastSlideTime;
	
	// 背包
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	// =================== Camera ====================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> PlayerCamera;
	
	
	virtual void OnDead(FGameplayTag Tag, int32 Count) override;
	
	void OnAbilityActivated(UGameplayAbility* Ability);
	
	void StartRagDoll() const;
};
