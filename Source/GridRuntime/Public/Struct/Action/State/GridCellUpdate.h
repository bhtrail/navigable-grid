// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/Cell/GridCell.h"
#include "GridCellUpdate.generated.h"

USTRUCT()
struct GRIDRUNTIME_API FGridCellUpdate
{
	
	GENERATED_BODY()

	UPROPERTY()
	FGridCell Old;

	UPROPERTY()
	FGridCell New;
	
};
