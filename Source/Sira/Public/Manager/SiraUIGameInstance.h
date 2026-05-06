#pragma once

#include "SiraUIGameInstance.generated.h"

class USiraUniquePanel;

// manage UniquePanel
UCLASS()
class USiraUIGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// Synchronic
	UFUNCTION(BlueprintCallable)
	USiraUniquePanel* GetOrCreateWidget(FName Key);
	
private:
	
	UPROPERTY()
	UDataTable* UniquePanelTable;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<USiraUniquePanel>> WidgetMap;

	UPROPERTY()
	TMap<FName, TSubclassOf<USiraUniquePanel>> WidgetClassMap;
};


