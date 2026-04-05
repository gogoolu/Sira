#include "Enemy/EnemyGenerator.h"

#include "Enemy/NormalEnemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AEnemyGenerator::StartGenerate()
{
	StopGenerate();
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		this,
		&AEnemyGenerator::GenerateEnemies,
		10.0f,
		true
	);
}

void AEnemyGenerator::StopGenerate()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
}

void AEnemyGenerator::BeginPlay()
{
	Super::BeginPlay();
	StartGenerate();
}

void AEnemyGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyGenerator::GenerateEnemies()
{
	if (auto Player = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		TArray<AActor*> Enemies;
		UGameplayStatics::GetAllActorsOfClass(this, EnemyClass, Enemies);
		if (Enemies.Num() >= MaxEnemyNum)
		{
			return;
		}
		
		auto PlayerLocation = Player->GetActorLocation();
		GetWorld()->SpawnActor<ANormalEnemy>(
			EnemyClass,
			PlayerLocation + FVector(FMath::RandRange(50, 200), FMath::RandRange(50, 200), 5.0f),
			FRotator::ZeroRotator
		);
		
		// if (auto NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		// {
		// 	FNavLocation NavLocation;
		// 	auto PlayerLocation = Player->GetActorLocation();
		// 	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		// 		PlayerLocation,
		// 		SpawnRadius,
		// 		NavLocation
		// 	);
		// 	if (bFound)
		// 	{
		// 		GetWorld()->SpawnActor<ANormalEnemy>(
		// 			EnemyClass,
		// 			NavLocation.Location,
		// 			FRotator::ZeroRotator
		// 		);
		// 	}
		// }
	}
}
