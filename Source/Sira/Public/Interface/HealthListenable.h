#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthListenable.generated.h"

struct FGameplayAttribute;

UINTERFACE(Blueprintable)
class UAttributeListenable : public UInterface
{
	GENERATED_BODY()
};

class IAttributeListenable
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetListenAttributes(TArray<FGameplayAttribute>& OutAttributes);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnListenAttributeChanged(const FGameplayAttribute& Attribute, float NewValue, float OldValue);
};
