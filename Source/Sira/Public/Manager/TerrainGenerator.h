#pragma once

#include "TerrainGenerator.generated.h"

class UProceduralMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;

enum class BaseTerrainType :uint8
{
	Plain,
	Mountain,	
};

USTRUCT(BlueprintType)
struct FTerrainMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	int32 Weight = 1;
};


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
	TObjectPtr<UMaterialInterface> BorderMaterial;
	
	UPROPERTY()
	TMap<UStaticMesh*, TWeakObjectPtr<UHierarchicalInstancedStaticMeshComponent>> GrassHISMMap; 
	
	UPROPERTY()
	TMap<UStaticMesh*, TWeakObjectPtr<UHierarchicalInstancedStaticMeshComponent>> TreeHISMMap; 
	
	
	// Base
	UPROPERTY(EditDefaultsOnly, Category="Base")
	bool bShowBaseTerrain = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	bool bUseBaseTerrain = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	int32 BaseSeed = 0;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	float BaseFreq = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	float BasePlainHeight = 0.5f;
	
	
	// Detail
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	int32 Seed = 0;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	int32 SizeX = 100;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	int32 SizeY = 100;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float ScaleX = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float ScaleY = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float ScaleZ = 300.f;
	
	// ============================ 
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float Frequency = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float Persistence = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float Amplitude = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float Lacunarity = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	int32 Layer = 3;
	
	UPROPERTY(EditDefaultsOnly, meta=(ToolTip="海拔高度的修正因数，与高度相乘后在做幂运算"), Category="Detail")
	float ExpFudgeFactor = 1.2f;
	
	UPROPERTY(EditDefaultsOnly, meta=(ToolTip="海拔高度的指数，高值将中高点推向山谷，低值将中高点拉向山峰"), Category="Detail")
	float Exp = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float PlainFactor = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float PlainHeight = 0.2;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float HillFactor = 500.0f;
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float HillHeight = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Detail")
	float MountainFactor = 1000.0f;
	
	// Foliage =============
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	bool bEnableGrass = true;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	TArray<FTerrainMeshInfo> GrassMeshes;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float GrassRadius = 10.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float GrassMaxSlope = 30.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float GrassStartCullDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float GrassEndCullDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	bool bEnableTree = true;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	TArray<FTerrainMeshInfo> TreeMeshes;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float TreeRadius = 100.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float TreeMaxSlope = 40.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float TreeStartCullDistance = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category="Foliage")
	float TreeEndCullDistance = 10000.f;
	
	// from 0 to 1, support seed
	static float Noise(int32 X, int32 Y, float Frequency, const FVector2D& Offset);
	// float RidgeNoise(int32 X, int32 Y, float Frequency, int32 InSeed = 0) const;
	
private:
	int32 GrassTotalWeight = 0;
	int32 TreeTotalWeight = 0;
	
	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh;
	
	
	// 低频噪声构建基本大陆轮廓
	void CalculateBaseVertices(TArray<BaseTerrainType>& BaseTerrainVertices) const;
	void CalculateVertices(const TArray<BaseTerrainType>& BaseTerrainVertices, TArray<FVector>& Vertices, TArray<FVector2D>& UV) const;
	void CalculateTriangles(TArray<int32>& Triangles) const;
	// 用以存储每个点的坡度，材质使用
	void CalculateLinearColors(const TArray<FVector>& Normals, TArray<FLinearColor>& Colors) const;
	
	void GenerateFoliage();
	void GenerateBorder();
	void SpawnGrass(const FVector& Pos);
	void SpawnTree(const FVector& Pos);
	
	UStaticMesh* GetRandomMesh(const TArray<FTerrainMeshInfo>& MeshInfos, int32 TotalWeight) const;
	
	FRandomStream RandomStream;
};
