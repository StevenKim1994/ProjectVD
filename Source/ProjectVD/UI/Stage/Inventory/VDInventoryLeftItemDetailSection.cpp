// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventoryLeftItemDetailSection.h"
#include "Object/VDInventoryInfo.h"
#include "System/VDInventorySubSystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UVDInventoryLeftItemDetailSection::UpdateItemDetailInfo(UVDInventoryInfo* InInventoryInfo)
{
	switch (InInventoryInfo->GetItemType())
	{
		case EVDItemType::Weapon:
		{
			UseButtonText->SetText(FText::FromString(TEXT("장착하기")));
		}
		break;
		case EVDItemType::Consumable:
		{
			UseButtonText->SetText(FText::FromString(TEXT("사용하기")));
		}
		break;
	}
}

void UVDInventoryLeftItemDetailSection::OnClickedUseButton()
{
	switch(CurrentInventoryInfo->GetItemType())
	{
		case EVDItemType::Weapon:
		{
			// TODO :: 무기 장착 로직
		}
		break;
		case EVDItemType::Consumable:
		{
			// TODO :: 소비 아이템 사용 로직
		}
		break;
	}
}

void UVDInventoryLeftItemDetailSection::OnClickedDeleteButton()
{
	UVDInventorySubSystem* InventorySubSystem = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();
	if (InventorySubSystem)
	{
		InventorySubSystem->RemoveInventoryItemBySlot(CurrentInventoryInfo->GetSlot());
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UVDInventoryLeftItemDetailSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UseButton->OnClicked.AddDynamic(this, &UVDInventoryLeftItemDetailSection::OnClickedUseButton);
	DeleteButton->OnClicked.AddDynamic(this, &UVDInventoryLeftItemDetailSection::OnClickedDeleteButton);
}

void UVDInventoryLeftItemDetailSection::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDInventoryLeftItemDetailSection::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDInventoryLeftItemDetailSection::SetItemDetailInfo(UVDInventoryInfo* InInventoryInfo)
{
	CurrentInventoryInfo = InInventoryInfo;
	if (CurrentInventoryInfo.IsValid())
	{
		UpdateItemDetailInfo(CurrentInventoryInfo.Get());
	}
}
