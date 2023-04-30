// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TileCalculator.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GRIDRUNTIME_API UTileCalculator : public UObject
{
	
	GENERATED_BODY()

public:
	virtual FVector2D GridToRelative(const FIntVector2 Coords, const double TileSize)
	{
		// TODO warn
		return FVector2d::Zero();
	}

	virtual FIntVector2 RelativeToGrid(const FVector2D Relative, const double TileSize)
	{
		// TODO warn
		return {};
	}
	
};
