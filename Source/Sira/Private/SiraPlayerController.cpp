#include "SiraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SiraLog.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "Manager/SiraUIGameInstance.h"
#include "UI/SiraUniquePanel.h"

ASiraPlayerController::ASiraPlayerController()
{
}

void ASiraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::TogglePauseGame);
	}
}

void ASiraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		// 角色操作映射
		Subsystem->AddMappingContext(GameplayMappingContext, 0);

		Subsystem->AddMappingContext(BasicGameMappingContext, 10);
	}
}

void ASiraPlayerController::TogglePauseGame()
{
	UE_LOG(LogSira, Log, TEXT("[ASiraPlayerController::TogglePauseGame] Toggle Game Pause to %s"), !IsPaused() ? TEXT("true") : TEXT("false"));
	if (!IsPaused())
	{
		if (auto Widget = GetGameInstance()->GetSubsystem<USiraUIGameSubsystem>()->GetOrCreateWidget("WBP_Pause"))
		{
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
		USiraBlueprintLibrary::SimplyShowMouse(GetWorld(), true);
		SetPause(true);
	} else
	{
		if (auto Widget = GetGameInstance()->GetSubsystem<USiraUIGameSubsystem>()->GetOrCreateWidget("WBP_Pause"))
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
		USiraBlueprintLibrary::SimplyShowMouse(GetWorld(), false);
		SetPause(false);
	}
}