// Fill out your copyright notice in the Description page of Project Settings.


#include "SiraCharacter.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "Manager/SiraGameplayTags.h"

// Sets default values
ASiraCharacter::ASiraCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

void ASiraCharacter::Attack()
{
	
}

// Called when the game starts or when spawned
void ASiraCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		auto HPAttr = UBaseAttributeSet::GetHPAttribute();
		AbilitySystemComponent->RegisterGameplayTagEvent(FSiraGameplayTags::GetTag_LifeState_Dead()).AddUObject(this, &ThisClass::OnDead);
	}
}

void ASiraCharacter::OnDead(FGameplayTag Tag, int32 Count)
{
}

// Called every frame
void ASiraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


UAbilitySystemComponent* ASiraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

