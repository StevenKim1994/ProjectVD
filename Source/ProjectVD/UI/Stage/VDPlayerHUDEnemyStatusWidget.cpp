// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::SetBossActor(AVDEnemyCharacterBase* Boss)
{
	BossActor = Boss;

	if (BossNameText)
	{
		if (BossActor.IsValid())
		{
			BossNameText->SetText(FText::FromName(BossActor->GetEnemyName()));
		}
		else
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (BossHealthBar)
	{
		UVDCharacterStatsBaseComponent* StatsComp = BossActor->GetBaseStatsComponent();
		if (StatsComp)
		{
		   BossHealthBar->SetPercent(StatsComp->GetHealth() / StatsComp->GetMaxHealth());

			if (BossHealthBarText)
			{
				BossHealthBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), 
					StatsComp->GetHealth(), 
					StatsComp->GetMaxHealth())));
			}
		}
	}
}

void UVDPlayerHUDEnemyStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVDPlayerHUDEnemyStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDPlayerHUDEnemyStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
