// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GridDefaultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GRIDRUNTIME_API AGridDefaultGameMode : public AGameMode
{
	
	GENERATED_BODY()

	

	virtual void PostLogin(APlayerController* NewPlayer) override
	{
		
	}
	
};
