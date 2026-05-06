// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SiraGameMode.generated.h"

class ASiraPlayerController;
class ATerrainGenerator;
class ANormalEnemy;
struct FLevelInfoRow;

UCLASS()
class SIRA_API ASiraGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// UFUNCTION()
	// void HandlePlayerDeath();
	
	UFUNCTION(BlueprintCallable, Category = "SiraGameplay")
	void StartNewGameLevel();
	
	UFUNCTION(BlueprintCallable, Category = "SiraGameplay")
	void ResetPlayerToPlayerStart(ASiraPlayerController* PC);
	
	// unit: (s)
	UFUNCTION(BlueprintCallable)
	double GetLevelElapsedTime() const;
	
	UFUNCTION(BlueprintCallable)
	double GetLevelRemainTime() const;
	
	ATerrainGenerator* GetTerrainGenerator() const;
	
	FLevelInfoRow* GetLevelInfoRow() const;
	
	UFUNCTION(BlueprintCallable, Category = "EnemyGeneration")
	void StartGenerateEnemies();
	
	UFUNCTION(BlueprintCallable, Category = "EnemyGeneration")
	void StopGenerateEnemies();
	
	void HandleLevelOver();

private:
	void GenerateEnemies();
	FTimerHandle LevelChangeHandle;
	FTimerHandle EnemyGenHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Terrain")
	TSubclassOf<ATerrainGenerator> TerrainGeneratorClass;
	
	// Enemy Generation Start ===================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<ANormalEnemy>> EnemyClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	int32 MaxEnemyNum = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	float EnemyGenRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	float EnemyGenInterval = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	float FirstGenEnemyDelay = 4.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	float EnemyGenMinRadius = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	float EnemyGenMaxRadius = 3000.f;
	// Enemy Generation End =================================================
	
	UPROPERTY(EditDefaultsOnly, Category = "SiraGameplay")
	UDataTable* LevelInfoDataTable;
	
	UPROPERTY(BlueprintReadOnly, Category = "SiraGameplay", meta = (AllowPrivateAccess = true))
	double LevelStartTime = 0;
	
	// unit (s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiraGameplay", meta = (AllowPrivateAccess = true))
	double LevelChangeTime = 60;
	
	UPROPERTY(EditDefaultsOnly, Category = "SiraGameplay")
	int32 CurrentLevel = 1;
	

	TWeakObjectPtr<ATerrainGenerator> TerrainGenerator;
};
