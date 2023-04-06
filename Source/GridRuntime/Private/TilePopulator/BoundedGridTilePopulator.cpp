// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePopulator/BoundedGridTilePopulator.h"


// Sets default values for this component's properties
UBoundedGridTilePopulator::UBoundedGridTilePopulator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBoundedGridTilePopulator::OnRegister()
{

	Super::OnRegister();
	
}


// Called when the game starts
void UBoundedGridTilePopulator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBoundedGridTilePopulator::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBoundedGridTilePopulator::GetAllTileCenters(TArray<FVector2D>& Out)
{
	
	check(TileCalculator.IsValid());
	Out = TArray<FVector2D>();
	for (int X = 0; X < GridDimensions.X; X++)
	{
		
		for (int Y = 0; Y < GridDimensions.Y; ++Y)
		{
			
			Out.Add(TileCalculator->GridToRelative({X, Y}, TileSize));
			
		}
		
	}
	
}

// TArray<FVector3> UBoundedGridTilePopulator::GetAllOrigins()
// {
// 	return
// }

