#include "Manager/TerrainGenerator.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "ProceduralMeshComponent/Public/KismetProceduralMeshLibrary.h"
#include "PoissonPoints.h"
#include "SiraLog.h"
#include "AI/NavigationSystemBase.h"


ATerrainGenerator::ATerrainGenerator()
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->SetCollisionProfileName("Land");
	SetRootComponent(ProceduralMesh);
}

// [-1, 1] to [0, 1]
float ATerrainGenerator::Noise(int32 X, int32 Y, float InFrequency, const FVector2D& Offset)
{
	return FMath::PerlinNoise2D({X * InFrequency + Offset.X,Y * InFrequency + Offset.Y}) / 2.0f + 0.5f;
}

void ATerrainGenerator::SetSeed(int32 InSeed)
{
	RandomStream = FRandomStream(Seed);
	Seed = InSeed;
}

void ATerrainGenerator::SetBaseSeed(int32 InSeed)
{
	BaseSeed = InSeed;
}

// float ATerrainGenerator::RidgeNoise(int32 X, int32 Y, float InFrequency, int32 InSeed) const
// {
// 	// [-1, 1] to [0, 1]
// 	FRandomStream Random {InSeed};
// 	float NoiseValue = FMath::PerlinNoise2D(
// 		FVector2D{X * InFrequency + Random.RandRange(0, 200000)
// 		,Y * InFrequency + Random.RandRange(0, 200000)}
// 	) / 2.0f + 0.5f;
// 	
// 	return 1.0f - FMath::Abs(NoiseValue * 2.0f - 1.0f);
// }

void ATerrainGenerator::CalculateBaseVertices(TArray<BaseTerrainType>& BaseTerrainVertices) const
{
	BaseTerrainVertices.SetNumUninitialized(SizeX * SizeY);
	FRandomStream Random {BaseSeed};
	FVector2D Offset(Random.FRandRange(0, 20000), Random.FRandRange(0, 20000));
	for (int32 x = 0; x < SizeX; x++)
	{
		for (int32 y = 0; y < SizeY; y++)
		{
			float Height = Noise(x, y, BaseFreq, Offset);
			if (Height < BasePlainHeight)
			{
				BaseTerrainVertices[x * SizeY + y] = BaseTerrainType::Plain;
			} else
			{
				BaseTerrainVertices[x * SizeY + y] = BaseTerrainType::Mountain;
			}
		}
	}
}

void ATerrainGenerator::GenerateTerrain()
{
	ProceduralMesh->ClearAllMeshSections();
	ClearFoliage();
	TArray<BaseTerrainType> BaseTerrainVertices;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;
	if (bUseBaseTerrain || bShowBaseTerrain)
	{
		CalculateBaseVertices(BaseTerrainVertices);
	}
	if (bShowBaseTerrain)
	{
		Vertices.Reserve(BaseTerrainVertices.Num());
		for (int32 x = 0; x < SizeX; x++)
		{
			for (int32 y = 0; y < SizeY; y++)
			{
				float Height = BaseTerrainVertices[x * SizeY + y] == BaseTerrainType::Plain ? 0 : 100;
				Vertices.Add({x * ScaleX, y * ScaleY, Height});
			}
		}
	} else
	{
		CalculateVertices(BaseTerrainVertices, Vertices, UV);
	}
	
	CalculateTriangles(Triangles);
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		Vertices,
		Triangles,
		UV,
		Normals,
		Tangents
	);
	
	CalculateLinearColors(Normals, VertexColors);
	ProceduralMesh->CreateMeshSection_LinearColor(
		0,
		Vertices,
		Triangles,
		Normals,
		UV,
		VertexColors,
		Tangents,
		true
	);
	GenerateFoliage();
	GenerateBorder();
	FNavigationSystem::UpdateComponentData(*ProceduralMesh);
	UE_LOG(LogSira, Log, TEXT("[ATerrainGenerator::GenerateTerrain] Generate Terrain Finished, BaseSeed: %d, Seed: %d"), BaseSeed, Seed);
}

void ATerrainGenerator::GenerateTerrainWithSeed(int32 InBaseSeed, int32 InSeed)
{
	SetBaseSeed(InBaseSeed);
	SetSeed(InSeed);
	GenerateTerrain();
}

