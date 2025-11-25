// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VDInventoryPanel.h"
#include "Components/TileView.h"
#include "System/VDInventorySubSystem.h"
#include "Object/VDInventoryInfo.h"
void UVDInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

 	/*
	TMap<int32, FInventoryItem> InventoryMap = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>()->GetInventoryMap();
	for (auto& Pair : InventoryMap)
	{
	//	InventoryTileView->AddItem(MakeShared<FInventoryItem>(Pair.Value));
	}
	*/
}

void UVDInventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();
}
