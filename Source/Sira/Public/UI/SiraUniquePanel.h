#pragma once

#include "Blueprint/UserWidget.h"


#include "SiraUniquePanel.generated.h"


UCLASS()
class USiraUniquePanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SiraUI")
	FName GetUIKey() const;
	
	UFUNCTION(BlueprintCallable, Category = "SiraUI")
	void SetUIKey(FName Key);
	
private:
	FName UIKey;
};
