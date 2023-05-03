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

	virtual FIntVector2 RelativeToGrid(const FVector2D Relative, const double TileSize) override
	{
		const auto XYVector = Relative - FVector2D{0, 0};
		const auto XYVectorDiv = XYVector / FVector2D(TileSize, TileSize);
		const auto XYVectorDivInt = FIntVector2(
			FMath::FloorToInt(XYVectorDiv.X),
			FMath::FloorToInt(XYVectorDiv.Y)
		);
		return XYVectorDivInt;
	}

	virtual void GetNeighbours(const FIntVector Coords, TArray<FIntVector>& Neighbours) override
	{
		ensure(Neighbours.IsEmpty());
		// TODO needs to be 3d
		Neighbours.Add(FIntVector(Coords.X - 1, Coords.Y, 0));
		Neighbours.Add(FIntVector(Coords.X + 1, Coords.Y, 0));
		Neighbours.Add(FIntVector(Coords.X, Coords.Y - 1, 0));
		Neighbours.Add(FIntVector(Coords.X, Coords.Y + 1, 0));
	}

	virtual int GetDistance(const FIntVector Start, const FIntVector End) override
	{
		// manhattan distance
		return FMath::Abs(Start.X - End.X) + FMath::Abs(Start.Y - End.Y);
	}
	
};
