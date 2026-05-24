#include "SiraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SiraLog.h"
#include "DataTable/SiraDataTable.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "Manager/SiraConfigSubsystem.h"
#include "UI/SiraUniquePanel.h"

ASiraPlayerController::ASiraPlayerController()
{
	// PrimaryActorTick.bCanEverTick = false;
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
		if (auto Widget = GetOrCreateWidget("WBP_Pause"))
		{
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Visible);
			if (auto Function = Widget->FindFunction("UpdateAttribute"))
			{
				Widget->ProcessEvent(Function, nullptr);
			}
		}
		USiraBlueprintLibrary::SimplyShowMouse(GetWorld(), true);
		SetPause(true);
	} else
	{
		if (auto Widget = GetOrCreateWidget("WBP_Pause"))
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
		USiraBlueprintLibrary::SimplyShowMouse(GetWorld(), false);
		SetPause(false);
	}
}

USiraUniquePanel* ASiraPlayerController::GetOrCreateWidget(FName Key)
{
	auto Config = GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	TSubclassOf<USiraUniquePanel> WidgetClass;
	if (TSubclassOf<USiraUniquePanel>* FoundClass = WidgetClassMap.Find(Key))
	{
		WidgetClass = *FoundClass;
	} else
	{
		auto Row = Config->GetUniquePanel(Key);
		if (!Row)
		{
			UE_LOG(LogSira, Error, TEXT("[ASiraPlayerController::GetOrCreateWidget] Widget Not Found, Key: %s"), *Key.ToString());
			return nullptr;
		}
		WidgetClass = Row->UniquePanelSoftClass.LoadSynchronous();
		if (!WidgetClass)
		{
			UE_LOG(LogSira, Error, TEXT("[ASiraPlayerController::GetOrCreateWidget] Widget Class Load Failed, Key: %s"), *Key.ToString());
			return nullptr;
		}
		WidgetClassMap.Add(Key, WidgetClass);
	}
	
	if (auto FoundWidget = WidgetMap.Find(Key))
	{
		return *FoundWidget;
	}
	
	USiraUniquePanel* NewWidget = CreateWidget<USiraUniquePanel>(USiraBlueprintLibrary::GetSiraPlayerController(GetWorld()), WidgetClass);
	if (!NewWidget)
	{
		UE_LOG(LogSira, Warning, TEXT("[ASiraPlayerController::GetOrCreateWidget] Create Widget Failed, Key: %s"), *Key.ToString());
		return nullptr;
	}
	WidgetMap.Add(Key, NewWidget);
	NewWidget->SetUIKey(Key);
	return NewWidget;
}
