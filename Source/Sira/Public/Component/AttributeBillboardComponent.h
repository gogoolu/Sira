#pragma once

#include "Components/MaterialBillboardComponent.h"
#include "Interface/AttributeListenable.h"
#include "AttributeSet/BaseAttributeSet.h"

#include "AttributeBillboardComponent.generated.h"

// manage attribute bar and make it face to camera 
UCLASS(ClassGroup=(Sira), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class UAttributeBillboardComponent : public UMaterialBillboardComponent, public IAttributeListenable
{
	GENERATED_BODY()
	
public:
	virtual void GetListenAttributes_Implementation(TArray<FGameplayAttribute>& OutAttributes) override;
private:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	TArray<FGameplayAttribute> ListenAttributes;
};

