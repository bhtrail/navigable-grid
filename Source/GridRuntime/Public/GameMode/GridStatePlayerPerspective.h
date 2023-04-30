// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGridRuntime.h"
#include "Components/ActorComponent.h"
#include "GridManager/GridArea.h"
#include "Net/UnrealNetwork.h"
#include "GridStatePlayerPerspective.generated.h"


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIDRUNTIME_API UGridStatePlayerPerspective : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridStatePlayerPerspective();

	TWeakObjectPtr<AGridArea> GridSingleton;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TMap<FGuid, TSoftObjectPtr<AActor>> AvatarMap;

protected:

	virtual void OnRegister() override
	{
		Super::OnRegister();
		const auto Name = GetOwner()->GetName();
		// TODO subscribe to grid state updates on authority
		GridSingleton = AGridArea::GetSingleton(this);
		GEngine->AddOnScreenDebugMessage(
				-1,
				5,
				FColor::Red,
				FString::Printf(TEXT("Binding %ls to state change delegate"), ToCStr(Name))
			);
		if (GridSingleton.IsValid())
		{

			// bind OnGridAreaOccupantSpawn
			GridSingleton->GridOccupantSpawnDelegate.AddUObject(
				this,
				&UGridStatePlayerPerspective::OnGridAreaOccupantSpawn
			);

			// bind OnGridAreaOccupantSpawn
			GridSingleton->GridOccupantMoveDelegate.AddUObject(
				this,
				&UGridStatePlayerPerspective::OnGridAreaOccupantMove
			);
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5,
				FColor::Red,
				FString::Printf(TEXT("Perspective component for %ls could not find the Grid Manager singleton"), ToCStr(Name))
			);
		}
	}

	virtual void OnUnregister() override
	{
		Super::OnUnregister();
	}
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_DELEGATE_TwoParams(FGridStatePlayerPerspectiveChangedDelegate, TWeakObjectPtr<UGridState>, FGridCellUpdate)
	FGridStatePlayerPerspectiveChangedDelegate GridStatePlayerPerspectiveChangedDelegate;

	UFUNCTION( Client, Reliable )
	virtual void ClientSpawnAvatar(const FGridOccupantSpawnAction Action);

	virtual void ClientSpawnAvatar_Implementation(const FGridOccupantSpawnAction Action)
	{
		UE_LOG(
			GridRuntime,
			Log,
			TEXT("Spawning avatar of type %s at %s for occupant %s"),
			Action.AvatarClass ? *Action.AvatarClass->GetName() : TEXT("nullptr"),
			*Action.Cell.Position.ToString(),
			*Action.Cell.Occupant.ID.ToString()
		);
		if (Action.AvatarClass != nullptr)
		{
			const FVector Absolute = GridSingleton->GetTileOrigin({Action.Cell.Position.X, Action.Cell.Position.Y});
			const auto Avatar = GetWorld()->SpawnActorAbsolute(
				Action.AvatarClass,
				FTransform(
					Absolute
				)
			);
			AvatarMap.Add(
				Action.Cell.Occupant.ID,
				Avatar
			);
		}
	}

	UFUNCTION( Client, Reliable )
	virtual void ClientMoveAvatar(const FGridCellOccupantMoved Action);

	virtual void ClientMoveAvatar_Implementation(const FGridCellOccupantMoved Action)
	{
		UE_LOG(
			GridRuntime,
			Log,
			TEXT("Moving occupant %s from %s to %s"),
			*Action.OccupantID.ToString(),
			*Action.From.New.Position.ToString(),
			*Action.To.New.Position.ToString()
		);
		if (AvatarMap.Contains(Action.OccupantID))
		{
			const auto NewPosition = GridSingleton->GetTileOrigin(
				{
					Action.To.New.Position.X,
					Action.To.New.Position.Y
				}
			);
			const auto Avatar = AvatarMap[Action.OccupantID];
			// TODO this should call a self-defined 'Move' method on the avatar 
			Avatar->SetActorLocation(NewPosition);
		}
		else
		{
			UE_LOG(
				GridRuntime,
				Log,
				TEXT("Received request to move occupant %s, but no avatar was found"),
				*Action.OccupantID.ToString()
			);
		}
	}

private:
	
	// This function is called by the grid manager when an occupant is spawned on the grid
	// and responds by spawning an a
	virtual void OnGridAreaOccupantSpawn(const FGridOccupantSpawnAction Action)
	{
		ClientSpawnAvatar(Action);
	}

	void OnGridAreaOccupantMove(const FGridCellOccupantMoved Action)
	{
		ClientMoveAvatar(Action);
	}


public:
	
	UFUNCTION( BlueprintCallable, Server, Unreliable )
	void ServerDispatchAddOccupant(TSubclassOf<AActor> Class, const FIntVector GridPosition) const;
	
	void ServerDispatchAddOccupant_Implementation(TSubclassOf<AActor> Class, const FIntVector GridPosition) const
	{
		if (GridSingleton.IsValid())
		{
			GridSingleton->SpawnOccupant(GridPosition, Class);
		}
		else
		{
			UE_LOG(GridRuntime, Error, TEXT("GridStatePlayerPerspective could not find the Grid Manager singleton"));
		}
	}

	UFUNCTION( BlueprintCallable, Server, Unreliable )
	void ServerDispatchMoveOccupant(FGuid OccupantID, FIntVector GridPosition) const;
	
	void ServerDispatchMoveOccupant_Implementation(FGuid OccupantID, FIntVector GridPosition) const
	{
		if (GridSingleton.IsValid())
		{
			GridSingleton->MoveOccupant(OccupantID, GridPosition);
		}
		else
		{
			UE_LOG(
				GridRuntime,
				Error,
				TEXT("%s does not have a valid Grid Manager reference"),
				*GetName()
			);
		}
	}
	
};
