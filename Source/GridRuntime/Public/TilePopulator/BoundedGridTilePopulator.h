// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridTilePopulator.h"
#include "Components/ActorComponent.h"
#include "BoundedGridTilePopulator.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIDRUNTIME_API UBoundedGridTilePopulator : public UGridTilePopulator
{
	GENERATED_BODY()

	virtual void OnRegister() override;
	
public:
	// Sets default values for this component's properties
	UBoundedGridTilePopulator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FIntVector2 GridDimensions = {3, 3};

	virtual void GetAllTileCenters(TArray<FVector2D>& Out) override;
	
};
