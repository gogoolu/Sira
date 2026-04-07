// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SiraPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ASiraPlayer::ASiraPlayer()
{
	// create SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 20;
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	
	// create Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	CameraBoom->bUsePawnControlRotation = true;
	PlayerCamera->bUsePawnControlRotation = false;
	
	// 默认情况不使用 Controller 的朝向，由运动状态自行决定
	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->RotationRate.Yaw = 640.0f;
	}
}

void ASiraPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ULocalPlayer* Player = PlayerController->GetLocalPlayer())
		{	
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMapping, 0);
			}
		}
	}
	
	BindAttributeEvent();
}

void ASiraPlayer::BindAttributeEvent()
{
	if (IsValid(AbilitySystemComponent))
	{
		FOnAttributeChangeData AttributeChangeData {};
		// HP
		auto HPAttr = UBaseAttributeSet::GetHPAttribute();
		float HP = AbilitySystemComponent->GetNumericAttribute(HPAttr);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HPAttr)
		.AddUObject(this, &ThisClass::HandleAttributeChanged);
		// manually trigger once
		AttributeChangeData.Attribute = HPAttr;
		AttributeChangeData.NewValue = HP;
		AttributeChangeData.OldValue = HP;
		HandleAttributeChanged(AttributeChangeData);
		
		// Max HP
		auto MaxHPAttr = UBaseAttributeSet::GetMaxHPAttribute();
		float MaxHP = AbilitySystemComponent->GetNumericAttribute(MaxHPAttr);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHPAttr)
		.AddUObject(this, &ThisClass::HandleAttributeChanged);
		// manually trigger once
		AttributeChangeData.Attribute = MaxHPAttr;
		AttributeChangeData.NewValue = MaxHP;
		AttributeChangeData.OldValue = MaxHP;
		HandleAttributeChanged(AttributeChangeData);
	}
}

void ASiraPlayer::HandleAttributeChanged(const FOnAttributeChangeData& Data) const
{
	if (Data.Attribute == UBaseAttributeSet::GetHPAttribute())
	{
		OnHPChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
	} else if (Data.Attribute == UBaseAttributeSet::GetMaxHPAttribute())
	{
		OnMaxHPChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
	}
}


// Called to bind functionality to input
void ASiraPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASiraPlayer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASiraPlayer::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASiraCharacter::Jump);
	}
}

void ASiraPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FDateTime::Now().GetTicks() - LastSlideTime > ETimespan::TicksPerSecond)
	{
		SetSlidingMode(false);
	}
}

void ASiraPlayer::Attack()
{
	// Super::Attack();
	SetSlidingMode(true);
}

void ASiraPlayer::Move(const FInputActionValue &Value)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		const FVector2D MoveVector = Value.Get<FVector2D>();
		AddMovementInput(ForwardDirection, MoveVector.X);
		AddMovementInput(RightDirection, MoveVector.Y);
		SetSlidingMode(true);
	}
}

void ASiraPlayer::Look(const FInputActionValue &Value)
{
	if (Controller)
	{
		const FVector2D LookVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ASiraPlayer::SetSlidingMode(bool InSlidingMode)
{
	bIsSlidingMode = InSlidingMode;
	GetCharacterMovement()->bUseControllerDesiredRotation = InSlidingMode;
	if (InSlidingMode)
	{
		LastSlideTime = FDateTime::Now().GetTicks();
	}
}

bool ASiraPlayer::GetSlidingMode() const
{
	return bIsSlidingMode;
}
