// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SiraHealthUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIRA_API USiraHealthUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category="Health")
	TObjectPtr<AActor> TargetActor;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void BindTargetActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void UnBindActor();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Health")
	void UpdateHealthBar(float CurrentHP, float MaxHP);
	
private:
	
	virtual void NativeDestruct() override;
};
