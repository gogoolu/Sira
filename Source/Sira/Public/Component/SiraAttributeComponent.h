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
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	TSubclassOf<UUserWidget> AttributeUIClass;
	
private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void Init();
	
	void HandleOwnerAttributeChanged(const FOnAttributeChangeData& Data) const;
	
	TWeakObjectPtr<UUserWidget> AttributeWidget;
	TArray<FGameplayAttribute> ListenAttributes;
	TMap<FGameplayAttribute, FDelegateHandle> AttributeDelegateMap;
};