void ATerrainGenerator::CalculateVertices(const TArray<BaseTerrainType>& BaseTerrainVertices, TArray<FVector>& Vertices, TArray<FVector2D>& UV) const
{
	Vertices.Empty();
	Vertices.SetNumUninitialized(SizeX * SizeY);
	UV.Empty();
	UV.SetNumUninitialized(SizeX * SizeY);
	TArray<FVector2D> LayerOffsets;
	LayerOffsets.Reserve(Layer);
	for (int i = 0; i < Layer; i++)
	{
		LayerOffsets.Add({RandomStream.FRandRange(0, 20000), RandomStream.FRandRange(0, 20000)});
	}
	
	ParallelFor(SizeX, [&](int32 x)
	{
		for (int32 y = 0; y < SizeY; y++)
		{
			// Calculate UV
			float U = StaticCast<float>(x) / StaticCast<float>(SizeX - 1);
			float V = StaticCast<float>(y) / StaticCast<float>(SizeY - 1);
			UV[x * SizeY + y] = {U, V};

			
			float Height = 0;
			float LocalAmplitude = Amplitude;
			float LocalFrequency = Frequency;
			float AmplitudeAccumulation = 0;
			
			// 叠加
			for (int i = 0; i < Layer; i++)
			{
				Height += Noise(x, y, LocalFrequency, LayerOffsets[i]) * LocalAmplitude;
				AmplitudeAccumulation += LocalAmplitude;
				LocalAmplitude *= Persistence;
				LocalFrequency *= Lacunarity;
			}
			// 确保 Height 仍在 [0, 1] 内
			Height /= AmplitudeAccumulation;
			Height *= ExpFudgeFactor;
			Height = FMath::Pow(Height, Exp);
			
			
			// 地形因素
			if (bUseBaseTerrain)
			{
				if (Height < PlainHeight)
				{
					if (BaseTerrainVertices[x * SizeY + y] == BaseTerrainType::Plain)
					{
						Height *= PlainFactor;
					} else
					{
						Height *= HillFactor;
					}
				} else if (Height < HillHeight)
				{
					Height *= HillFactor;
				} else
				{
					if (BaseTerrainVertices[x * SizeY + y] == BaseTerrainType::Mountain)
					{
						Height *= MountainFactor;
					} else
					{
						Height *= HillFactor;
					}
				}
			} else
			{
				if (Height < PlainHeight)
				{
					Height *= PlainFactor;
				} else if (Height < HillHeight)
				{
					Height *= HillFactor;
				} else
				{
					Height *= MountainFactor;
				}
			}
			Vertices[x * SizeY + y] = {x * ScaleX, y * ScaleY, Height};
		}
	});
}

void ATerrainGenerator::CalculateLinearColors(const TArray<FVector>& Normals, TArray<FLinearColor>& VertexColors) const
{
	VertexColors.Empty();
	VertexColors.Reserve(Normals.Num());
	for (auto& Normal : Normals)
	{
		VertexColors.Add({
			StaticCast<float>(Normal.X),
			StaticCast<float>(Normal.Y),
			StaticCast<float>(Normal.Z)
		});
	}
}

void ATerrainGenerator::CalculateTriangles(TArray<int32>& Triangles) const
{
	Triangles.Empty();
	Triangles.Reserve(SizeX * SizeY);
	for (int32 x = 0; x < SizeX; x++)
	{
		for (int32 y = 0; y < SizeY - 1; y++)
		{
			int32 i = x * SizeY + y;

			Triangles.Add(i);
			Triangles.Add(i + 1);
			Triangles.Add(i + SizeY);

			Triangles.Add(i + 1);
			Triangles.Add(i + SizeY + 1);
			Triangles.Add(i + SizeY);
		}
	}
}

void ATerrainGenerator::SpawnGrass(const FVector& Pos)
{
	UStaticMesh* GrassMesh = GetRandomMesh(GrassMeshes, GrassTotalWeight);
	if (auto HISM = GrassHISMMap.Find(GrassMesh))
	{
		if (HISM->IsValid())
		{
			(*HISM)->AddInstance(FTransform {FRotator{0.f, FMath::FRandRange(0, 360.f), 0.f}, Pos});
		}
	}
}

void ATerrainGenerator::SpawnTree(const FVector& Pos)
{
	UStaticMesh* TreeMesh = GetRandomMesh(TreeMeshes, TreeTotalWeight);
	if (auto HISM = TreeHISMMap.Find(TreeMesh))
	{
		if (HISM->IsValid())
		{
			(*HISM)->AddInstance(FTransform {FRotator{0.f, FMath::FRandRange(0, 360.f), 0.f}, Pos});
		}
	}
}

