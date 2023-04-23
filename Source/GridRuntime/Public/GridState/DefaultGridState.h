// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "DefaultGridState.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API UDefaultGridState : public UGridState
{
	GENERATED_BODY()

	TMap<FIntVector, FGridCell> StateMap;

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

	// TODO maybe should be on superclass
	void ApplyUpdate(FGridCellUpdate Update)
	{

		UpdateCell(Update.New.Position, Update.New);
		
	}
	
};
