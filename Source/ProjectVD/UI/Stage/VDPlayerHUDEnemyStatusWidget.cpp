// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDBaseStatsComponent.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar(float CurrentHP , float MaxHP)
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
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		if (FMath::IsNearlyEqual(CurrentTweenPercent, NewPercent))
		{
			bIsHPTweenPlaying = false;
			if (TweenTimerHandle.IsValid())
			{
				World->GetTimerManager().ClearTimer(TweenTimerHandle);
			}
			return;
		}

		TweenStartPercent = CurrentTweenPercent;
		TweenTargetPercent = NewPercent;
		TweenElapsedTime = 0.0f;
		bIsHPTweenPlaying = true;

		World->GetTimerManager().ClearTimer(TweenTimerHandle);
		World->GetTimerManager().SetTimer(TweenTimerHandle,
			this,
			&UVDPlayerHUDEnemyStatusWidget::UpdateTweenBar,
			0.01f,
			true);
	}

}

void UVDPlayerHUDEnemyStatusWidget::UpdateTweenBar()
{
	if (!bIsHPTweenPlaying || TweenDuration <= 0.0f)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TweenTimerHandle);
		}
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();
	TweenElapsedTime += DeltaTime;

	float RawAlpha = TweenElapsedTime / TweenDuration;
	RawAlpha = FMath::Clamp(RawAlpha, 0.0f, 1.0f);

	float EaseAlpha = RawAlpha;

	const float NewValue = FMath::Lerp(TweenStartPercent, TweenTargetPercent, EaseAlpha);
	HealthBarTweenBar->SetPercent(NewValue);

	if (RawAlpha >= 1.0f)
	{
		bIsHPTweenPlaying = false;
		HealthBarTweenBar->SetPercent(TweenTargetPercent);
		World->GetTimerManager().ClearTimer(TweenTimerHandle);
	}
}

void UVDPlayerHUDEnemyStatusWidget::SetTargetEnemy(AVDEnemyCharacterBase* Enemy)
{
	if (TargetEnemy == Enemy)
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
		NewStats->GetOnChangeHealth().AddUObject(this, &UVDPlayerHUDEnemyStatusWidget::UpdateBossHealthBar);
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
				UpdateBossHealthBar(StatsComp->GetHealth(), StatsComp->GetMaxHealth());

				
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

void UVDPlayerHUDEnemyStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UVDPlayerHUDEnemyStatusWidget::OnBossHPHideTimerExpired()
{
	SetVisibility(ESlateVisibility::Collapsed);
	GetWorld()->GetTimerManager().ClearTimer(BossHPVisibleTimerHandle);
}
