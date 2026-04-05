// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SiraHealthComponent.h"

#include "SiraCharacter.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Component/FaceCameraWidgetComponent.h"

// Sets default values for this component's properties
USiraHealthComponent::USiraHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USiraHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind ASC HP Attribute change event to Component 
	if (auto Owner = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent())
		{
			FGameplayAttribute HPAttribute = UBaseAttributeSet::GetHPAttribute();
			FGameplayAttribute MaxHPAttribute = UBaseAttributeSet::GetMaxHPAttribute();
			
			ASC->GetGameplayAttributeValueChangeDelegate(HPAttribute).AddUObject(this, &USiraHealthComponent::HandleOwnerHPChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(MaxHPAttribute).AddUObject(this, &USiraHealthComponent::HandleOwnerMaxHPChanged);
			CurrentHP = ASC->GetNumericAttribute(HPAttribute);
			MaxHP = ASC->GetNumericAttribute(MaxHPAttribute);
		}
	}
	
	// Attach HealthBar Widget to Owner, and Bind Event
	if (auto OwnerActor = Cast<AActor>(GetOwner()))
	{
		auto HealthBarWidget = NewObject<UFaceCameraWidgetComponent>(OwnerActor);
		HealthBarWidget->RegisterComponent();
		HealthBarWidget->SetWidgetClass(HealthBarWidgetClass);
		HealthBarWidget->SetDrawSize({200.0f, 10.0f});
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
		HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		if (USiraHealthUserWidget* WidgetObject = Cast<USiraHealthUserWidget>(HealthBarWidget->GetUserWidgetObject()))
		{
			WidgetObject->BindTargetActor(OwnerActor);
		}
		
		HealthBarWidget->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		
		if (auto OwnerRoot = OwnerActor->GetRootComponent())
		{
			if (auto Capsule = Cast<UCapsuleComponent>(OwnerRoot))
			{
				FVector HealthBarLocation = Capsule->GetComponentLocation() + FVector(0.0f, 0.0f, Capsule->GetScaledCapsuleHalfHeight());
				HealthBarWidget->SetWorldLocation(HealthBarLocation);
			}
		}
	}
}

void USiraHealthComponent::HandleOwnerHPChanged(const FOnAttributeChangeData& Data)
{
	CurrentHP = Data.NewValue;
	OnHealthChanged.Broadcast(CurrentHP, MaxHP);
}

void USiraHealthComponent::HandleOwnerMaxHPChanged(const FOnAttributeChangeData& Data)
{
	MaxHP = Data.NewValue;
	OnHealthChanged.Broadcast(CurrentHP, MaxHP);
}

float USiraHealthComponent::GetHP() const
{
	return CurrentHP;
}

float USiraHealthComponent::GetMaxHP() const
{
	return MaxHP;
}
