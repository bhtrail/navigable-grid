// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager/ABoundedGridManager.h"

ABoundedGridManager::ABoundedGridManager()
{

	TilePopulator = CreateDefaultSubobject<UBoundedGridTilePopulator>("TilePopulator");
	TilePopulator->SetupAttachment(ShapeComponent);
	
}