#pragma once
#include "NormalEnemy.h"

#include "SiraEnemyPool.generated.h"

class ANormalEnemy;

USTRUCT()
struct FEnemyGroup
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TObjectPtr<ANormalEnemy>> Enemies;
};

UCLASS()
class USiraEnemyPool : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	int32 GetCurrentEnemiesNum() const;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	ANormalEnemy* SpawnEnemy(const TSubclassOf<ANormalEnemy> EnemyClass, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	
	ANormalEnemy* SpawnEnemySync(FName EnemyID, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	
	void SpawnEnemyAsync(FName EnemyID, const FVector& SpawnLocation, const FRotator& SpawnRotation, 
		TFunction<void(ANormalEnemy*)> Callback);

	void RecycleEnemy(ANormalEnemy* Enemy);

private:
	
	UPROPERTY()
	TMap<int32, FEnemyGroup> Pool;
	
	int32 EnemiesNum = 0;
};
