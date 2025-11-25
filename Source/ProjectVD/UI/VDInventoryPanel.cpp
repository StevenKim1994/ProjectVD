// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VDInventoryPanel.h"
#include "Components/TileView.h"
#include "System/VDInventorySubSystem.h"
#include "Object/VDInventoryInfo.h"
#include "Public/VDConstrants.h"
void UVDInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryTileView)
	{
		auto& InventoryMap = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>()->GetInventoryMap();
		for (auto& Pair : InventoryMap)
		{
			InventoryTileView->AddItem(Pair.Value);
		}
	}
}

void UVDInventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();
}
