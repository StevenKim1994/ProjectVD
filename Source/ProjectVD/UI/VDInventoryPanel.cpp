// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VDInventoryPanel.h"
#include "Components/TileView.h"
#include "Components/Button.h"
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
	}
}

void UVDInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
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
