// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/Cell/GridCell.h"
#include "GridOccupantSpawnAction.generated.h"

USTRUCT()
struct GRIDRUNTIME_API FGridOccupantSpawnAction
{
	GENERATED_BODY()

	UPROPERTY()
	FGridCell Cell;

	UPROPERTY()
	TSubclassOf<UObject> AvatarClass;
};
