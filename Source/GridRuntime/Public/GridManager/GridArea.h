// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileCalculator/SquareTileCalculator.h"

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
	FVector GetTileOrigin(const FIntPoint Coordinate);

	FVector GetTileOrigin_Implementation(const FIntPoint Coordinate)
	{
		// TODO
		auto Rel = GetTileCalculator()->GridToRelative(FIntVector2(Coordinate.X, Coordinate.Y), TileSize);
		auto World = Rel + FVector2D(GetActorLocation().X, GetActorLocation().Y);
		
		return {World.X, World.Y, 0};
	}

private:
	void DrawAllDebugTraces();

	void ClearDebugTraces() const
	{
		DebugTracerInstancedMesh->ClearInstances();
	}
};
