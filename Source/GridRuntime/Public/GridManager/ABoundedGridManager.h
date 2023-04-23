// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridArea.h"
#include "ABoundedGridManager.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API ABoundedGridManager : public AGridArea
{
	
	GENERATED_BODY()

	ABoundedGridManager();

	UPROPERTY(EditAnywhere, Category=Grid)
	FIntVector2 GridDimensions = {3, 3};

protected:

	virtual void Tick(float DeltaSeconds) override
	{
		Super::Tick(DeltaSeconds);
	}

	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
		DebugShapeComponent->SetBoxExtent(
			FVector(
				TileSize * GridDimensions.X / 2,
				TileSize * GridDimensions.Y / 2,
				TestVolumeHeight / 2
			)
		);
		DebugShapeComponent->SetRelativeLocation(
			GetVisualCenter()
		);
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		IndexAllTilesWithinBounds();
	}

	UFUNCTION(BlueprintCallable)
	void IndexAllTilesWithinBounds()
	{
		for (int X = 0; X < GridDimensions.X; X++)
		{
		
			for (int Y = 0; Y < GridDimensions.Y; ++Y)
			{
			
				GetTileOrigin({X, Y});
			
			}
		
		}
	}

public:

	virtual void GetAllTileCenters(TArray<FVector2D>& Out) override;

private:
	FVector GetVisualCenter() const;
};
