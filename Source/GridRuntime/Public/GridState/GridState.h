// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridState.generated.h"

USTRUCT()
struct GRIDRUNTIME_API FGridCellOccupant
{
	
	GENERATED_BODY()

	UPROPERTY()
	FGuid ID;
	
};

USTRUCT()
struct GRIDRUNTIME_API FGridCell
{

	GENERATED_BODY()

	UPROPERTY()
	FIntVector Position;

	UPROPERTY()
	FGridCellOccupant Occupant;
	
};

USTRUCT()
struct GRIDRUNTIME_API FGridCellUpdate
{
	
	GENERATED_BODY()

	UPROPERTY()
	FGridCell Old;

	UPROPERTY()
	FGridCell New;
	
};

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

protected:
	TWeakObjectPtr<UGridState> Copy()
	{
		return this; // TODO
	}
	
};
