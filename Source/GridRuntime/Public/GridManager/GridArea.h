// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileCalculator/SquareTileCalculator.h"

#include "CoreMinimal.h"
#include "CollisionDebugDrawingPublic.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/LineComponent.h"
#include "GridState/DefaultGridState.h"
#include "GridState/GridState.h"
#include "Kismet/GameplayStatics.h"
#include "GridArea.generated.h"

USTRUCT()
struct GRIDRUNTIME_API FGridOccupantSpawnAction
{
	GENERATED_BODY()

	UPROPERTY()
	FGridCell Cell;

	UPROPERTY()
	TSubclassOf<UObject> AvatarClass;
};

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
	
	virtual void Tick(float DeltaSeconds) override
	{
		Super::Tick(DeltaSeconds);
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

// ~Game mode and player interactions

// State change delegates

	// Delegate called FGridOccupantSpawnDelegate that takes a FGridOccupantSpawnAction
	DECLARE_MULTICAST_DELEGATE_OneParam(FGridOccupantSpawnDelegate, FGridOccupantSpawnAction);
	FGridOccupantSpawnDelegate GridOccupantSpawnDelegate;

	// Delegate for when an occupant is moved
	DECLARE_MULTICAST_DELEGATE_OneParam(FGridOccupantMoveDelegate, FGridCellOccupantMoved);
	FGridOccupantMoveDelegate GridOccupantMoveDelegate;

private:

	void NotifyOccupantSpawned(FGridOccupantSpawnAction Update) const
	{
		if (HasAuthority())
		{
			UKismetSystemLibrary::PrintString(
			   GetWorld(),
			   TEXT("GridArea: Notifying Occupant Spawned"),
			   true,
			   true,
			   FLinearColor::Red,
			   3
		   );
			GridOccupantSpawnDelegate.Broadcast(Update);
		}
	}

	void NotifyOccupantMoved(FGridCellOccupantMoved Update) const
	{
		if (HasAuthority())
		{
			UKismetSystemLibrary::PrintString(
			   GetWorld(),
			   TEXT("GridArea: Notifying Occupant Moved"),
			   true,
			   true,
			   FLinearColor::Red,
			   3
		   );
			GridOccupantMoveDelegate.Broadcast(Update);
		}
	}

// ~State change delegates

// Grid construction and spatial utilities

public:
	
	UFUNCTION(BlueprintCallable)
	// TODO this needs to take a 3d coordinate
	FVector GetTileOrigin(const FIntPoint Coordinate)
	{
		const auto Rel = GetTileCalculator()->GridToRelative(FIntVector2(Coordinate.X, Coordinate.Y), TileSize);
		const auto World = Rel + FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const auto Hit = DetectTileSurfaces(
			FVector(World.X, World.Y, VolumeExtentWorldZTop()),
			FVector(World.X, World.Y, VolumeExtentWorldZBottom())
		);
		return Hit.Location;
	}

	// function that takes a world space FVector and returns the coordinate that represents it
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint GetCoordsForPosition(const FVector WorldLocation)
	{
		const auto Rel = FVector2D(WorldLocation.X, WorldLocation.Y) - FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const auto Grid = GetTileCalculator()->RelativeToGrid(Rel, TileSize);
		return FIntPoint(Grid.X, Grid.Y);
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

// Grid actions and dispatching

public:
	// blueprint callable function that takes a coordinate and a uclass pointer, updates the state object, and notifies listeners of the result
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SpawnOccupant(const FIntVector Coordinate, const TSubclassOf<AActor> AvatarClass)
	{
		// TODO this is just to verify replication on playercontroller
		// Won't be needed once pipeline is complete
		// So remove this condition
		if (HasAuthority())
		{
			UKismetSystemLibrary::PrintString(
			   GetWorld(),
			   TEXT("GridArea: Spawning Occupant"),
			   true,
			   true,
			   FLinearColor::Red,
			   3
		   );
			const auto SpawnAction = GridState->AddOccupant(Coordinate);
			
			NotifyOccupantSpawned({
				SpawnAction,
				AvatarClass
			});
		}
	}
	
	// MoveOccupant takes an id and a coordinate and updates the state to move that id
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void MoveOccupant(const FGuid OccupantId, const FIntVector NewPosition)
	{
		if (HasAuthority())
		{
			UKismetSystemLibrary::PrintString(
			   GetWorld(),
			   TEXT("GridArea: Moving Occupant"),
			   true,
			   true,
			   FLinearColor::Red,
			   3
			);

			const auto MoveAction = GridState->MoveOccupant(OccupantId, NewPosition);
			NotifyOccupantMoved(MoveAction);
		}
	}

// ~Grid actions and dispatching

// Display/Drawing

private:
	
	UPROPERTY()
	TObjectPtr<ULineComponent> PathTraceComponent;

	UPROPERTY(EditAnywhere)
	float ZOffset = 10;

	UPROPERTY(EditAnywhere)
	FLinearColor PathColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere)
	float LineThickness = 5;

public:
	
	UFUNCTION(BlueprintCallable)
	void SetGridPath(const TArray<FIntVector>& Path)
	{

		// map each point in path to the world space tile origin
		TArray<FVector> PathPoints;
		for (auto& Point : Path)
		{
			const auto TileOrigin = GetTileOrigin({
				Point.X,
				Point.Y
			});
			PathPoints.Add(TileOrigin + FVector(0, 0, ZOffset));
		}
		
		PathTraceComponent->SetPoints(PathPoints);
		PathTraceComponent->LineColor = PathColor;
		PathTraceComponent->LineThickness = LineThickness;
		
	}

// ~Display/Drawing

// Pathfinding

	// Function that takes a coordinates for a source and a destination and calculates the shortest path using A*
	// and returns a list of grid coordinates
	UFUNCTION(BlueprintCallable)
	bool FindPath(const FIntVector Source, const FIntVector Destination, TArray<FIntVector>& OutPath);
	

// ~Pathfinding
	
};
