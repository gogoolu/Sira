// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SiraGameMode.generated.h"

class ADropItem;
class ASiraPlayerController;
class ATerrainGenerator;
class ANormalEnemy;
class UNiagaraSystem;

struct FLevelInfoRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelStageChange, int32, CurrentLevel);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelEnd);

UCLASS()
class SIRA_API ASiraGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	ASiraGameMode();
	
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
	
	// FLevelInfoRow* GetLevelInfoRow() const;
	
	UFUNCTION(BlueprintCallable, Category = "EnemyGeneration")
	void StartGenerateEnemies();
	
	UFUNCTION(BlueprintCallable, Category = "EnemyGeneration")
	void StopGenerateEnemies();
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetCurrentLevel() const;
	
	// Level Stage ============
	UFUNCTION()
	void HandlePlayerDead();
	
	void HandleLevelCountDownOver() const;
	
	void StopLevelTimer();

	UFUNCTION()
	void HandleGameFailed(int32 InCurrentLevel);
	
	UFUNCTION(BlueprintCallable)
	void HandleGameSuccess(int32 InCurrentLevel);
	
	
	// Utilities ===========
	UFUNCTION(BlueprintCallable)
	void K2_ShowDamageNumber(float Damage, const FVector SpawnLocation, int32 DamageLevel = 0) const;
	
	void ShowDamageNumber(float Damage, const FVector& SpawnLocation, int32 DamageLevel = 0) const;
	
	// Global Events =============
	UPROPERTY(BlueprintAssignable)
	FOnLevelStageChange OnLevelStart;
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelStageChange OnLevelEnd;
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelStageChange OnLevelFailed;
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelStageChange OnLevelSuccuss;
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelStageChange OnGameSuccess;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<ADropItem> DropItemClass;

private:
	
	void GenerateEnemies();
	// 根据配置以及当前关卡数，获取敌人
	int32 GetRandomEnemy() const;
	
	
	FTimerHandle LevelChangeHandle;
	FTimerHandle EnemyGenHandle;
	int32 MaxLevel;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Terrain")
	TSubclassOf<ATerrainGenerator> TerrainGeneratorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyGeneration", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<ANormalEnemy>> EnemyClasses;
	
	// Enemy Generation Start ===================================================
	
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
	
	// UPROPERTY(EditDefaultsOnly, Category = "SiraGameplay")
	// UDataTable* LevelInfoDataTable;
	
	UPROPERTY(BlueprintReadOnly, Category = "SiraGameplay", meta = (AllowPrivateAccess = true))
	double LevelStartTime = 0;
	
	// unit (s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiraGameplay", meta = (AllowPrivateAccess = true))
	double LevelChangeTime = 60;
	
	// start with 1
	UPROPERTY(Transient)
	int32 CurrentLevel = 0;
	

	TWeakObjectPtr<ATerrainGenerator> TerrainGenerator;
	
	UPROPERTY(EditDefaultsOnly, Category = "SiraGameplay")
	TObjectPtr<UNiagaraSystem> DamageNumberNS;
	
	UPROPERTY(EditDefaultsOnly, Category = "SiraGameplay")
	TArray<FLinearColor> DamageNumberColors;
	
};
