// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiraCharacter.h"
#include "SiraPlayer.generated.h"
class UInventoryComponent;
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;
	
	// ================== INPUT ====================
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	
	// ================ Move Sliding =============
	virtual void Attack() override;
	void SetSlidingMode(bool InSlidingMode);
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

	
	void StartRagDoll() const;
};
