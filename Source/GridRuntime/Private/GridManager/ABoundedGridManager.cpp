// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager/ABoundedGridManager.h"

ABoundedGridManager::ABoundedGridManager()
{
	
}

void ABoundedGridManager::GetAllTileCenters(TArray<FVector2D>& Out)
{
	
	Out = TArray<FVector2D>();
	for (int X = 0; X < GridDimensions.X; X++)
	{
		
		for (int Y = 0; Y < GridDimensions.Y; ++Y)
		{
			
			Out.Add(GetTileCalculator()->GridToRelative({X, Y}, TileSize));
			
		}
		
	}
	
}

FVector ABoundedGridManager::GetVisualCenter() const
{

	// TODO in the future
	// Move this to the abstract class
	// Use current extents and find the midpoint in each axis
	// this will be the visual center

	return FVector(
		GridDimensions.X * TileSize / 2,
		GridDimensions.Y * TileSize / 2,
		0
	);
	
}
