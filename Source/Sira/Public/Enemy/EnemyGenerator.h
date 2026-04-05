#pragma once

#include "CoreMinimal.h"
#include "EnemyGenerator.generated.h"

class ANormalEnemy;

UCLASS()
class AEnemyGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="EnemyGenerator")
	int32 MaxEnemyNum = 5;
	
	UPROPERTY(EditDefaultsOnly, Category="EnemyGenerator")
	float Interval = 2;
	
	UPROPERTY(EditDefaultsOnly, Category="EnemyGenerator")
	TSubclassOf<ANormalEnemy> EnemyClass;
	
	UPROPERTY(EditDefaultsOnly, Category="EnemyGenerator")
	float SpawnRadius = 10;
	
	UFUNCTION(BlueprintCallable, Category="EnemyGenerator")
	void StartGenerate();
	
	UFUNCTION(BlueprintCallable, Category="EnemyGenerator")
	void StopGenerate();
	
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void GenerateEnemies();
	
	FTimerHandle Handle;
	
};