// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileCalculator/SquareTileCalculator.h"

#include "CoreMinimal.h"
#include "CollisionDebugDrawingPublic.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridArea.generated.h"

// TODO rename/merge with existing grid manager
UCLASS(Blueprintable, Abstract)
class GRIDRUNTIME_API AGridArea : public AActor
{
	
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugTracerInstancedMesh;

	// TODO doc
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Grid)
	bool VisualizeTileSensingTraces;

	UPROPERTY(EditAnywhere, Category=Grid)
	TSubclassOf<UTileCalculator> TileCalculatorClass = USquareTileCalculator::StaticClass();

	TWeakObjectPtr<UTileCalculator> TileCalculator;
	
public:	
	// Sets default values for this actor's properties
	AGridArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TObjectPtr<UBoxComponent> DebugShapeComponent;

	// X and Y size of each grid tile in units
	UPROPERTY(EditAnywhere, Category=Grid)
	double TileSize = 100;

	// Height of the grid area in units
	UPROPERTY(EditAnywhere, Category=Grid)
	double TestVolumeHeight = 100;
	
	TWeakObjectPtr<UTileCalculator> GetTileCalculator()
	{
		if (!TileCalculator.IsValid())
		{
			TileCalculator = NewObject<UTileCalculator>(this, TileCalculatorClass);
		}
		return TileCalculator;
	}

	virtual void GetAllTileCenters(TArray<FVector2D>& Out)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				TEXT("UGridTilePopulator::GetAllOrigins not implemented! Returning empty list.")
			);
		Out = TArray<FVector2D>();
	}

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float VolumeExtentWorldZTop() const
	{
		const auto Bounds = DebugShapeComponent->Bounds.GetBox();
		return Bounds.Max.Z;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float VolumeExtentWorldZBottom() const
	{
		const auto Bounds = DebugShapeComponent->Bounds.GetBox();
		return Bounds.Min.Z;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FHitResult DetectTileSurfaces(const FVector Start, const FVector End);

	FHitResult DetectTileSurfaces_Implementation(const FVector Start, const FVector End)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByProfile(
			Hit,
			Start,
			End,
			TEXT("None"),
			FCollisionQueryParams(
				"Tag",
				true,
				this
			)
		);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				10.0f,
				FColor::Yellow,
				Hit.Location.ToString()
			);
		DrawLineTraces(GetWorld(), Start, End, {Hit}, 10);
		return Hit;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetTileOrigin(const FIntPoint Coordinate)
	{
		// TODO
		const auto Rel = GetTileCalculator()->GridToRelative(FIntVector2(Coordinate.X, Coordinate.Y), TileSize);
		const auto World = Rel + FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const auto Hit = DetectTileSurfaces(
			FVector(World.X, World.Y, VolumeExtentWorldZTop()),
			FVector(World.X, World.Y, VolumeExtentWorldZBottom())
		);
		return Hit.Location;
	}

private:
	void DrawAllDebugTraces();

	void ClearDebugTraces() const
	{
		DebugTracerInstancedMesh->ClearInstances();
	}
};
