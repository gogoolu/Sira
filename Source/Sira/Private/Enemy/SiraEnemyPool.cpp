#include "Enemy/SiraEnemyPool.h"

#include "SiraLog.h"
#include "DataTable/SiraDataTable.h"
#include "Enemy/NormalEnemy.h"
#include "Manager/SiraConfigSubsystem.h"

int32 USiraEnemyPool::GetCurrentEnemiesNum() const
{
	return EnemiesNum;
}

void USiraEnemyPool::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USiraEnemyPool::Deinitialize()
{
	Super::Deinitialize();
	Pool.Empty();
}

ANormalEnemy* USiraEnemyPool::SpawnEnemy(const TSubclassOf<ANormalEnemy> EnemyClass, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!EnemyClass)
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraEnemyPool::SpawnEnemy] EnemyClass is nullptr"))
		return nullptr;
	}
	int32 ItemID = EnemyClass->GetDefaultObject<ANormalEnemy>()->GetEnemyID();

	for (auto Enemy : Pool.FindOrAdd(ItemID).Enemies)
	{
		if (Enemy && Enemy->IsHidden())
		{
			Enemy->SetActorHiddenInGame(false);
			Enemy->SetActorEnableCollision(true);
			Enemy->SetActorLocation(SpawnLocation);
			Enemy->SetActorRotation(SpawnRotation);
			Enemy->InitEnemy();
			EnemiesNum++;
			return Enemy;
		}
	}
	ANormalEnemy* NewEnemy = GetWorld()->SpawnActor<ANormalEnemy>(EnemyClass, SpawnLocation, SpawnRotation);
	if (NewEnemy)
	{
		Pool.FindOrAdd(ItemID).Enemies.Add(NewEnemy);
		EnemiesNum++;
	}
	
	return NewEnemy;
}

ANormalEnemy* USiraEnemyPool::SpawnEnemySync(FName EnemyID, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	auto SiraConfig = GetWorld()->GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	auto EnemyConfig = SiraConfig->GetEnemyConfig(EnemyID);
	if (!EnemyConfig)
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraEnemyPool::SpawnEnemySync] Enemy Config not exist, EnemyID: %s"), *EnemyID.ToString());
		return nullptr;
	}
	auto EnemyClass = EnemyConfig->EnemyClass.LoadSynchronous();
	return SpawnEnemy(EnemyClass, SpawnLocation, SpawnRotation);
}

void USiraEnemyPool::SpawnEnemyAsync(FName EnemyID, const FVector& SpawnLocation, const FRotator& SpawnRotation,
	TFunction<void(ANormalEnemy*)> Callback)
{
	auto SiraConfig = GetWorld()->GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	auto EnemyConfig = SiraConfig->GetEnemyConfig(EnemyID);
	if (!EnemyConfig)
	{
		UE_LOG(LogSira, Warning, TEXT("[USiraEnemyPool::SpawnEnemyAsync] Enemy Config not exist, EnemyID: %s"), *EnemyID.ToString());
		return;
	}
	EnemyConfig->EnemyClass.LoadAsync(
	FLoadSoftObjectPathAsyncDelegate::CreateWeakLambda(this, 
	[this,
		SpawnLocation,
		SpawnRotation,
		Callback = MoveTemp(Callback)
	]
	// (const FSoftObjectPath& Path, const TSubclassOf<ANormalEnemy> EnemyClass)
	(const FSoftObjectPath& Path, UObject* EnemyClass)
	{
		UClass* Class = Cast<UClass>(EnemyClass);
		if (auto Enemy = SpawnEnemy(Class, SpawnLocation, SpawnRotation))
		{
			Callback(Enemy);
		}
	}));
	
}

void USiraEnemyPool::RecycleEnemy(ANormalEnemy* Enemy)
{
	if (Enemy && !Enemy->IsHidden())
	{
		Enemy->ResetGASState();
		Enemy->SetActorHiddenInGame(true);
		Enemy->SetActorEnableCollision(false);
		EnemiesNum--;
		// if (!Pool[Enemy->GetEnemyID()].Enemies.Contains(Enemy))
		// {
		// 	Pool[Enemy->GetEnemyID()].Enemies.Add(Enemy);
		// }
		//
		if (!Pool.FindOrAdd(Enemy->GetEnemyID()).Enemies.Contains(Enemy))
		{
			Pool[Enemy->GetEnemyID()].Enemies.Add(Enemy);
		}
	}
}
