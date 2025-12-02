// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventoryLeftItemDetailSection.h"
#include "Object/VDInventoryInfo.h"
#include "System/VDInventorySubSystem.h"
#include "System/VDPlayerSubsystem.h"
#include "System/VDUISubsystem.h"
#include "UI/Global/VDConfirmPopup.h"
#include "System/VDDataTableSubSystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Public/VDConstrants.h"
#include "Public/VDEquipType.h"
#include "DataTable/VDItemInfoTable.h"

void UVDInventoryLeftItemDetailSection::UpdateItemDetailInfo(UVDInventoryInfo* InInventoryInfo)
{
	UVDDataTableSubSystem* DataTableSubsystem = GetGameInstance()->GetSubsystem<UVDDataTableSubSystem>();
	FVDItemInfoTable* Table = DataTableSubsystem->GetDataTableRow<FVDItemInfoTable>(FName(TEXT("ItemInfo")), InInventoryInfo->GetItemID());

	ItemName->SetText(FText::FromName(InInventoryInfo->GetItemID()));
	ItemDesc->SetText(FText::FromString(Table->ItemDescription));

	// DESC :: 아이템 타입에 따른 버튼 텍스트 변경 (장비, 무기 : 장착 , 소비아이템 : 사용)
	switch (InInventoryInfo->GetItemType())
	{
		case EVDItemType::Weapon:
		case EVDItemType::Equipment:
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
	UGameInstance* GI = GetGameInstance(); 
	UVDPlayerSubsystem* PlayerSubsystem = GI->GetSubsystem<UVDPlayerSubsystem>();
	UVDInventorySubSystem* InventorySubSystem = GI->GetSubsystem<UVDInventorySubSystem>();
	switch(CurrentInventoryInfo->GetItemType())
	{
		case EVDItemType::Weapon:
		{
			PlayerSubsystem->SetPlayerEquippedItem (EVDEquipType::Weapon, CurrentInventoryInfo->GetItemID());
		}
		break;
		case EVDItemType::Equipment:
		{
			EVDEquipType EquipType = EVDEquipType::HeadArmor;
			FName EquipID = FName();
			// TODO :: 추후 테이블 추가시 ID로 해당 테이블 정보 읽어서 장비 타입 분기 필요
			// TODO :: 장비 장착 로직
			//PlayerSubsystem->SetPlayerEquippedItem(EquipType, EquipID);
		}
		break;
		case EVDItemType::Consumable:
		{
			// TODO :: 소비아이템 테이블에서 GetItemID로 읽어와서 사용 로직 처리
			//PlayerSubsystem->SetUseConsumeableItem(CurrentInventoryInfo->GetItemID());
		}
		break;
	}

	InventorySubSystem->RemoveInventoryItemBySlot(CurrentInventoryInfo->GetSlot());
}

void UVDInventoryLeftItemDetailSection::OnClickedDeleteButton()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance == nullptr)
	{
		return;
	}
	UVDUISubsystem* UISubSystem = GameInstance->GetSubsystem<UVDUISubsystem>();
	if (UISubSystem == nullptr)
	{
		return;
	}

	UVDConfirmPopup* ConfirmPopup = Cast<UVDConfirmPopup>(UISubSystem->ShowUIWidget(VDConstants::ConfirmPopupWidget));
	if (ConfirmPopup && CurrentInventoryInfo.IsValid())
	{
		const FText DescriptionText = FText::Format(
			NSLOCTEXT("Inventory", "DestroyItemConfirmDescription", "정말로 {0} 아이템을 파괴하시겠습니까?"),
			FText::FromName(CurrentInventoryInfo.Get()->GetItemID())
		);

		ConfirmPopup
			->SetDescriptionText(DescriptionText)
			->SetTitleText(NSLOCTEXT("Inventory", "DestroyItemTitle", "아이템 파괴"))
			->SetConfirmButtonText(NSLOCTEXT("Inventory", "DestroyItemConfirmButton", "파괴"))
			->SetCancelButtonText(NSLOCTEXT("Inventory", "DestroyItemCancelButton", "취소"));

		ConfirmPopup->OnClickEvent().BindLambda([this](EConfirmPopupResult Result)
		{
			if (Result == EConfirmPopupResult::Confirm)
			{
				UVDInventorySubSystem* InventorySubSystem = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();
				if (InventorySubSystem && CurrentInventoryInfo.IsValid())
				{
					InventorySubSystem->RemoveInventoryItemBySlot(CurrentInventoryInfo->GetSlot());
				}
			}
		});
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
