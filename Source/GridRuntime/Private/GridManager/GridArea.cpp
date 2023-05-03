// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager/GridArea.h"

#include "Util/AStar.h"

AGridArea::AGridArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const auto TempRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(TempRoot);

	const auto DebugMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Grid/Meshes/Debug/DebugSenseVisualization"));
	const auto DebugMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Grid/Materials/Debug/TileVizualizeDecal"));
	DebugTracerInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugMeshComp"));
	DebugTracerInstancedMesh->SetMobility(EComponentMobility::Static);
	DebugTracerInstancedMesh->SetStaticMesh(DebugMesh.Object);
	DebugTracerInstancedMesh->SetMaterial(0, DebugMaterial.Object);
	DebugTracerInstancedMesh->SetCastShadow(false);
	DebugTracerInstancedMesh->SetupAttachment(RootComponent);
	
	DebugShapeComponent = CreateDefaultSubobject<UBoxComponent>("DebugShapeComponent");
	DebugShapeComponent->SetupAttachment(RootComponent);

	PathTraceComponent = CreateDefaultSubobject<ULineComponent>("PathTraceComponent");
	PathTraceComponent->SetupAttachment(RootComponent);
	
}

void AGridArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	DebugTracerInstancedMesh->SetWorldLocation(GetActorLocation() + FVector::UnitZ() * TestVolumeHeight / 2);
	DrawAllDebugTraces();

	GridState = NewObject<UGridState>(this, GridStateClass);
	
}

// Called when the game starts or when spawned
void AGridArea::BeginPlay()
{
	Super::BeginPlay();
}

void AGridArea::DrawAllDebugTraces()
{
	ClearDebugTraces();
	DebugTracerInstancedMesh->ClearInstances();
	TArray<FVector2D> Centers;
	GetAllTileCenters(Centers);
	TArray<FTransform> Transforms;
	for (auto& Center: Centers)
	{
		Transforms.Add(
			FTransform(
				FRotator::ZeroRotator,
				{Center.X, Center.Y, 0},
				FVector(1, 1, -TestVolumeHeight)
			)
		);
	}
	DebugTracerInstancedMesh->AddInstances(Transforms, false, false);
	DebugTracerInstancedMesh->SetVisibility(VisualizeTileSensingTraces, true);
}

bool AGridArea::FindPath(const FIntVector Source, const FIntVector Destination, TArray<FIntVector>& OutPath)
{

	FAStarSearch Search(Source, Destination, TileCalculator.GetEvenIfUnreachable());

	int32 Step = 0;
	while (!Search.Succ)
	{
		if (!Search.Step())
			break;

		// TODO
		// if (++Step > Request.MaxSearchStep)
		// {
		// 	// LOG_WARNING(TEXT("AGridManager::FindPath failed, out of MaxFindStep"));
		// 	break;
		// }
	}

	if (Search.Succ)
	{
		OutPath.Reset();
		Search.CollectPath(Source, OutPath);

		// if (Request.bRemoveDest)
		// {
		// 	Result.Pop();
		// }

		// TODO
		// if (Request.MaxCost >= 0)
		// {
		// 	int32 Cost = 0;
		// 	int32 i;
		// 	for (i = 1; i < Result.Num(); ++i)
		// 	{
		// 		Cost += PathFinder->GetCost(Result[i - 1], Result[i]);
		//
		// 		if (Cost > Request.MaxCost)
		// 			break;
		// 	}
		//
		// 	if (i < Result.Num())
		// 	{
		// 		Result.RemoveAt(i, Result.Num() - i);
		// 		Succ = false;
		// 	}
		// }
	}
	return Search.Succ;
	
}

