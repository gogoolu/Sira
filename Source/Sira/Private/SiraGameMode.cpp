// Fill out your copyright notice in the Description page of Project Settings.


#include "SiraGameMode.h"


#include <random>
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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
	USiraBlueprintLibrary::SimplyShowMouse(GetWorld(), true);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ASiraGameMode::K2_ShowDamageNumber(float Damage, const FVector SpawnLocation, int32 DamageLevel) const
{
	ShowDamageNumber(Damage, SpawnLocation, DamageLevel);
}

void ASiraGameMode::ShowDamageNumber(float Damage, const FVector& SpawnLocation, int32 DamageLevel) const
{
	if (!DamageNumberNS)
	{
		UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::ShowDamageNumber] DamageNumberNS is NULL"));
		return;
	}

	float RightOffset = FMath::FRandRange(-100.f, 100.f);
	float UpOffset = FMath::FRandRange(-100.0f, 100.0f);
	
	auto CameraManger = USiraBlueprintLibrary::GetCameraManger(GetWorld());
	if (!CameraManger)
	{
		return;
	}
	auto CameraRotation = CameraManger->GetCameraRotation();
	auto RotationMatrix = FRotationMatrix(CameraRotation);
	
	auto CameraRightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);
	auto CameraUpDirection = RotationMatrix.GetUnitAxis(EAxis::Z);

	int32 DamageInteger = FMath::RoundToInt(Damage);
	int32 Index = 0;
	while (DamageInteger > 0)
	{
		constexpr float DamageNumberSpacing = 30.f;
		FVector FinalSpawnLocation = SpawnLocation - CameraRightDirection * DamageNumberSpacing * Index 
			+ CameraRightDirection * RightOffset
			+ CameraUpDirection * UpOffset;
	
		int32 RightDecimalDigit = DamageInteger % 10;
	
		auto DamageNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageNumberNS, FinalSpawnLocation);
		DamageNiagaraComp->SetFloatParameter(TEXT("DamageDigit"), RightDecimalDigit);
		if (DamageNumberColors.Num() > DamageLevel)
		{
			DamageNiagaraComp->SetColorParameter(TEXT("DigitalColor"), DamageNumberColors[DamageLevel]);
		} else
		{
			UE_LOG(LogSira, Warning, TEXT("[ASiraPlayer::ShowDamageNumber] Damage Level has no match Color, DamageLevel: %d"), DamageLevel);
		}

		DamageInteger /= 10;
		Index++;
	}
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
