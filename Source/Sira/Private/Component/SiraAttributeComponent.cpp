#include "Component/SiraAttributeComponent.h"

// #include "AbilitySystemComponent.h"
// #include "AbilitySystemInterface.h"
// #include "Blueprint/UserWidget.h"
// #include "Interface/AttributeListenable.h"
// #include "SiraLog.h"


// float USiraAttributeComponent::GetAttributeValue(const FGameplayAttribute& Attribute) const
// {
// 	if (auto Owner = Cast<IAbilitySystemInterface>(GetOwner()))
// 	{
// 		if (auto ASC = Owner->GetAbilitySystemComponent())
// 		{
// 			return ASC->GetNumericAttribute(Attribute);
// 		}
// 	}
// 	UE_LOG(LogSira, Warning, TEXT("[USiraAttributeComponent::GetAttributeValue] Owner ASC is null, Owner: [%s]"), *GetOwner()->GetName())
// 	return 0.f;
// }
//
// float USiraAttributeComponent::K2_GetAttributeValue(FGameplayAttribute Attribute) const
// {
// 	return GetAttributeValue(Attribute);
// }
//
// void USiraAttributeComponent::BeginPlay()
// {
// 	Super::BeginPlay();
// 	// Init();
// }
//
// void USiraAttributeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
// {
// 	Super::EndPlay(EndPlayReason);
// 	if (auto Owner = Cast<IAbilitySystemInterface>(GetOwner()))
// 	{
// 		if (auto ASC = Owner->GetAbilitySystemComponent())
// 		{
// 			for (auto& [Attribute, Handle] : AttributeDelegateMap)
// 			{
// 				ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(Handle);
// 			}
// 		}
// 	}
// 	
// 	// destroy widget, only destroy ui that add to viewport
// 	if (AttributeListener.IsValid() && bFullScreenUI)
// 	{
// 		if (auto Widget = Cast<UUserWidget>(AttributeListener.Get()))
// 		{
// 			Widget->SetVisibility(ESlateVisibility::Collapsed);
// 			Widget->RemoveFromParent();
// 		}
// 	}
// }
//
// void USiraAttributeComponent::Init()
// {
// 	if (auto AbilityOwner = Cast<IAbilitySystemInterface>(GetOwner()))
// 	{
// 		if (auto ASC = AbilityOwner->GetAbilitySystemComponent())
// 		{
// 			if (bFullScreenUI && AttributeUIClass)
// 			{
// 				// boss血条, 使用全屏UI实现
// 				if (UUserWidget* Widget = CreateWidget(GetWorld(), AttributeUIClass))
// 				{
// 					Widget->AddToViewport();
// 					// 如果 UI 实现了 AttributeListenable 接口，则绑定对应的 Attribute 改变事件
// 					if (Widget->Implements<UAttributeListenable>())
// 					{
// 						AttributeListener = Widget;
// 						IAttributeListenable::Execute_GetListenAttributes(Widget, ListenAttributes);
// 					}
// 				}
// 			} else
// 			{
// 				// 小怪血条，使用 MaterialBillboardComponent 实现
// 				if (auto OwnerActor = Cast<AActor>(GetOwner()))
// 				{
// 					if (OwnerActor->Implements<UAttributeListenable>())
// 					{
// 						AttributeListener = OwnerActor;
// 						IAttributeListenable::Execute_GetListenAttributes(OwnerActor, ListenAttributes);
// 					}
// 				}
// 			}
// 			
// 			for (auto& Attribute : ListenAttributes)
// 			{
// 				if (Attribute.IsValid())
// 				{
// 					auto Handle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &USiraAttributeComponent::HandleOwnerAttributeChanged);
// 					AttributeDelegateMap.Add(Attribute, Handle);
// 					
// 					// manually trigger once
// 					float Value = ASC->GetNumericAttribute(Attribute);
// 					IAttributeListenable::Execute_OnListenAttributeChanged(AttributeListener.Get(), Attribute, Value, Value);
// 				}
// 			}
// 		}
// 	}
// }
//
// void USiraAttributeComponent::ManuallyTrigger()
// {
// 	if (auto AbilityOwner = Cast<IAbilitySystemInterface>(GetOwner()))
// 	{
// 		if (auto ASC = AbilityOwner->GetAbilitySystemComponent())
// 		{
// 			for (auto& Attribute : ListenAttributes)
// 			{
// 				if (Attribute.IsValid())
// 				{
// 					float Value = ASC->GetNumericAttribute(Attribute);
// 					IAttributeListenable::Execute_OnListenAttributeChanged(AttributeListener.Get(), Attribute, Value, Value);
// 				}
// 			}
// 		}
// 	}
// }
//
// void USiraAttributeComponent::HandleOwnerAttributeChanged(const FOnAttributeChangeData& Data) const
// {
// 	if (AttributeListener.IsValid())
// 	{
// 		IAttributeListenable::Execute_OnListenAttributeChanged(AttributeListener.Get(), Data.Attribute, Data.NewValue, Data.OldValue);
// 	}
// }
