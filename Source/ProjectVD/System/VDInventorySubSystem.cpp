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
		EmptyItem->SetSlot(i);
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
	if (InventoryMap.Contains(Slot))
	{
		InventoryMap[Slot]->SetItemID(Item->GetItemID());
		InventoryMap[Slot]->SetItemType(Item->GetItemType());
		InventoryMap[Slot]->SetQuantity(Item->GetQuantity());
		InventoryMap[Slot]->SetMaxQuantity(Item->GetMaxQuantity());
		InventoryMap[Slot]->SetIsEmpty(Item->GetIsEmpty());
	}

	if (OnInventoryChangedDelegate.IsBound())
	{
		OnInventoryChangedDelegate.Broadcast(Item);
	}
}

void UVDInventorySubSystem::RemoveInventoryItemBySlot(int32 Slot, int32 Count)
{
	if (InventoryMap.Contains(Slot))
	{
		UVDInventoryInfo* ChangedItem = InventoryMap[Slot];
		int32 NewQuantity = ChangedItem->GetQuantity() - Count;
		if (NewQuantity > 0)
		{
			ChangedItem->SetQuantity(NewQuantity);
			ChangedItem->SetIsEmpty(false);
		}
		else
		{
			ChangedItem->SetIsEmpty(true);
		}
		
		if (OnInventoryChangedDelegate.IsBound())
		{
			OnInventoryChangedDelegate.Broadcast(ChangedItem);
		}
	}
}

void UVDInventorySubSystem::ClearInventory()
{
	InventoryMap.Empty();
}

void UVDInventorySubSystem::AddInventoryItem(const UVDInventoryInfo* Item)
{
	UVDInventoryInfo* NewItem = NewObject<UVDInventoryInfo>();
	NewItem->SetItemID(Item->GetItemID());
	NewItem->SetQuantity(Item->GetQuantity());
	NewItem->SetItemType(Item->GetItemType());
	NewItem->SetMaxQuantity(Item->GetMaxQuantity());
	NewItem->SetIsEmpty(false);

	// DESC :: 기존에 같은 아이템이 있다면 수량을 합침
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
					return;
				}
				else
				{
					NewItem->SetQuantity(RemainingQuantity);
				}
			}
		}
	}

	// DESC :: 남은 수량이 있다면 빈슬롯에 추가
	for (auto& Pair : InventoryMap)
	{
		if (Pair.Value->GetIsEmpty())
		{
			int32 QuantityToAdd = FMath::Min(NewItem->GetMaxQuantity(), NewItem->GetQuantity());
			Pair.Value->SetItemID(NewItem->GetItemID());
			Pair.Value->SetQuantity(QuantityToAdd);
			Pair.Value->SetMaxQuantity(NewItem->GetMaxQuantity());
			Pair.Value->SetItemType(NewItem->GetItemType());
			Pair.Value->SetIsEmpty(false);
			int32 RemainingQuantity = NewItem->GetQuantity() - QuantityToAdd;
			if (RemainingQuantity <= 0)
			{
				return;
			}
			else
			{
				NewItem->SetQuantity(RemainingQuantity);
			}
		}
	}

	if (NewItem->GetQuantity() > 0)
	{
		// TODO :: 이럴땐 어떻게 하면좋을까 ? 일딴 인벤토리 꽉참 알림
	}

	if (OnInventoryChangedDelegate.IsBound())
	{
		OnInventoryChangedDelegate.Broadcast(NewItem);
	}
}

bool UVDInventorySubSystem::IsInventoryFull() const
{
	return false;
}

UVDInventoryInfo* UVDInventorySubSystem::GetItem(EVDItemType ItemType, FName ItemID) const
{
	for (auto& Pair : InventoryMap)
	{
		if (!Pair.Value->GetIsEmpty() && Pair.Value->GetItemType() == ItemType && Pair.Value->GetItemID() == ItemID)
		{
			return Pair.Value;
		}
	}

	return nullptr;
}
