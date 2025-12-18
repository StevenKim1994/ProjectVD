// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::UpdateEnemyHealthBar(float CurrentHP , float MaxHP)
{
	const float NewPercent = FMath::Clamp(CurrentHP / MaxHP, 0.f, 1.f);

	if (HealthBar)
	{
		HealthBar->SetPercent(NewPercent);
	}

	if (HealthBarText)
	{
		HealthBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));
	}

	if (HealthBarTweenBar)
	{
		const float CurrentTweenPercent = HealthBarTweenBar->GetPercent();

		if (FMath::IsNearlyEqual(CurrentTweenPercent, NewPercent))
		{
			HPTween.bIsPlaying = false;
			if (HPTickerHandle.IsValid())
			{
				FTSTicker::GetCoreTicker().RemoveTicker(HPTickerHandle);
				HPTickerHandle.Reset();
			}
			return;
		}

		HPTween.StartPercent = CurrentTweenPercent;
		HPTween.TargetPercent = NewPercent;
		HPTween.ElapsedTime = 0.0f;
		HPTween.bIsPlaying = true;
		HPTween.LastRealTime = -1.0; // DESC :: 실제 시간 추적 초기화

		HPTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVDPlayerHUDEnemyStatusWidget::TickHealthBarTween));
	}

}

bool UVDPlayerHUDEnemyStatusWidget::TickHealthBarTween(float DeltaTime)
{
	if (!HPTween.bIsPlaying || HPTween.DurationTime <= 0.0f)
	{
		return false;
	}

	// DESC :: Global Time Dilation 영향을 받지 않도록 실제 경과 시간 사용
	const double CurrentRealTime = FPlatformTime::Seconds();
	if (HPTween.LastRealTime < 0.0)
	{
		HPTween.LastRealTime = CurrentRealTime;
	}

	const float UnscaledDeltaTime = static_cast<float>(CurrentRealTime - HPTween.LastRealTime);

	HPTween.LastRealTime = CurrentRealTime;

	HPTween.ElapsedTime += UnscaledDeltaTime;

	float RawAlpha = HPTween.ElapsedTime / HPTween.DurationTime;
	RawAlpha = FMath::Clamp(RawAlpha, 0.0f, 1.0f);

	float EaseAlpha = RawAlpha;

	const float NewValue = FMath::Lerp(HPTween.StartPercent, HPTween.TargetPercent, EaseAlpha);
	HealthBarTweenBar->SetPercent(NewValue);

	if (RawAlpha >= 1.0f)
	{
		HPTween.bIsPlaying = false;
		HPTween.LastRealTime = -1.0;
		HealthBarTweenBar->SetPercent(HPTween.TargetPercent);
		return false;
	}

	return true;
}

void UVDPlayerHUDEnemyStatusWidget::SetTargetEnemy(AVDEnemyCharacterBase* Enemy)
{
	if (TargetEnemy.UEOpEquals(Enemy))
	{
		return;
	}

	if (TargetEnemy.IsValid())
	{
		if (UVDEnemyStatsBaseComponent* PrevStats = TargetEnemy->GetBaseStatsComponent())
		{
			PrevStats->GetOnChangeHealth().RemoveAll(this);
		}
	}

	TargetEnemy = Enemy;

	if (TargetEnemy == nullptr)
	{
		if (NameText)
		{
			NameText->SetText(FText::GetEmpty());
		}
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (UVDEnemyStatsBaseComponent* NewStats = TargetEnemy->GetBaseStatsComponent())
	{
		NewStats->GetOnChangeHealth().RemoveAll(this);
		NewStats->GetOnChangeHealth().AddUObject(this, &UVDPlayerHUDEnemyStatusWidget::UpdateEnemyHealthBar);
	}

	if (NameText)
	{
		if (TargetEnemy.IsValid())
		{
			NameText->SetText(FText::FromName(TargetEnemy->GetEnemyName()));
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (HealthBar)
	{

		if (UVDEnemyStatsBaseComponent* StatsComp = TargetEnemy->GetBaseStatsComponent())
		{
			if (StatsComp->GetMaxHealth() > 0.f)
			{
				UpdateEnemyHealthBar(StatsComp->GetHealth(), StatsComp->GetMaxHealth());

				
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
	if (TargetEnemy.IsValid())
	{
		if (UVDEnemyStatsBaseComponent* Stats = TargetEnemy->GetBaseStatsComponent())
		{
			Stats->GetOnChangeHealth().RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void UVDPlayerHUDEnemyStatusWidget::OnEnemyHPHideTimerExpired()
{
	SetVisibility(ESlateVisibility::Collapsed);
	FTSTicker::GetCoreTicker().RemoveTicker(BossHPHideTickerHandle);
	BossHPHideTickerHandle.Reset();
}
