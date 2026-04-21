#include "Manager/TerrainGenerator.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "ProceduralMeshComponent/Public/KismetProceduralMeshLibrary.h"


ATerrainGenerator::ATerrainGenerator()
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
}

float ATerrainGenerator::Noise(int32 X, int32 Y, float InFrequency) const
{
	return FMath::PerlinNoise2D(SeedOffset + FVector2D{X * InFrequency, Y * Frequency});
}

void ATerrainGenerator::GenerateTerrain()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FProcMeshTangent> Tangents;
	
	for (int32 x = 0; x < SizeX; x++)
	{
		for (int32 y = 0; y < SizeY; y++)
		{
			float Height = 0;
			float LocalAmplitude = Amplitude;
			float LocalFrequency = Frequency;
			for (int i = 0; i < Layer; i++)
			{
				Height += Noise(x, y, Frequency) * LocalAmplitude;
				LocalAmplitude *= Persistence;
				LocalFrequency *= Lacunarity;
			}
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
				
			Vertices.Add({x * ScaleX, y * ScaleY, Height});
		}
	}
	TArray<FLinearColor> VertexColors;
	VertexColors.Reserve(Vertices.Num());
	
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		VertexColors.Add(VertexLinearColor);
	}
	
	for (int32 x = 0; x < SizeX - 1; x++)
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
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		Vertices,
		Triangles,
		UV,
		Normals,
		Tangents
	);
	
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
}

void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateTerrain();
	ProceduralMesh->SetMaterial(0, TerrainMaterial);
	RandomStream = FRandomStream(Seed);
	SeedOffset = {StaticCast<float>(RandomStream.RandRange(0, 10000)),StaticCast<float>( RandomStream.RandRange(0, 10000))};
}
