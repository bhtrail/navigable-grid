// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCellOccupant.generated.h"

USTRUCT(BlueprintType)
struct GRIDRUNTIME_API FGridCellOccupant
{
	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGuid ID;
	
};