void ATerrainGenerator::GenerateFoliage()
{
	if (bEnableGrass)
	{
		TArray<FVector2D> GrassPoints = UPoissonPoints::GeneratePoissonPoints(SizeX * ScaleX, SizeY * ScaleY, GrassRadius, RandomStream.RandRange(0, 200000),15);
		for (auto& Point : GrassPoints)
		{
			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, {Point.X, Point.Y, 10000},{Point.X, Point.Y, -10000}, ECC_GameTraceChannel1))
			{
				FVector HitNormal = HitResult.ImpactNormal;
				float Slope = FMath::RadiansToDegrees(FMath::Acos(FMath::Abs(HitNormal.Z)));
				if (Slope < GrassMaxSlope)
				{
					SpawnGrass(HitResult.ImpactPoint);
				}
			}
		}
	}
	
	if (bEnableTree)
	{
		TArray<FVector2D> TreePoints = UPoissonPoints::GeneratePoissonPoints(SizeX * ScaleX, SizeY * ScaleY, TreeRadius, RandomStream.RandRange(0, 200000),15);
		for (auto& Point : TreePoints)
		{
			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, {Point.X, Point.Y, 10000},{Point.X, Point.Y, -10000}, ECC_GameTraceChannel1))
			{
				FVector HitNormal = HitResult.ImpactNormal;
				float Slope = FMath::RadiansToDegrees(FMath::Acos(FMath::Abs(HitNormal.Z)));
				if (Slope < TreeMaxSlope)
				{
					SpawnTree(HitResult.ImpactPoint);
				}
			}
		}
	}
}

void ATerrainGenerator::ClearFoliage()
{
	for (auto& [k, HISM] : GrassHISMMap)
	{
		HISM->ClearInstances();
	}
	for (auto& [k, HISM] : TreeHISMMap)
	{
		HISM->ClearInstances();
	}
}

UStaticMesh* ATerrainGenerator::GetRandomMesh(const TArray<FTerrainMeshInfo>& MeshInfos, int32 TotalWeight) const
{
	int RandValue = RandomStream.FRandRange(0, TotalWeight);
	int32 Sum = 0;
	for (auto& [Mesh, Weight] : MeshInfos)
	{
		Sum += Weight;
		if (Sum >= RandValue)
		{
			return Mesh;
		}
	}
	return nullptr;
}

void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();
	RandomStream = FRandomStream(Seed);
	
	// GrassHISM
	for (auto& GrassMesh : GrassMeshes)
	{
		if (GrassMesh.StaticMesh)
		{
			if (auto GrassMeshComp = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass()))
			{
				GrassMeshComp->RegisterComponent();
				GrassMeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				GrassMeshComp->SetStaticMesh(GrassMesh.StaticMesh);
				GrassMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GrassHISMMap.Add(GrassMesh.StaticMesh, GrassMeshComp);
				GrassTotalWeight += GrassMesh.Weight;
				GrassMeshComp->SetCullDistances(GrassStartCullDistance, GrassEndCullDistance);
			}
		}
	}
	
	// Tree HISM
	for (auto TreeMesh : TreeMeshes)
	{
		if (TreeMesh.StaticMesh)
		{
			if (auto TreeMeshComp = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass()))
			{
				TreeMeshComp->RegisterComponent();
				TreeMeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				TreeMeshComp->SetStaticMesh(TreeMesh.StaticMesh);
				// TreeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				TreeHISMMap.Add(TreeMesh.StaticMesh, TreeMeshComp);
				TreeTotalWeight += TreeMesh.Weight;
				TreeMeshComp->SetCullDistances(TreeStartCullDistance, TreeEndCullDistance);
			}
		}
	}
	
	// 地形mesh
	ProceduralMesh->SetMaterial(0, TerrainMaterial);
	// 边界空气墙
	ProceduralMesh->SetMaterial(1, BorderMaterial);
}

void ATerrainGenerator::GenerateBorder()
{
	TArray<FVector> Vertices {
		{0, 0, 0},
		{(SizeX - 1) * ScaleX, 0, 0},
		{(SizeX - 1) * ScaleX, (SizeY - 1) * ScaleY, 0},
		{0, (SizeY - 1) * ScaleY, 0},
		
		{0, 0, 10000},
		{(SizeX - 1) * ScaleX, 0, 10000},
		{(SizeX - 1) * ScaleX, (SizeY - 1) * ScaleY, 10000},
		{0, (SizeY - 1) * ScaleY, 10000},
	};
	
	TArray<int32> Triangles {
		0, 1, 5,
		5, 4, 0,
		
		1, 2, 6,
		6, 5, 1,
		
		2, 3, 7,
		7, 6, 2,
		
		3, 0, 4,
		4, 7, 3
	};
	
	ProceduralMesh->CreateMeshSection_LinearColor(
		1,
		Vertices,
		Triangles,
		{},
		{},
		{},
		{},
		true
	);
}