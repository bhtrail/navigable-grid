// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SquareTileCalculator.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API USquareTileCalculator : public UObject
{
	
	GENERATED_BODY()

public:
	// result XY will always be positive
	FVector2D GridToRelative(const FIntVector2 Coords, const double TileSize)
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
