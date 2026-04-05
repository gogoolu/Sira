#pragma once
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "SiraAttributeUserWidget.generated.h"

struct FOnAttributeChangeData;

UCLASS()
class USiraAttributeUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSet")
	TArray<FGameplayAttribute> ListenAttributes;

	void Init(UAbilitySystemComponent* InASC);
	
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AttributeSet")
	void HandleAttributeChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);
	
private:
	virtual void NativeDestruct() override;
	
	void ClearHandle();
	
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	
	TMap<FGameplayAttribute, FDelegateHandle> DelegateHandleMap;
};
