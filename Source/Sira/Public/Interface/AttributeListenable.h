#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeListenable.generated.h"

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
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetListenAttributes(TArray<FGameplayAttribute>& OutAttributes);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnListenAttributeChanged(const FGameplayAttribute& Attribute, float NewValue, float OldValue);
};
