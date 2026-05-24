// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SiraPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SiraGameMode.h"
#include "SiraLog.h"
#include "SiraPlayerController.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Component/InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Manager/SiraGameplayTags.h"


ASiraPlayer::ASiraPlayer()
{
	PrimaryActorTick.bCanEverTick = false;
	
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
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}

void ASiraPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->AddLooseGameplayTag(SiraGameplayTags::TAG_Character_Player);
	
	BindAttributeEvent();
	
	AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::OnAbilityActivated);
	
	if (auto GameMode = Cast<ASiraGameMode>(GetWorld()->GetAuthGameMode()))
	{
		OnPlayerDead.AddDynamic(GameMode, &ASiraGameMode::HandlePlayerDead);
	}
}

void ASiraPlayer::OnAbilityActivated(UGameplayAbility* Ability)
{
	if (Ability->GetAssetTags().HasTag(SiraGameplayTags::TAG_Attack))
	{
		SetSlidingMode(true);
	}
}

void ASiraPlayer::BindAttributeEvent()
{
	if (IsValid(AbilitySystemComponent))
	{
		FOnAttributeChangeData AttributeChangeData {};
		// HP
		auto HPAttr = UPlayerAttr::GetHPAttribute();
		float HP = AbilitySystemComponent->GetNumericAttribute(HPAttr);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HPAttr)
		.AddUObject(this, &ThisClass::HandleAttributeChanged);
		// manually trigger once
		AttributeChangeData.Attribute = HPAttr;
		AttributeChangeData.NewValue = HP;
		AttributeChangeData.OldValue = HP;
		HandleAttributeChanged(AttributeChangeData);
		
		// Max HP
		auto MaxHPAttr = UPlayerAttr::GetMaxHPAttribute();
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
	if (Data.Attribute == UPlayerAttr::GetHPAttribute())
	{
		OnHPChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
	} else if (Data.Attribute == UPlayerAttr::GetMaxHPAttribute())
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
		if (!LookAction)
		{
			UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::SetupPlayerInputComponent] LookAction is nullptr"));
		}
		if (!MoveAction)
		{
			UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::SetupPlayerInputComponent] MoveAction is nullptr"));
		}
		if (!JumpAction)
		{
			UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::SetupPlayerInputComponent] JumpAction is nullptr"));
		}
		if (!AttackAction)
		{
			UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::SetupPlayerInputComponent] AttackAction is nullptr"));
		}
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASiraPlayer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASiraPlayer::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASiraCharacter::Jump);
		// EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASiraPlayer::Attack);
	}
}

UInventoryComponent* ASiraPlayer::GetInventoryComponent() const
{
	return InventoryComponent;
}

FVector ASiraPlayer::GetPlayerShootStartPoint() const
{
	if (PlayerCamera)
	{
		float DotProduction = PlayerCamera->GetForwardVector().Dot(GetActorForwardVector().GetSafeNormal());
		if (FMath::IsNearlyZero(DotProduction))
		{
			// camera direction is parallel with plane
			return PlayerCamera->GetComponentLocation();
		}
		// not parallel, calculate intersection
		return FMath::LinePlaneIntersection(PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector(), GetActorLocation(), GetActorForwardVector().GetSafeNormal());
	}
	return GetActorLocation();
}

// void ASiraPlayer::Dash()
// {
// 	LaunchCharacter(GetLastMovementInputVector().GetSafeNormal2D() * 1000, false, false);
// }

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

void ASiraPlayer::OnDead(FGameplayTag Tag, int32 Count)
{
	if (Count > 0)
	{
		DisableInput(Cast<ASiraPlayerController>(GetController()));
		CameraBoom->TargetArmLength += 200;
		if (auto MoveComp = GetCharacterMovement())
		{
			MoveComp->DisableMovement();
		}
		// StartRagDoll();
		int32 MontageIndex = FMath::RandRange(0, DeathMontages.Num() - 1);
		PlayAnimMontage(DeathMontages[MontageIndex]);
		
		if (auto PC = Cast<ASiraPlayerController>(GetController()))
		{
			PC->SetInputMode(FInputModeUIOnly());
			PC->SetShowMouseCursor(true);
		}
		
		OnPlayerDead.Broadcast();
	}
}

void ASiraPlayer::StartRagDoll() const
{
	auto MeshComponent = GetMesh();
	if (!MeshComponent) return;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->WakeAllRigidBodies();
	// MeshComponent->HideBoneByName(TEXT("weapon"), PBO_Term);
	
	if (auto MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetComponentTickEnabled(false);
	}
}

void ASiraPlayer::SetSlidingMode(bool InSlidingMode)
{
	bIsSlidingMode = InSlidingMode;
	GetCharacterMovement()->bUseControllerDesiredRotation = InSlidingMode;
	if (InSlidingMode)
	{
		// LastSlideTime = FDateTime::Now().GetTicks();
		
		GetWorld()->GetTimerManager().ClearTimer(SlidingModeTimer);
		GetWorld()->GetTimerManager().SetTimer(SlidingModeTimer, [WeakThis = MakeWeakObjectPtr(this)]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->SetSlidingMode(false);
			}
		}, 1.f, false);
	}
}

bool ASiraPlayer::GetSlidingMode() const
{
	return bIsSlidingMode;
}
