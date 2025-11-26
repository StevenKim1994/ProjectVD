// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventoryPanel.h"
#include "UI/Stage/Inventory/VDInventoryLeftItemDetailSection.h"
#include "UI/Stage/Inventory/VDInventorySlotWidget.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "System/VDInventorySubSystem.h"
#include "System/VDUISubsystem.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Object/VDInventoryInfo.h"
#include "Public/VDConstrants.h"

void UVDInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UVDInventoryPanel::OnClickCloseButton);
	}

	UVDInventorySubSystem* InventorySubSystem = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();
	
	if (InventoryTileView && InventorySubSystem)
	{
		InventoryTileView->SetEntryWidth(125);
		InventoryTileView->SetEntryHeight(InventoryTileView->GetEntryWidth());
		
		const auto& InventoryMap = InventorySubSystem->GetInventoryMap();
		for (auto& Pair : InventoryMap)
		{
			InventoryTileView->AddItem(Pair.Value);
		}

		InventoryTileView->SetSelectionMode(ESelectionMode::Single);
		InventoryTileView->OnItemIsHoveredChanged().AddUObject(this, &UVDInventoryPanel::OnInventoryItemHoveredChanged);
		InventoryTileView->OnItemClicked().AddUObject(this, &UVDInventoryPanel::OnInventoryItemClicked);
		
		// DESC :: 서브시스템의 변경 이벤트에 바인딩
		InventorySubSystem->OnInventoryChanged().AddUObject(this, &UVDInventoryPanel::OnChangeInventorySubsystemChanged);
	}
}

void UVDInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryTileView->RequestRefresh();
	ItemDetailSection->SetVisibility(ESlateVisibility::Collapsed);
	PlayAnimation(RightAnim);
}

void UVDInventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDInventoryPanel::OnClickCloseButton()
{
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		AVDStagePlayerController* Controller = Cast<AVDStagePlayerController>(GI->GetFirstLocalPlayerController());
		if (Controller)
		{
			Controller->ChangeToggleInputContext();
			GI->GetSubsystem<UVDUISubsystem>()->HideUIWidget(this);
		}
	}
}

void UVDInventoryPanel::OnInventoryItemHoveredChanged(UObject* Item, bool bIsHovered)
{
	if (Item)
	{
		UVDInventoryInfo* InventoryInfo = Cast<UVDInventoryInfo>(Item);
		if (InventoryInfo)
		{
			// TODO :: 툴팁출력
			 
			//if (InventoryInfo->GetIsEmpty()) // DESC :: 빈 슬롯일 경우
			//{
			//	ItemDetailSection->SetVisibility(ESlateVisibility::Collapsed);
			//	return;
			//}
			//
			//if (bIsHovered)
			//{
			//	ItemDetailSection->SetVisibility(ESlateVisibility::Visible);
			//	PlayAnimation(LeftAnim);
			//}
			//else
			//{
			//	ItemDetailSection->SetVisibility(ESlateVisibility::Collapsed);
			//	//PlayAnimation(LeftAnim, LeftAnim->GetEndTime(), 1, EUMGSequencePlayMode::Reverse);
			//}
		}
	}
}

void UVDInventoryPanel::OnInventoryItemClicked(UObject* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("UVDInventoryPanel::OnInventoryItemClicked"));
	if (Item)
	{
		UVDInventoryInfo* InventoryInfo = Cast<UVDInventoryInfo>(Item);
		if (InventoryInfo)
		{
			if (InventoryInfo->GetIsEmpty()) // DESC :: 빈 슬롯일 경우
			{
				return;
			}

			ItemDetailSection->SetVisibility(ESlateVisibility::Visible);
			ItemDetailSection->SetItemDetailInfo(InventoryInfo);
		}

		UVDInventorySlotWidget* ItemSlotWidget = Cast<UVDInventorySlotWidget>(InventoryTileView->GetEntryWidgetFromItem(Item));
		if (ItemSlotWidget)
		{
			//ItemSlotWidget->SetIsSelected(true);
		}
	}
}

void UVDInventoryPanel::OnChangeInventorySubsystemChanged(UVDInventoryInfo* ChangedItem)
{
    if (InventoryTileView)
    {
        InventoryTileView->RequestRefresh(); 
    }
}
