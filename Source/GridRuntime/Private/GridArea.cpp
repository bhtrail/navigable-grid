// Fill out your copyright notice in the Description page of Project Settings.


#include "GridArea.h"


void AGridArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DebugTracerInstancedMesh->SetWorldLocation(GetActorLocation() + FVector::UnitZ() * AreaHeight / 2);
	DrawAllDebugTraces();
}



// Sets default values
AGridArea::AGridArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// auto debugMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Grid/Meshes/Debug/DebugSegment"));

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
	
}

void AGridArea::DrawAllDebugTraces()
{
	ClearDebugTraces();
	DebugTracerInstancedMesh->ClearInstances();
	TArray<FVector2D> Centers;
	GetDebugTraceCenters(Centers);
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

