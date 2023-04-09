// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridArea.generated.h"

// TODO rename/merge with existing grid manager
UCLASS(Blueprintable, Abstract)
class GRIDRUNTIME_API AGridArea : public AActor
{
	
	GENERATED_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugTracerInstancedMesh;
	
public:	
	// Sets default values for this actor's properties
	AGridArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitDebugTraceSubcomponent()
	{
		auto debugMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Grid/Meshes/Debug/DebugSenseVisualization"));
		auto debugMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Grid/Materials/Debug/TileVizualizeDecal"));
		DebugTracerInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugMeshComp"));
		DebugTracerInstancedMesh->SetMobility(EComponentMobility::Static);
		DebugTracerInstancedMesh->SetStaticMesh(debugMesh.Object);
		DebugTracerInstancedMesh->SetMaterial(0, debugMaterial.Object);
		DebugTracerInstancedMesh->SetCastShadow(false);
		DebugTracerInstancedMesh->SetupAttachment(RootComponent);
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Height of the grid area in units
	// TODO move to populator?
	UPROPERTY(EditAnywhere, Category=Grid)
	float AreaHeight = 100;

	// TODO doc
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Grid)
	bool VisualizeTileSensingTraces;
	

protected:
	void DrawAllDebugTraces();

	void ClearDebugTraces() const
	{
		DebugTracerInstancedMesh->ClearInstances();
	}

	virtual void GetDebugTraceCenters(TArray<FVector2D>& Out)
	{
		// TODO warn
		Out = {};
	}
};
