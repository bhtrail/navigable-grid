// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileCalculator.h"
#include "UObject/Object.h"
#include "SquareTileCalculator.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API USquareTileCalculator : public UTileCalculator
{
	
	GENERATED_BODY()

public:
	// result XY will always be positive
	virtual FVector2D GridToRelative(const FIntVector2 Coords, const double TileSize) override
	{
		const auto HalfTile = TileSize / 2;
		const auto BaseLine = FVector2D(
			HalfTile,
			HalfTile
		);
		const auto XYVector = BaseLine + FVector2D(TileSize, TileSize) * FVector2D(Coords.X, Coords.Y);
		return XYVector;
	}
	
};
