// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SiraHealthUserWidget.h"

#include "Component/SiraHealthComponent.h"

void USiraHealthUserWidget::BindTargetActor(AActor* Actor)
{
	UnBindActor();
	if (IsValid(Actor))
	{
		TargetActor = Actor;
		if (USiraHealthComponent* HealthComp = TargetActor->GetComponentByClass<USiraHealthComponent>())
		{
			HealthComp->OnHealthChanged.RemoveDynamic(this, &USiraHealthUserWidget::UpdateHealthBar);
			HealthComp->OnHealthChanged.AddDynamic(this, &USiraHealthUserWidget::UpdateHealthBar);
			UpdateHealthBar(HealthComp->GetHP(), HealthComp->GetMaxHP());
		}
	}
}

void USiraHealthUserWidget::UnBindActor()
{
	if (IsValid(TargetActor))
	{
		if (USiraHealthComponent* HealthComp = TargetActor->GetComponentByClass<USiraHealthComponent>())
		{
			HealthComp->OnHealthChanged.RemoveDynamic(this, &USiraHealthUserWidget::UpdateHealthBar);
		}
	}
	TargetActor = nullptr;
}

void USiraHealthUserWidget::NativeDestruct()
{
	UnBindActor();
	Super::NativeDestruct();
}

