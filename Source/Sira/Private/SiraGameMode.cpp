// Fill out your copyright notice in the Description page of Project Settings.


#include "SiraGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Manager/SiraBlueprintLibrary.h"


void ASiraGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// if (auto SiraPlayer = USiraBlueprintLibrary::GetSiraPlayer(this))
	// {
	// 	SiraPlayer->OnPlayerDead.AddDynamic(this, &ThisClass::HandlePlayerDeath);				
	// }
}

void ASiraGameMode::HandlePlayerDeath()
{
	if (FailureWidgetClass)
	{
		if (auto Widget = CreateWidget(GetWorld(), FailureWidgetClass))
		{
			Widget->AddToViewport();
		}
	}
}

void ASiraGameMode::HandlePlayerWin()
{
	if (SuccessWidgetClass)
	{
		if (auto Widget = CreateWidget(GetWorld(), SuccessWidgetClass))
		{
			Widget->AddToViewport();
		}
	}
}
