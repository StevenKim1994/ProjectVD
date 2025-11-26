// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventoryLeftItemDetailSection.h"
#include "Object/VDInventoryInfo.h"
#include "System/VDInventorySubSystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "System/VDPlayerSubsystem.h"
#include "Public/VDConstrants.h"
#include "Public/VDEquipType.h"

void UVDInventoryLeftItemDetailSection::UpdateItemDetailInfo(UVDInventoryInfo* InInventoryInfo)
{
	switch (InInventoryInfo->GetItemType())
	{
		case EVDItemType::Weapon:
		{
			UseButtonText->SetText(VDConstants::GetInventoryEquipButtonText());
		}
		break;
		case EVDItemType::Consumable:
		{
			UseButtonText->SetText(VDConstants::GetInventoryUseButtonText());
		}
		break;
	}
}

void UVDInventoryLeftItemDetailSection::OnClickedUseButton()
{
	switch(CurrentInventoryInfo->GetItemType())
	{
		UVDPlayerSubsystem* PlayerSubsystem = GetGameInstance()->GetSubsystem<UVDPlayerSubsystem>();
		case EVDItemType::Weapon:
		{
			PlayerSubsystem->SetPlayerEquippedItem(EVDEquipType::Weapon, CurrentInventoryInfo->GetItemID());
		}
		case EVDItemType::Equipment:
		{
			EVDEquipType EquipType = EVDEquipType::HeadArmor;
			int32 EquipID = 1;
			// TODO :: 추후 테이블 추가시 ID로 해당 테이블 정보 읽어서 장비 타입 분기 필요
			// TODO :: 장비 장착 로직
			PlayerSubsystem->SetPlayerEquippedItem(EquipType, EquipID);
		}
		break;
		case EVDItemType::Consumable:
		{
			// TODO :: 소비 아이템 사용 로직
			PlayerSubsystem->SetUseConsumeableItem(CurrentInventoryInfo->GetItemID());
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
}

void UVDInventoryLeftItemDetailSection::OnInventoryChanged(UVDInventoryInfo* ChangedItem)
{
	if (CurrentInventoryInfo == ChangedItem)
	{
		if (ChangedItem->GetIsEmpty())
		{
			CurrentInventoryInfo = nullptr;
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UVDInventoryLeftItemDetailSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UVDInventorySubSystem* InventorySubSystem = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();
	if (InventorySubSystem)
	{
		InventorySubSystem->OnInventoryChanged().AddUObject(this, &UVDInventoryLeftItemDetailSection::OnInventoryChanged);
	}

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
