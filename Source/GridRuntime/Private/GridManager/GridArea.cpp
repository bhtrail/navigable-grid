// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager/GridArea.h"

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
	
}

void AGridArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	DebugTracerInstancedMesh->SetWorldLocation(GetActorLocation() + FVector::UnitZ() * TestVolumeHeight / 2);
	DrawAllDebugTraces();
	
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

