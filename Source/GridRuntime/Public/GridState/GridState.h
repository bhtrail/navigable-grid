// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/Action/State/GridCellOccupantMoved.h"
#include "Struct/Action/State/GridCellUpdate.h"
#include "Struct/Cell/GridCell.h"
#include "UObject/Object.h"
#include "GridState.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GRIDRUNTIME_API UGridState : public UObject
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<UGridState> GetFilteredCopy(TFunction<bool(FGridCell*)> Predicate)
	{
		auto New = Copy();
		// TODO
		return New;
	}

	virtual FGridCell GetCell(const FIntVector Position)
	{
		// TODO warn
		return {};
	}

	virtual FGridCellUpdate UpdateCell(const FIntVector Position, const FGridCell New)
	{
		// TODO warn
		return {};
	}
	
	UFUNCTION()
	virtual FGridCell AddOccupant(const FIntVector Position)
	{
		// TODO warn
		return {};
	}

	UFUNCTION()
	virtual FGridCellOccupantMoved MoveOccupant(const FGuid OccupantID, const FIntVector NewPosition)
	{
		// TODO warn
		return {};
	}

protected:
	TWeakObjectPtr<UGridState> Copy()
	{
		return this; // TODO
	}
	
};
