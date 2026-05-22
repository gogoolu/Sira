#include "Component/AttributeBillboardComponent.h"


void UAttributeBillboardComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateDynamicMaterialInstance(0);
}

void UAttributeBillboardComponent::UpdateAttributeBarByASC(const UAbilitySystemComponent* ASC) const
{
	if (ASC)
	{
		float HP = ASC->GetNumericAttribute(UEnemyAttr::GetHPAttribute());
		float MaxHP = ASC->GetNumericAttribute(UEnemyAttr::GetMaxHPAttribute());
		if (!FMath::IsNearlyZero(MaxHP))
		{
			SetAttributeBarPercent(HP / MaxHP);
		}
	}
}

void UAttributeBillboardComponent::SetAttributeBarPercent(float Percent) const
{
	if (auto Mat = Cast<UMaterialInstanceDynamic>(GetMaterial(0)))
	{
		Mat->SetScalarParameterValue("FillPercent", Percent);
	}
}
