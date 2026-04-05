#include "Anim/PlayerAnim.h"

#include "KismetAnimationLibrary.h"
#include "SiraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/SiraPlayer.h"


void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AnimPlayer = Cast<ASiraPlayer>(TryGetPawnOwner());
	if (IsValid(AnimPlayer))
	{
		PlayerCharacterMovement = AnimPlayer->GetCharacterMovement();
	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (IsValid(PlayerCharacterMovement))
	{
		Velocity = PlayerCharacterMovement->Velocity;
		Orientation = AnimPlayer->GetActorRotation();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Orientation);
		GroudSpeed = Velocity.Size2D();
		Acceleration = PlayerCharacterMovement->GetCurrentAcceleration();
		bIsFalling = PlayerCharacterMovement->IsFalling();
		
		bShouldMove = !FMath::IsNearlyZero(GroudSpeed) && !PlayerCharacterMovement->GetCurrentAcceleration().IsNearlyZero();
		bShouldRelax = !AnimPlayer->GetSlidingMode();
		
		
		// Calculate Aim Offset Param
		AimOffsetYaw = AnimPlayer->GetViewRotation().Yaw - AnimPlayer->GetActorRotation().GetDenormalized().Yaw;
		if (AimOffsetYaw > 180.f)
		{
			AimOffsetYaw = AimOffsetYaw - 360.f;
		} else if (AimOffsetYaw < -180.f)
		{
			AimOffsetYaw = AimOffsetYaw + 360.f;
		}
		AimOffsetPitch = AnimPlayer->GetBaseAimRotation().Pitch + 30; 
	}
}

