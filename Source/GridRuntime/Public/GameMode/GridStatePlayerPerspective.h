// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridManager/GridArea.h"
#include "Net/UnrealNetwork.h"
#include "GridStatePlayerPerspective.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIDRUNTIME_API UGridStatePlayerPerspective : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridStatePlayerPerspective();

	TWeakObjectPtr<AGridArea> GridSingleton;

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
			GridSingleton->GridStateUpdateDelegate.BindUObject(
				this,
				&UGridStatePlayerPerspective::OnGridStateChange
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

	virtual void SpawnAvatar(UClass* Class, FIntVector GridPosition)
	{
		const FVector Absolute = GridSingleton->GetTileOrigin({GridPosition.X, GridPosition.Y});
		GetWorld()->SpawnActorAbsolute(
			Class,
			FTransform(
				Absolute
			)
		);
	}

private:
	// TODO remove state param
	void OnGridStateChange(TWeakObjectPtr<UGridState> state, FGridCellUpdate update)
	{
		GridStatePlayerPerspectiveChangedDelegate.Execute(nullptr, update);
	}
};
