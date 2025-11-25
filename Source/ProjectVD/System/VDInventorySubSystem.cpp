// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDInventorySubSystem.h"
#include "Public/VDConstrants.h"
#include "Object/VDInventoryInfo.h"

void UVDInventorySubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InventoryMap.Empty();
	for (int i = 0; i < VD_INVENTORY_ROW_COUNT * VD_INVENTORY_COLUMN_COUNT; ++i)
	{
		UVDInventoryInfo* EmptyItem = NewObject<UVDInventoryInfo>();
		EmptyItem->SetIsEmpty(true);
		InventoryMap.Add(i, EmptyItem);
	}
}

void UVDInventorySubSystem::Deinitialize()
{
	Super::Deinitialize();
}

UVDInventorySubSystem::UVDInventorySubSystem()
{

}

void UVDInventorySubSystem::SetInventoryItemBySlot(int32 Slot, UVDInventoryInfo* Item)
{

}

void UVDInventorySubSystem::RemoveInventoryItemBySlot(int32 Slot)
{
	InventoryMap.Remove(Slot);
}

void UVDInventorySubSystem::ClearInventory()
{
	InventoryMap.Empty();
}

void UVDInventorySubSystem::AddInventoryItem(const UVDInventoryInfo& Item)
{
	UVDInventoryInfo* NewItem = NewObject<UVDInventoryInfo>();
	NewItem->SetItemID(Item.GetItemID());
	NewItem->SetQuantity(Item.GetQuantity());
	NewItem->SetMaxQuantity(Item.GetMaxQuantity());
	NewItem->SetIsEmpty(false);

	// TODO :: 같은 아이템을 찾아서 겹칠 수있는 상태가 있다면 그걸 먼저 처리하도록 처리 추가필요
	for (auto& Pair : InventoryMap)
	{
		if (Pair.Value->GetItemID() == NewItem->GetItemID())
		{
			if (Pair.Value->GetQuantity() < Pair.Value->GetMaxQuantity())
			{
				int32 AvailableSpace = Pair.Value->GetMaxQuantity() - Pair.Value->GetQuantity();
				int32 QuantityToAdd = FMath::Min(AvailableSpace, NewItem->GetQuantity());
				Pair.Value->SetQuantity(Pair.Value->GetQuantity() + QuantityToAdd);
				
				int32 RemainingQuantity = NewItem->GetQuantity() - QuantityToAdd;
				if (RemainingQuantity <= 0)
				{
					return; // 모든 아이템이 추가되었으므로 종료
				}
				else
				{
					NewItem->SetQuantity(RemainingQuantity);
				}
			}
		}
	}

	if (NewItem->GetQuantity() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough space to add all items. Remaining quantity: %d"), NewItem->GetQuantity());
	}
}

bool UVDInventorySubSystem::IsInventoryFull() const
{
	return false;
}
