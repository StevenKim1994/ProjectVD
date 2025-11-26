// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Object/VDInventoryInfo.h"
#include "System/VDInventorySubSystem.h"

void UVDInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UVDInventoryInfo* InventoryItem = Cast<UVDInventoryInfo>(ListItemObject);
	CurrentInventoryInfo = InventoryItem;
	// DESC :: null 일시 비어있는 슬롯처리
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

			ItemQuantityText->SetText(FText::AsNumber(CurrentInventoryInfo->GetQuantity()));

			switch (CurrentInventoryInfo->GetItemType())
			{
				case EVDItemType::Weapon:
				{
					// TODO :: 무기 아이콘 설정
				}
				break;
				case EVDItemType::Consumable:
				{
					// TODO :: 소비 아이템 아이콘 설정
				}
				break;
			}
		}
	}
	else
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
