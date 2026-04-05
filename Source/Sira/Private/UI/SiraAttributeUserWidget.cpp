#include "UI/SiraAttributeUserWidget.h"
#include "AbilitySystemComponent.h"


void USiraAttributeUserWidget::Init(UAbilitySystemComponent* InASC)
{
	if (IsValid(InASC))
	{
		ClearHandle();
		ASC = InASC;
		for (auto& Attribute : ListenAttributes)
		{
			FDelegateHandle Handle = InASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &USiraAttributeUserWidget::OnAttributeChanged);
			DelegateHandleMap.Add(Attribute, Handle);
		}
	}
}

void USiraAttributeUserWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	HandleAttributeChanged(Data.Attribute, Data.NewValue, Data.OldValue);
}

void USiraAttributeUserWidget::NativeDestruct()
{
	if (ASC.IsValid())
	{
		for (auto& [Attribute, Handle] : DelegateHandleMap)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(Handle);
		}
	}
	Super::NativeDestruct();
}

void USiraAttributeUserWidget::ClearHandle()
{
	if (ASC.IsValid())
	{
		for (auto& [Attribute, Handle] : DelegateHandleMap)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(Handle);
		}
	}
	DelegateHandleMap.Empty();
}
