// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDStagePlayerHUDWidget.h"
#include "UI/Stage/VDStagePlayerHUDToastWidget.h"
#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"

void UVDStagePlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeWidget();
}

void UVDStagePlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDStagePlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDStagePlayerHUDWidget::InitializeWidget()
{
	if(BossState)
	{
		BossState->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UVDStagePlayerHUDWidget::ShowPerformanceTween()
{
	if (State)
	{
		State->ShowPerformanceTween();
	}
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

void UVDStagePlayerHUDWidget::SetCharacterState(UVDCharacterStatsBaseComponent* StatsBaseComponent)
{
	if(State)
	{
		State->SetCharacterState(StatsBaseComponent);
	}
}

void UVDStagePlayerHUDWidget::HideBossStatus()
{
	if(BossState)
	{
		BossState->SetVisibility(ESlateVisibility::Collapsed);
	}
}
