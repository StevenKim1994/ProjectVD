// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDInventorySubSystem.h"

void UVDInventorySubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDInventorySubSystem::Deinitialize()
{
	Super::Deinitialize();
}

UVDInventorySubSystem::UVDInventorySubSystem()
{
	InventoryMap.Empty();
}

void UVDInventorySubSystem::SetInventoryItemBySlot(int32 Slot, const FInventoryItem& Item)
{
}

void UVDInventorySubSystem::RemoveInventoryItemBySlot(int32 Slot)
{
}

void UVDInventorySubSystem::ClearInventory()
{
}

void UVDInventorySubSystem::AddInventoryItem(const FInventoryItem& Item)
{
}
