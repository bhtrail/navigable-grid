// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCellOccupant.h"
#include "GridCell.generated.h"

USTRUCT(BlueprintType)
struct GRIDRUNTIME_API FGridCell
{

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FIntVector Position;

	UPROPERTY(BlueprintReadWrite)
	FGridCellOccupant Occupant;
	
};
