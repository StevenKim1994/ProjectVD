// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/Inventory/VDInventoryPanel.h"
#include "UI/Stage/Inventory/VDInventoryLeftItemDetailSection.h"
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

	if (InventoryTileView)
	{
		auto& InventoryMap = GetGameInstance()->GetSubsystem<UVDInventorySubSystem>()->GetInventoryMap(); // DESC :: 일단 슬롯 갯수만큼 생성
		for (auto& Pair : InventoryMap)
		{
			InventoryTileView->AddItem(Pair.Value);
		}

		InventoryTileView->OnItemIsHoveredChanged().AddUObject(this, &UVDInventoryPanel::OnInventoryItemHoveredChanged);
	}
}

void UVDInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryTileView)
	{
		float TileViewWidth = InventoryTileView->GetCachedGeometry().GetLocalSize().X;
		InventoryTileView->SetEntryWidth(125);
		InventoryTileView->SetEntryHeight(InventoryTileView->GetEntryWidth()); 
	}

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
			if (InventoryInfo->GetIsEmpty()) // DESC :: 빈 슬롯일 경우
			{
				ItemDetailSection->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}

			if (bIsHovered)
			{
				ItemDetailSection->SetVisibility(ESlateVisibility::Visible);
				PlayAnimation(LeftAnim);
			}
			else
			{
				ItemDetailSection->SetVisibility(ESlateVisibility::Collapsed);
				//PlayAnimation(LeftAnim, LeftAnim->GetEndTime(), 1, EUMGSequencePlayMode::Reverse);
			}
		}
	}
}
