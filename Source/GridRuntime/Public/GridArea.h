// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridArea.generated.h"

UCLASS(Blueprintable)
class GRIDRUNTIME_API AGridArea : public AActor
{
	
	GENERATED_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> ShapeComponent;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugTracerInstancedMesh;
	
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
	UPROPERTY(EditAnywhere, Category=Grid)
	float AreaHeight = 100;

	// TODO doc
	UPROPERTY(EditAnywhere, Category=Grid)
	bool VisualizeTileSensingTraces;
	

private:
	void DrawAllDebugTraces() const;

	FVector GridToRelativePosition(int x, int y) const;

	void ClearDebugTraces() const
	{
		DebugTracerInstancedMesh->ClearInstances();
	}

};
