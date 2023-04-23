// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileCalculator/SquareTileCalculator.h"

#include "CoreMinimal.h"
#include "CollisionDebugDrawingPublic.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridState/DefaultGridState.h"
#include "GridState/GridState.h"
#include "Kismet/GameplayStatics.h"
#include "GridArea.generated.h"

// TODO rename/merge with existing grid manager
UCLASS(Blueprintable, Abstract)
class GRIDRUNTIME_API AGridArea : public AActor
{
	
	GENERATED_BODY()

// AActor builtins

public:
	
	AGridArea();

protected:
	
	virtual void BeginPlay() override;

	float total = 0;
	
	virtual void Tick(float DeltaSeconds) override
	{
		Super::Tick(DeltaSeconds);
		if (HasAuthority())
		{
			total += DeltaSeconds;
			if (total > 3)
			{
				total = 0;
				auto loc = FIntVector{
					FMath::RandRange(0, 20),
					FMath::RandRange(0, 10),
					0
				};
				FGridCellUpdate Update = GridState->UpdateCell(
					loc,
					{
						loc,
						{FGuid::NewGuid()}
					}
				);
				NotifyUpdateListeners(Update);
			}
		}
		
	}

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override
	{
		Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	}

	virtual void OnConstruction(const FTransform& Transform) override;

// ~AActor builtins

// Game mode and player interactions
	
public:
	
	static AGridArea* GetSingleton(const UObject* Context)
	{
		return static_cast<AGridArea*>(UGameplayStatics::GetActorOfClass(Context, StaticClass()));
	}
	
	DECLARE_DELEGATE_TwoParams(FStateUpdateDelegate, TWeakObjectPtr<UGridState>, FGridCellUpdate);
	FStateUpdateDelegate GridStateUpdateDelegate;

private:
	
	void NotifyUpdateListeners(FGridCellUpdate Update) const
	{
		// TODO this is just to verify replication on playercontroller
		// Won't be needed once pipeline is complete
		// So remove this condition
		if (HasAuthority())
		{
			UKismetSystemLibrary::PrintString(
			   GetWorld(),
			   TEXT("Notifying listeners"),
			   true,
			   true,
			   FLinearColor::Red,
			   3
		   );
			GridStateUpdateDelegate.ExecuteIfBound(GridState, Update);
		}
	}

// ~Game mode and player interactions

// Grid construction and spatial utilities

public:
	UFUNCTION(BlueprintCallable)
	FVector GetTileOrigin(const FIntPoint Coordinate)
	{
		// TODO idk what this todo was for /shrug
		const auto Rel = GetTileCalculator()->GridToRelative(FIntVector2(Coordinate.X, Coordinate.Y), TileSize);
		const auto World = Rel + FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const auto Hit = DetectTileSurfaces(
			FVector(World.X, World.Y, VolumeExtentWorldZTop()),
			FVector(World.X, World.Y, VolumeExtentWorldZBottom())
		);
		return Hit.Location;
	}
	
protected:

	UPROPERTY()
	TObjectPtr<UBoxComponent> DebugShapeComponent;

	// X and Y size of each grid tile in units
	UPROPERTY(EditAnywhere, Category=Grid)
	double TileSize = 100;

	// Height of the grid area in units
	UPROPERTY(EditAnywhere, Category=Grid)
	double TestVolumeHeight = 100;
	
	TWeakObjectPtr<UTileCalculator> GetTileCalculator()
	{
		if (!TileCalculator.IsValid())
		{
			TileCalculator = NewObject<UTileCalculator>(this, TileCalculatorClass);
		}
		return TileCalculator;
	}

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

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float VolumeExtentWorldZTop() const
	{
		const auto Bounds = DebugShapeComponent->Bounds.GetBox();
		return Bounds.Max.Z;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float VolumeExtentWorldZBottom() const
	{
		const auto Bounds = DebugShapeComponent->Bounds.GetBox();
		return Bounds.Min.Z;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FHitResult DetectTileSurfaces(const FVector Start, const FVector End);

	FHitResult DetectTileSurfaces_Implementation(const FVector Start, const FVector End)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByProfile(
			Hit,
			Start,
			End,
			TEXT("None"),
			FCollisionQueryParams(
				"Tag",
				true,
				this
			)
		);
		DrawLineTraces(GetWorld(), Start, End, {Hit}, 10);
		return Hit;
	}

private:
	
	void DrawAllDebugTraces();

	void ClearDebugTraces() const
	{
		DebugTracerInstancedMesh->ClearInstances();
	}
	
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugTracerInstancedMesh;

	// TODO doc
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Grid)
	bool VisualizeTileSensingTraces;

	UPROPERTY(EditAnywhere, Category=Grid)
	TSubclassOf<UTileCalculator> TileCalculatorClass = USquareTileCalculator::StaticClass();

	TWeakObjectPtr<UTileCalculator> TileCalculator;

// ~Grid construction and visualization

// Grid state

private:
	
	UPROPERTY()
	TSubclassOf<UGridState> GridStateClass = UDefaultGridState::StaticClass();

	UPROPERTY()
	// TODO weak ptr gets garbage collected
	// verify this is safe
	UGridState * GridState;
	// TWeakObjectPtr<UGridState> GridState;
	
// ~Grid state
	
};
