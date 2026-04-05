#include "Component/SiraAttributeComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Component/FaceCameraWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interface/HealthListenable.h"


void USiraAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void USiraAttributeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (auto Owner = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (auto ASC = Owner->GetAbilitySystemComponent())
		{
			for (auto& [Attribute, Handle] : AttributeDelegateMap)
			{
				ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(Handle);
			}
		}
	}
	
	// destroy widget, only destroy ui that add to viewport
	if (AttributeWidget.IsValid())
	{
		AttributeWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (bFullScreenUI)
		{
			AttributeWidget->RemoveFromParent();
		}
	}
}

void USiraAttributeComponent::Init()
{
	if (auto AbilityOwner = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (auto ASC = AbilityOwner->GetAbilitySystemComponent())
		{
			if (AttributeUIClass)
			{
				UUserWidget* Widget = nullptr;
				if (bFullScreenUI)
				{
					// 全屏UI，boss血条
					Widget = CreateWidget(GetWorld(), AttributeUIClass);
					if (Widget)
					{
						Widget->AddToViewport();
					}
				} else
				{
					// 挂到 WidgetComponent，小怪血条
					if (AActor* OwnerActor = Cast<AActor>(GetOwner()))
					{
						auto WidgetComponent = NewObject<UFaceCameraWidgetComponent>(OwnerActor);
						WidgetComponent->RegisterComponent();
						WidgetComponent->SetWidgetClass(AttributeUIClass);
						WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						Widget = WidgetComponent->GetUserWidgetObject();
						WidgetComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
						if (auto OwnerRoot = OwnerActor->GetRootComponent())
						{
							if (auto Capsule = Cast<UCapsuleComponent>(OwnerRoot))
							{
								FVector WidgetLocation = Capsule->GetComponentLocation() + FVector(0.0f, 0.0f, Capsule->GetScaledCapsuleHalfHeight());
								WidgetComponent->SetWorldLocation(WidgetLocation);
							}
						}
					}
				}
				
				// 如果实现了 AttributeListenable 接口，则绑定对应的 Attribute 改变事件
				if (Widget && Widget->Implements<UAttributeListenable>())
				{
					AttributeWidget = Widget;
					IAttributeListenable::Execute_GetListenAttributes(Widget, ListenAttributes);
					for (auto& Attribute : ListenAttributes)
					{
						if (Attribute.IsValid())
						{
							auto Handle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &USiraAttributeComponent::HandleOwnerAttributeChanged);
							AttributeDelegateMap.Add(Attribute, Handle);
							
							// manualy trigger once
							float Value = ASC->GetNumericAttribute(Attribute);
							IAttributeListenable::Execute_OnListenAttributeChanged(Widget, Attribute, Value, Value);
						}
					}
				}
			}
		}
	}
}

void USiraAttributeComponent::HandleOwnerAttributeChanged(const FOnAttributeChangeData& Data) const
{
	if (AttributeWidget.IsValid())
	{
		IAttributeListenable::Execute_OnListenAttributeChanged(AttributeWidget.Get(), Data.Attribute, Data.NewValue, Data.OldValue);
	}
}
