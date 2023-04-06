// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridArea.h"
#include "TilePopulator/BoundedGridTilePopulator.h"
#include "UObject/Object.h"
#include "ABoundedGridManager.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API ABoundedGridManager : public AGridArea
{
	
	GENERATED_BODY()

	ABoundedGridManager();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoundedGridTilePopulator> TilePopulator;

	virtual void GetDebugTraceCenters(TArray<FVector2D>& Out) override
	{
		TilePopulator->GetAllTileCenters(Out);
	}
};
