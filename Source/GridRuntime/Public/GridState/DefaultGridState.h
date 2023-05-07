// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "IGridRuntime.h"
#include "DefaultGridState.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API UDefaultGridState : public UGridState
{
	GENERATED_BODY()

	TMap<FIntVector, FGridCell> StateMap;
	TMap<FGuid, FIntVector> OccupantPositions;

public:
	// TODO return bool and give result by reference
	virtual FGridCell GetCell(const FIntVector Position) override
	{
		if (StateMap.Contains(Position))
			return StateMap[Position];
		UKismetSystemLibrary::PrintString(
			GetWorld(),
			FString::Printf(TEXT("Does not exist in state: %d, %d, %d"), Position.X, Position.Y, Position.Z)
		);
		return {};
	}

private:
	virtual FGridCellUpdate UpdateCell(const FIntVector Position, const FGridCell New) override
	{
		FGridCellUpdate Update;
		Update.New = New;
		if (StateMap.Contains(Position))
		{
			FGridCell Old;
			StateMap.RemoveAndCopyValue(Position, Old);
			Update.Old = Old;
		}
		StateMap.Add(Position, New);
		return Update;
	}

	// method that removes a cell from the state map
	virtual FGridCellUpdate RemoveCell(const FIntVector Position) /* TODO override */
	{
		FGridCellUpdate Update;
		if (StateMap.Contains(Position))
		{
			Update.Old = StateMap[Position];
			StateMap.Remove(Position);
			Update.New = Update.Old;
			Update.New.Occupant = {};
		}
		return Update;
	}

public:
	virtual FGridCell AddOccupant(const FIntVector Position) override
	{
		FGridCell Cell;
		Cell.Position = Position;
		Cell.Occupant.ID = FGuid::NewGuid();
		UpdateCell(Position, Cell);
		OccupantPositions.Add(Cell.Occupant.ID, Position);
		return Cell;
	}

	FGridCellOccupantMoved MoveOccupant(const FGuid OccupantID, const FIntVector NewPosition)
	{
		if (OccupantPositions.Contains(OccupantID))
		{
			FGridCellOccupantMoved Moved;
			Moved.OccupantID = OccupantID;
			Moved.From = RemoveCell(OccupantPositions[OccupantID]);
			Moved.To = UpdateCell(NewPosition, {
				NewPosition,
				{
					OccupantID
				}
			});
			// update the new cell with the occupant
		
			OccupantPositions[OccupantID] = NewPosition;
			return Moved;
		}
		// log an error
		UE_LOG(
			GridRuntime,
			Error,
			TEXT("Tried to move occupant that doesn't exist: %s"),
			*OccupantID.ToString()
		);
		return {};
	}

	
};
