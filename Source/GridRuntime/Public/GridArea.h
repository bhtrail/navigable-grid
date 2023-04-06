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

protected:
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> ShapeComponent;
	
public:	
	// Sets default values for this actor's properties
	AGridArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// X and Y size of each grid tile in units
	UPROPERTY(EditAnywhere, Category=Grid)
	float TileSize = 100;

	// TODO doc
	UPROPERTY(EditAnywhere, Category=Grid)
	FIntVector2 GridDimensions = FIntVector2(3, 3);

	// Height of the grid area in units
	// TODO move to populator?
	UPROPERTY(EditAnywhere, Category=Grid)
	float AreaHeight = 100;

	// TODO doc
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Grid)
	bool VisualizeTileSensingTraces;
	

protected:
	void DrawAllDebugTraces();

	FVector GridToRelativePosition(int x, int y) const;

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
