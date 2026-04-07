#pragma once
#include "CoreMinimal.h"
#include "PlayerAnim.generated.h"

class ASiraCharacter;
class ASiraPlayer;
class UCharacterMovementComponent;

UCLASS()
class UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	
	UPlayerAnim() {}
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASiraPlayer> AnimPlayer;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> PlayerCharacterMovement;
	
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	FVector Velocity;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	FRotator Orientation;

	// delta between player face orientation and ground velocity orientation 
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float Direction;
	
	UPROPERTY(BlueprintReadOnly, Category="AimOffset")
	float AimOffsetYaw;
	
	UPROPERTY(BlueprintReadOnly, Category="AimOffset")
	float AimOffsetPitch;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float GroudSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	FVector Acceleration;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bShouldMove;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bShouldRelax;
	
	UPROPERTY(BlueprintReadOnly, Category="State")
	bool bIsAttacking;
	
private:
	// if Time longer than a TimeToOutOfMoveMode, then Switch bAllowSlide to false
	float LastMoveDeltaTime = 0;
};
