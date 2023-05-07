// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCellUpdate.h"
#include "GridCellOccupantMoved.generated.h"

USTRUCT()
struct GRIDRUNTIME_API FGridCellOccupantMoved
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid OccupantID;

	UPROPERTY()
	FGridCellUpdate From;

	UPROPERTY()
	FGridCellUpdate To;
};
