// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TileCalculator/SquareTileCalculator.h"
#include "GridTilePopulator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, editinlinenew, meta=(BlueprintSpawnableComponent), Abstract)
class GRIDRUNTIME_API UGridTilePopulator : public UPrimitiveComponent
{
	
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Grid)
	TSubclassOf<UTileCalculator> TileCalculatorClass = USquareTileCalculator::StaticClass();

protected:
	// X and Y size of each grid tile in units
	UPROPERTY(EditAnywhere, Category=Grid)
	double TileSize = 100;
	
	TWeakObjectPtr<UTileCalculator> TileCalculator;
	
public:
	virtual void GetAllTileCenters(TArray<FVector2D>& Out)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.0f,
				FColor::Yellow,
				TEXT("UGridTilePopulator::GetAllOrigins not implemented! Returning empty list.")
			);
		Out = TArray<FVector2D>();
	}

	virtual void OnRegister() override
	{
		Super::OnRegister();
		TileCalculator = NewObject<UTileCalculator>(this, TileCalculatorClass);
	}
};
