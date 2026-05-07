// Fill out your copyright notice in the Description page of Project Settings.


#include "SiraGameMode.h"


#include <random>
#include "NavigationSystem.h"
#include "SiraLog.h"
#include "Blueprint/UserWidget.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "Manager/TerrainGenerator.h"
#include "DataTable/SiraDataTable.h"
#include "Enemy/NormalEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/SiraUIGameInstance.h"
#include "UI/SiraUniquePanel.h"


void ASiraGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (auto GameInstance = GetGameInstance())
	{
		if (auto UISubsystem = GameInstance->GetSubsystem<USiraUIGameSubsystem>())
		{
			if (auto Widget = UISubsystem->GetOrCreateWidget("WBP_HUD_Main"))
			{
				Widget->AddToViewport();
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	StartNewGameLevel();
}

void ASiraGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StopGenerateEnemies();
}

void ASiraGameMode::HandleLevelOver()
{
	if (auto GameInstance = GetGameInstance())
	{
		if (auto UISubsystem = GameInstance->GetSubsystem<USiraUIGameSubsystem>())
		{
			if (auto Widget = UISubsystem->GetOrCreateWidget("WBP_Store"))
			{
				Widget->AddToViewport();
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	
	if (auto PC = USiraBlueprintLibrary::GetSiraPlayerController(GetWorld()))
	{
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetInputMode(InputMode);
	}
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

// when old game level finished trigger the function
void ASiraGameMode::StartNewGameLevel()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	// remove all enemies
	SiraUtilities::CallForAllActors<ANormalEnemy>(GetWorld(), [](ANormalEnemy* Enemy)
	{
		Enemy->Die();
	});
	
	if (!TerrainGenerator.IsValid())
	{
		TerrainGenerator = GetWorld()->SpawnActor<ATerrainGenerator>(TerrainGeneratorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
	if (TerrainGenerator.IsValid())
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int32> dis(0, MAX_int32);
		int32 BaseSeed = dis(gen);
		int32 Seed = dis(gen);
		TerrainGenerator->GenerateTerrainWithSeed(BaseSeed, Seed);
		
		ResetPlayerToPlayerStart(USiraBlueprintLibrary::GetSiraPlayerController(GetWorld()));
	}
	
	if (auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}
	
	// delay to gen enemy
	GetWorld()->GetTimerManager().ClearTimer(EnemyGenHandle);
	GetWorld()->GetTimerManager().SetTimer(
		EnemyGenHandle,
		this,
		&ASiraGameMode::StartGenerateEnemies,
		FirstGenEnemyDelay,
		false
	);
	
	// delay to next level
	GetWorld()->GetTimerManager().ClearTimer(LevelChangeHandle);
	GetWorld()->GetTimerManager().SetTimer(
		LevelChangeHandle,
		this,
		&ASiraGameMode::HandleLevelOver,
		LevelChangeTime,
		false
	);
	
	LevelStartTime = GetWorld()->GetTimeSeconds();
}

void ASiraGameMode::ResetPlayerToPlayerStart(ASiraPlayerController* PC)
{
	if (PC == nullptr || PC->IsPendingKillPending())
	{
		return;
	}
	
	// if no pawn 
	auto Player = Cast<ASiraPlayer>(PC->GetPawn());
	if (!IsValid(Player))
	{
		RestartPlayer(PC);
		return;
	}

	AActor* StartSpot = FindPlayerStart(PC);
	if (!StartSpot)
	{
		UE_LOG(LogSira, Warning, TEXT("[ASiraGameMode::ResetPlayerToPlayerStart] Do not find PlayerStart"));
		return;
	}
	
	Player->SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
	if (auto CharMovement = Player->GetCharacterMovement())
	{
		CharMovement->Velocity = FVector::ZeroVector;
	}
	PC->SetControlRotation(StartSpot->GetActorRotation());
}

double ASiraGameMode::GetLevelElapsedTime() const
{
	return GetWorld()->GetTimeSeconds() - LevelStartTime;
}

double ASiraGameMode::GetLevelRemainTime() const
{
	return LevelChangeTime - GetLevelElapsedTime();
}

ATerrainGenerator* ASiraGameMode::GetTerrainGenerator() const
{
	return TerrainGenerator.Get();
}

FLevelInfoRow* ASiraGameMode::GetLevelInfoRow() const
{
	FString RowName = FString::FromInt(CurrentLevel);
	if (auto Row = LevelInfoDataTable->FindRow<FLevelInfoRow>(FName(*RowName), TEXT("")))
	{
		return Row;
	} 
	UE_LOG(LogSira, Warning, TEXT("[ASiraGameMode::GetLevelInfoRow] Row not exist, RowName: %s"), *RowName)
	return nullptr;
}

void ASiraGameMode::StartGenerateEnemies()
{
	StopGenerateEnemies();
	GetWorld()->GetTimerManager().SetTimer(
		EnemyGenHandle,
		this,
		&ASiraGameMode::GenerateEnemies,
		EnemyGenInterval > 0.5f ? EnemyGenInterval : 0.5f,
		true
	);
}

void ASiraGameMode::StopGenerateEnemies()
{
	GetWorld()->GetTimerManager().ClearTimer(EnemyGenHandle);
}


void ASiraGameMode::GenerateEnemies()
{
	if (auto Player = USiraBlueprintLibrary::GetSiraPlayer(GetWorld()))
	{
		if (USiraBlueprintLibrary::GetActorsCount<ANormalEnemy>(GetWorld()) >= MaxEnemyNum)
		{
			return;
		}
		
		auto GetRandomSpawnLocation = [MinRadius = EnemyGenMinRadius, MaxRadius = EnemyGenMaxRadius]() -> FVector2D
		{
			float RandomDistance = FMath::FRandRange(MinRadius, MaxRadius);
			float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);
			return {FMath::Cos(RandomAngle) * RandomDistance, FMath::Sin(RandomAngle) * RandomDistance};
		};
		
		auto PlayerLocation = Player->GetActorLocation();
		auto RandomOffset = GetRandomSpawnLocation();
		FVector StartTrace {PlayerLocation.X + RandomOffset.X, PlayerLocation.Y + RandomOffset.Y, 20000};
		FVector EndTrace {StartTrace.X, StartTrace.Y, -20000};
		// line trace Terrain
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1))
		{
			auto RandomEnemyClass = EnemyClasses[FMath::RandRange(0, EnemyClasses.Num() - 1)];
			float HalfHeight = 500.f;
			if (auto CDO = RandomEnemyClass->GetDefaultObject<ANormalEnemy>())
			{
				HalfHeight = CDO->GetDefaultHalfHeight();
			}
			FVector SpawnLocation {Hit.Location.X, Hit.Location.Y, Hit.Location.Z + HalfHeight};
			FRotator SpawnRotation = {0, (PlayerLocation - SpawnLocation).ToOrientationRotator().Yaw, 0};
			GetWorld()->SpawnActor<ANormalEnemy>(
				RandomEnemyClass,
				SpawnLocation,
				SpawnRotation
			);
		}
	}
}
