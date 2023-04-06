// Fill out your copyright notice in the Description page of Project Settings.


#include "GridArea.h"


void AGridArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ShapeComponent->SetBoxExtent(
		FVector(
			TileSize * GridDimensions.X / 2,
			TileSize * GridDimensions.Y / 2,
			AreaHeight / 2
		)
	);
	ShapeComponent->SetMobility(EComponentMobility::Static);
	DebugTracerInstancedMesh->SetWorldLocation(GetActorLocation() + FVector::UnitZ() * AreaHeight / 2);
	DrawAllDebugTraces();
}



// Sets default values
AGridArea::AGridArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ShapeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ShapeComp"));
	SetRootComponent(ShapeComponent);
	ShapeComponent->SetMobility(EComponentMobility::Static);

	// auto debugMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Grid/Meshes/Debug/DebugSegment"));
	auto debugMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Grid/Meshes/Debug/DebugSenseVisualization"));
	auto debugMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Grid/Materials/Debug/TileVizualizeDecal"));
	DebugTracerInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugMeshComp"));
	DebugTracerInstancedMesh->SetMobility(EComponentMobility::Static);
	DebugTracerInstancedMesh->SetStaticMesh(debugMesh.Object);
	DebugTracerInstancedMesh->SetMaterial(0, debugMaterial.Object);
	DebugTracerInstancedMesh->SetCastShadow(false);
	DebugTracerInstancedMesh->SetupAttachment(ShapeComponent);

	

	// DebugTraceActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("DebugTraceActor"));
	// DebugTraceActor->SetupAttachment(RootComponent);
	// DebugTraceActor->SetChildActorClass()
	
}

// Called when the game starts or when spawned
void AGridArea::BeginPlay()
{
	Super::BeginPlay();
	SetHidden(false);
}

// Called every frame
void AGridArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto bounds = ShapeComponent->Bounds.GetBox();
	auto top = bounds.Max.Z;
	auto bottom = bounds.Min.Z;

	FHitResult result;
	// DrawDebugLine(
	// 	GetWorld(),
	// 	FVector(GetActorLocation().X, GetActorLocation().Y, top),
	// 	FVector(GetActorLocation().X, GetActorLocation().Y, bottom),
	// 	FColor::Red,
	// 	true,
	// 	-1,
	// 	0,
	// 	10
	// );
	
}

void AGridArea::DrawAllDebugTraces()
{
	ClearDebugTraces();
	DebugTracerInstancedMesh->ClearInstances();
	TArray<FVector2D> Centers;
	GetDebugTraceCenters(Centers);
	// for (int x = 0; x < GridDimensions.X; ++x)
	// {
	// 	for (int y = 0; y < GridDimensions.Y; ++y)
	// 	{
	// 		Transforms.Add(
	// 			FTransform(
	// 				FRotator::ZeroRotator,
	// 				GridToRelativePosition(x, y),
	// 				FVector(1, 1, -AreaHeight)
	// 			)
	// 		);
	// 	}
	// }
	TArray<FTransform> Transforms;
	for (auto& Center: Centers)
	{
		Transforms.Add(
			FTransform(
				FRotator::ZeroRotator,
				{Center.X, Center.Y, 0},
				FVector(1, 1, -AreaHeight)
			)
		);
	}
	DebugTracerInstancedMesh->AddInstances(Transforms, false, false);
	DebugTracerInstancedMesh->SetVisibility(VisualizeTileSensingTraces, true);
}

FVector AGridArea::GridToRelativePosition(const int x, const int y) const
{
	auto HalfTile = TileSize / 2;
	auto BaseLine = FVector2D(
		HalfTile * -(GridDimensions.X - 1),
		HalfTile * -(GridDimensions.Y - 1)
	);
	auto XYVector = BaseLine + FVector2D(TileSize, TileSize) * FVector2D(x, y);
	return FVector(XYVector.X, XYVector.Y, 0);
}

