// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDStagePlayerHUDWidget.h"
#include "UI/Stage/VDStagePlayerHUDToastWidget.h"
#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"

void UVDStagePlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVDStagePlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDStagePlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDStagePlayerHUDWidget::ShowToast(const FString& InTitle, const FString& InMessage)
{
	if (Toast)
	{
		Toast->ShowToast(InTitle, InMessage);
	}
}

void UVDStagePlayerHUDWidget::ShowBossStatus(AActor* Boss)
{
	if(BossState)
	{
		BossState->SetBossActor(Boss);
		BossState->SetVisibility(ESlateVisibility::Visible);
	}
}

void UVDStagePlayerHUDWidget::HideBossStatus()
{
	if(BossState)
	{
		BossState->SetVisibility(ESlateVisibility::Collapsed);
	}
}
