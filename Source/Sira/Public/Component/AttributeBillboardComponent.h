#pragma once

#include "Components/MaterialBillboardComponent.h"
#include "Interface/AttributeListenable.h"
#include "AttributeSet/BaseAttributeSet.h"

#include "AttributeBillboardComponent.generated.h"

// manage attribute bar and make it face to camera 
UCLASS(ClassGroup=(Sira), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class UAttributeBillboardComponent : public UMaterialBillboardComponent
{
	GENERATED_BODY()
	
public:
	void UpdateAttributeBarByASC(const UAbilitySystemComponent* ASC) const;
	void SetAttributeBarPercent(float Percent) const;
	
private:
	virtual void BeginPlay() override;
};

