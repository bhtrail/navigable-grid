// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GridDefaultPlayerController.h"

AGridDefaultPlayerController::AGridDefaultPlayerController()
{
	
	PrimaryActorTick.bCanEverTick = true;
	PlayerPerspective = CreateDefaultSubobject<UGridStatePlayerPerspective>("PlayerPerspective");
	PlayerPerspective->RegisterComponent();

	RelativeState = CreateDefaultSubobject<UDefaultGridState>("RelativeState");

	static ConstructorHelpers::FClassFinder<AActor> Finder(TEXT("/Game/Blueprints/DebugAvatar"));
	TempAvatar = Finder.Class;
	
}

// Called when the game starts or when spawned
void AGridDefaultPlayerController::BeginPlay()
{
	
	Super::BeginPlay();
	
}

// Called every frame
void AGridDefaultPlayerController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
}

