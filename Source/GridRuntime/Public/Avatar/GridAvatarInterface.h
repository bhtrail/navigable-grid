// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridAvatarInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGridAvatarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRIDRUNTIME_API IGridAvatarInterface
{
	GENERATED_BODY()

public:

	FGuid OccupantId;

	UFUNCTION(BlueprintNativeEvent)
	void SetOccupantId(FGuid Id);

	virtual void SetOccupantId_Implementation(FGuid Id)
	{
		OccupantId = Id;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGuid GetOccupantId();

	virtual FGuid GetOccupantId_Implementation()
	{
		return OccupantId;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool MoveTo(const FIntVector& CellCoordinate);

	virtual bool MoveTo_Implementation(const FIntVector& CellCoordinate)
	{
		return false;
	}
	
};
