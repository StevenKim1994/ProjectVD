// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Object/VDInventoryInfo.h"
#include "System/VDInventorySubSystem.h"

void UVDInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (nullptr == ListItemObject)
	{
		return;
	}

	UVDInventoryInfo* InventoryItem = Cast<UVDInventoryInfo>(ListItemObject);
	if (nullptr == InventoryItem)
	{
		return;
	}

	CurrentInventoryInfo = InventoryItem;
	RefreshSlotWidget();
	
}

void UVDInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVDInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDInventorySlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDInventorySlotWidget::RefreshSlotWidget()
{
	if (CurrentInventoryInfo.IsValid())
	{
		if (CurrentInventoryInfo->GetIsEmpty())
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
}
