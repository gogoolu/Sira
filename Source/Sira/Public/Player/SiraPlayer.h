// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiraCharacter.h"
#include "SiraPlayer.generated.h"
struct FInputActionValue;


UCLASS()
class SIRA_API ASiraPlayer : public ASiraCharacter
{
	GENERATED_BODY()
	
public:
	
	ASiraPlayer();
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack() override;
	
	void SetSlidingMode(bool InSlidingMode);
	bool GetSlidingMode() const;
private:
	
	// ================ Movement ==================
	// Action Response
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	// void Jump();
	
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	bool bIsSlidingMode;
	int64 LastSlideTime;
	
	// ================== INPUT ====================
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	
	// =================== Camera ====================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> PlayerCamera;
	
};
