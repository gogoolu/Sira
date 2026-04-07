#include "HUD/SiraHUD.h"

#include "Player/SiraPlayer.h"
#include "UI/SiraAttributeUserWidget.h"

void ASiraHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (USiraAttributeUserWidget* HUDWidget = CreateWidget<USiraAttributeUserWidget>(GetWorld(), MainHUDClass))
	{
		if (auto Player = Cast<ASiraPlayer>(GetOwningPawn()))
		{
			HUDWidget->Init(Player->GetAbilitySystemComponent());
		}
		HUDWidget->AddToViewport();	
	}
}
