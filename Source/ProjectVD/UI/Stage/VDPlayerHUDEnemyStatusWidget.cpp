// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar(float CurrentHP , float MaxHP)
{
	if (BossHealthBar)
	{
		TargetBossHPPercent = CurrentHP / MaxHP;
		bIsBossHPTweenPlaying = true;
		//BossHealthBar->SetPercent(CurrentHP / MaxHP);

		if (BossHealthBarText)
		{
			BossHealthBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"),
				CurrentHP,
				MaxHP)));
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
		if (UVDEnemyStatsBaseComponent* PrevStats = BossActor->GetBaseStatsComponent())
		{
			PrevStats->GetOnChangeHealth().RemoveAll(this);
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

	if (UVDEnemyStatsBaseComponent* NewStats = BossActor->GetBaseStatsComponent())
	{
		NewStats->GetOnChangeHealth().RemoveAll(this);
		NewStats->GetOnChangeHealth().AddUObject(this, &UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar);
	}

	if (BossNameText)
	{
		if (BossActor.IsValid())
		{
			BossNameText->SetText(FText::FromName(BossActor->GetEnemyName()));
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (BossHealthBar)
	{
		if (UVDEnemyStatsBaseComponent* StatsComp = BossActor->GetBaseStatsComponent())
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
			else
			{
				SetVisibility(ESlateVisibility::Hidden);
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
	// 위젯 파괴 시 바인딩 정리
	if (BossActor.IsValid())
	{
		if (UVDEnemyStatsBaseComponent* Stats = BossActor->GetBaseStatsComponent())
		{
			Stats->GetOnChangeHealth().RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void UVDPlayerHUDEnemyStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsBossHPTweenPlaying)
	{
		if (BossHealthBar)
		{
			float CurrentPercent = BossHealthBar->GetPercent();
			float NewPercent = FMath::FInterpTo(CurrentPercent, TargetBossHPPercent, InDeltaTime, 5.0f);
			BossHealthBar->SetPercent(NewPercent);
			if (FMath::IsNearlyEqual(NewPercent, TargetBossHPPercent, 0.001f))
			{
				BossHealthBar->SetPercent(TargetBossHPPercent);
				bIsBossHPTweenPlaying = false;
			}
		}
	}
}
