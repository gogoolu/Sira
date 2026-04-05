#pragma once
#include "GameFramework/HUD.h"

#include "SiraHUD.generated.h"


class USiraAttributeUserWidget;


UCLASS()
class ASiraHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	TSubclassOf<USiraAttributeUserWidget> MainHUDClass;
	
	virtual void BeginPlay() override;
};
