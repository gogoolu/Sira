#include "Component/AttributeBillboardComponent.h"


void UAttributeBillboardComponent::GetListenAttributes_Implementation(TArray<FGameplayAttribute>& OutAttributes)
{
	OutAttributes = ListenAttributes;
}

void UAttributeBillboardComponent::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < Elements.Num(); i++)
	{
		CreateDynamicMaterialInstance(i);
	}
}
