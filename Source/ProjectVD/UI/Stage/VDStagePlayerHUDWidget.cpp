// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDStagePlayerHUDWidget.h"
#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "UI/Stage/VDHUDStaminaWidget.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

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

void UVDStagePlayerHUDWidget::ShowBossStatus(AVDEnemyCharacterBase* Boss)
{
	if(BossState)
	{
		BossState->SetBossActor(Boss);
		BossState->SetVisibility(ESlateVisibility::Visible);
	}
}

void UVDStagePlayerHUDWidget::SetCharacter(AVDCharacterBase* Character)
{
	AVDCharacterBase* PlayerCharacter = Character;

	if (PlayerCharacter)
	{
		UVDBaseStaminaComponent* StaminaComp = Cast<UVDBaseStaminaComponent>(PlayerCharacter->GetComponentByClass(UVDBaseStaminaComponent::StaticClass()));
		if (StaminaComp)
		{
			StaminaIndicator->SetStaminaComp(StaminaComp);
			StaminaIndicator->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			StaminaIndicator->SetVisibility(ESlateVisibility::Collapsed);
		}

		UVDCharacterStatsBaseComponent* StatsComp = Cast<UVDCharacterStatsBaseComponent>(PlayerCharacter->GetComponentByClass(UVDCharacterStatsBaseComponent::StaticClass()));
		if (StatsComp)
		{
			State->SetCharacterState(StatsComp);
			State->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			State->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UVDStagePlayerHUDWidget::HideBossStatus()
{
	if(BossState)
	{
		BossState->SetVisibility(ESlateVisibility::Collapsed);
	}
}
