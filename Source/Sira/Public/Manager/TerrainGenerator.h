#pragma once

#include "TerrainGenerator.generated.h"

class UProceduralMeshComponent;

UCLASS()
class ATerrainGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	ATerrainGenerator();
	void GenerateTerrain();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> TerrainMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Seed = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SizeX = 100;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SizeY = 100;
	
	UPROPERTY(EditDefaultsOnly)
	float ScaleX = 100.f;
	
	UPROPERTY(EditDefaultsOnly)
	float ScaleY = 100.f;
	
	UPROPERTY(EditDefaultsOnly)
	float ScaleZ = 300.f;
	
	// ===========================
	
	UPROPERTY(EditDefaultsOnly)
	float InitFrequency = 0.1f;
	
	UPROPERTY(EditDefaultsOnly)
	float InitPersistent = 0.5f;
	
	// UPROPERTY(EditDefaultsOnly)
	
	
	
	// ============================ 
	UPROPERTY(EditDefaultsOnly)
	float Frequency = 0.1f;
	
	UPROPERTY(EditDefaultsOnly)
	float Persistence = 0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	float Amplitude = 1.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float Lacunarity = 1.5f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Layer = 1;
	
	// 平原参数，低于此值判定为平原
	UPROPERTY(EditDefaultsOnly)
	float PlainHeight = 0.3f;
	
	UPROPERTY(EditDefaultsOnly)
	float PlainFactor = 1.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float HillHeight = 0.6f;
	
	UPROPERTY(EditDefaultsOnly)
	float HillFactor = 300.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MountainFactor = 500.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor VertexLinearColor = FLinearColor(1.0f, 1.0f, 1.0f);
	
private:
	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh;
	
	float Noise(int32 X, int32 Y, float Frequency) const;
	
	FRandomStream RandomStream;
	
	FVector2D SeedOffset;
};
