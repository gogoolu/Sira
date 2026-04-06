#pragma once
#include "AttributeSet.h"
#include "SiraAttributeComponent.generated.h"


struct FOnAttributeChangeData;

UCLASS(ClassGroup=(Sira), meta=(BlueprintSpawnableComponent))
class USiraAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	bool bFullScreenUI = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute", meta = (EditCondition = "bFullScreenUI", EditConditionHides))
	TSubclassOf<UUserWidget> AttributeUIClass;
	
	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetAttributeValue(const FGameplayAttribute& Attribute) const;
	
	UFUNCTION(BlueprintPure, Category = "Attribute")
	float K2_GetAttributeValue(FGameplayAttribute Attribute) const;
	
	UFUNCTION()
	virtual void Init();
	
private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void HandleOwnerAttributeChanged(const FOnAttributeChangeData& Data) const;
	
	TWeakObjectPtr<UObject> AttributeListener;
	TArray<FGameplayAttribute> ListenAttributes;
	TMap<FGameplayAttribute, FDelegateHandle> AttributeDelegateMap;
};
