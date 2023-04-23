// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridStatePlayerPerspective.h"
#include "GridDefaultPlayerController.generated.h"

UCLASS()
class GRIDRUNTIME_API AGridDefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UDefaultGridState* RelativeState;

public:

	UClass* TempAvatar;

	UFUNCTION(NetMulticast, Reliable)
	void TestRPC(FGridCellUpdate Update);

	void TestRPC_Implementation( FGridCellUpdate Update )
	{
		
		UKismetSystemLibrary::PrintString(
			GetWorld(),
			Update.Old.Position.ToString()
				.Append(Update.Old.Occupant.ID.ToString())
				.Append(" -> ")
				.Append(Update.New.Occupant.ID.ToString())
		);

		RelativeState->ApplyUpdate(Update);

		UKismetSystemLibrary::PrintString(
			GetWorld(),
			RelativeState->GetCell(Update.New.Position).Occupant.ID.ToString()
		);

		PlayerPerspective->SpawnAvatar(
			TempAvatar,
			Update.New.Position
		);
		
	}

	// TODO remove State param
	void OnGridCellUpdate(TWeakObjectPtr<UGridState> State, FGridCellUpdate Update)
	{

		TestRPC(Update);
		
	}
	
	AGridDefaultPlayerController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UGridStatePlayerPerspective> PlayerPerspective;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{

		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		// DOREPLIFETIME(AGridDefaultPlayerController, PerspectiveMap);
		
	}

	virtual void OnConstruction(const FTransform& Transform) override
	{
		PlayerPerspective->GridStatePlayerPerspectiveChangedDelegate.BindUObject(
			this,
			&AGridDefaultPlayerController::OnGridCellUpdate
		);
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
