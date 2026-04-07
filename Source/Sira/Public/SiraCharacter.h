#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SiraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SIRA_API ASiraCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASiraCharacter();
	
	// Ability Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void Attack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Mark Character Pending die
	virtual void OnDead(FGameplayTag Tag, int32 Count);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	
};
