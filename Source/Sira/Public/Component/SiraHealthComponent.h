// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/SiraHealthUserWidget.h"

#include "SiraHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnHealthChanged,
	float, CurrentHealth,
	float, MaxHealth
);

// class USiraHealthUserWidget;

UCLASS( ClassGroup=(Sira), meta=(BlueprintSpawnableComponent) )
class SIRA_API USiraHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	// Sets default values for this component's properties
	USiraHealthComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHP() const;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHP() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Health")
	TSubclassOf<USiraHealthUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Health")
	FOnHealthChanged OnHealthChanged;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void HandleOwnerHPChanged(const struct FOnAttributeChangeData& Data);
	void HandleOwnerMaxHPChanged(const FOnAttributeChangeData& Data);
	
	float CurrentHP;
	float MaxHP;
};
