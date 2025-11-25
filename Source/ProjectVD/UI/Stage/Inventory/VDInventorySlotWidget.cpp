// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Object/VDInventoryInfo.h"

void UVDInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (nullptr == ListItemObject)
	{
		return;
	}

	const UVDInventoryInfo* InventoryItem = Cast<UVDInventoryInfo>(ListItemObject);
	if (nullptr == InventoryItem)
	{
		return;
	}

	if (InventoryItem->GetIsEmpty())
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Visible);
		ItemQuantityText->SetVisibility(ESlateVisibility::Visible);
		// TODO :: 실제 아이템 존재하는 슬롯
	}
}
