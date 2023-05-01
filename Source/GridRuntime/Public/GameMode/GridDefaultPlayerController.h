// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridStatePlayerPerspective.h"
#include "GridDefaultPlayerController.generated.h"

UCLASS(Blueprintable)
class GRIDRUNTIME_API AGridDefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UDefaultGridState* RelativeState;

public:

	UClass* TempAvatar;
	
	AGridDefaultPlayerController();

protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UGridStatePlayerPerspective> PlayerPerspective;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{

		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		// DOREPLIFETIME(AGridDefaultPlayerController, PerspectiveMap);
		
	}

	virtual void OnConstruction(const FTransform& Transform) override
	{
		Super::OnConstruction(Transform);
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
