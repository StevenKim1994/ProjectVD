// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar(UVDBaseStatsComponent* BossStats)
{
	if (BossHealthBar && BossStats && BossStats->GetMaxHealth() > 0.f)
	{
		BossHealthBar->SetPercent(BossStats->GetHealth() / BossStats->GetMaxHealth());

		if (BossHealthBarText)
		{
			BossHealthBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"),
				BossStats->GetHealth(),
				BossStats->GetMaxHealth())));
		}
	}
}

void UVDPlayerHUDEnemyStatusWidget::SetBossActor(AVDEnemyCharacterBase* Boss)
{
	if (BossActor == Boss)
	{
		return;
	}

	if (BossActor.IsValid())
	{
		if (UVDCharacterStatsBaseComponent* PrevStats = BossActor->GetBaseStatsComponent())
		{
			// 참조 반환이므로 '->'가 아니라 '.'
			PrevStats->GetOnChangeHealth().RemoveDynamic(this, &UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar);
		}
	}

	BossActor = Boss;

	if (Boss == nullptr)
	{
		if (BossNameText)
		{
			BossNameText->SetText(FText::GetEmpty());
		}
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (UVDCharacterStatsBaseComponent* NewStats = BossActor->GetBaseStatsComponent())
	{
		// 중복 바인딩 방지
		NewStats->GetOnChangeHealth().RemoveDynamic(this, &UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar);
		NewStats->GetOnChangeHealth().AddDynamic(this, &UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar);
	}

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
		if (UVDCharacterStatsBaseComponent* StatsComp = BossActor->GetBaseStatsComponent())
		{
			if (StatsComp->GetMaxHealth() > 0.f)
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
