// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDPlayerHUDStateWidget::SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats)
{
	PlayerName->SetVisibility(ESlateVisibility::Collapsed); // 임시로 이름 숨김
	HPBar->SetPercent(BaseStats->GetHealth() / BaseStats->GetMaxHealth());
	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), BaseStats->GetHealth(), BaseStats->GetMaxHealth())));
	BaseStats->GetOnChangeHealth().RemoveAll(this);
	BaseStats->GetOnChangeHealth().AddUObject(this, &UVDPlayerHUDStateWidget::SetHPBarPercent);

	if (UVDCharacterStatsBaseComponent* CastStats = Cast<UVDCharacterStatsBaseComponent>(BaseStats))
	{
		MPBar->SetPercent(CastStats->GetMana() / CastStats->GetMaxMana());
		MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CastStats->GetMana(), CastStats->GetMaxMana())));

		CastStats->GetOnChangeMana().RemoveAll(this);
		CastStats->GetOnChangeMana().AddUObject(this, &UVDPlayerHUDStateWidget::SetMPBarPercent);
	}
}

void UVDPlayerHUDStateWidget::SetHPBarPercent(float CurrentHP, float MaxHP)
{
	const float NewPercent = FMath::Clamp(CurrentHP / MaxHP, 0.0f, 1.0f);

	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float CurrentTweenPercent = HPBarTween->GetPercent();
	if (FMath::IsNearlyEqual( CurrentTweenPercent, NewPercent))
	{
		HPTween.bIsPlaying = false;
		if (HPTweenHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(HPTweenHandle);
		}
		return;
	}

	HPTween.StartPercent = CurrentTweenPercent;
	HPTween.TargetPercent = NewPercent;
	HPTween.ElapsedTime = 0.0f;
	HPTween.bIsPlaying = true;

	World->GetTimerManager().ClearTimer(HPTweenHandle);
	World->GetTimerManager().SetTimer(HPTweenHandle, this, &UVDPlayerHUDStateWidget::UpdateHPBarTween, 0.01f , true);
}



void UVDPlayerHUDStateWidget::SetMPBarPercent(float CurrentMP, float MaxMP)
{
	const float NewPercent = FMath::Clamp(CurrentMP / MaxMP, 0.0f, 1.0f);

	MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float CurrentTweenPercent = MPBarTween->GetPercent();
	if (FMath::IsNearlyEqual(CurrentTweenPercent, NewPercent))
	{
		MPTween.bIsPlaying = false;
		if (MPTweenHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(MPTweenHandle);
		}
		return;
	}

	MPTween.StartPercent = CurrentTweenPercent;
	MPTween.TargetPercent = NewPercent;
	MPTween.ElapsedTime = 0.0f;
	MPTween.bIsPlaying = true;

	World->GetTimerManager().ClearTimer(MPTweenHandle);
	World->GetTimerManager().SetTimer(MPTweenHandle, this, &UVDPlayerHUDStateWidget::UpdateMPBarTween, 0.01f, true);
}

void UVDPlayerHUDStateWidget::UpdateHPBarTween()
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	if (!HPTween.bIsPlaying || HPTween.DurationTime <= 0.0f)
	{
		World->GetTimerManager().ClearTimer(HPTweenHandle);
		return;
	}

	HPTween.ElapsedTime += World->GetDeltaSeconds();
	const float Alpha = FMath::Clamp(HPTween.ElapsedTime / HPTween.DurationTime, 0.0f, 1.0f);
	const float NewPercent = FMath::Lerp(HPTween.StartPercent, HPTween.TargetPercent, Alpha);
	HPBarTween->SetPercent(NewPercent);
	if (Alpha >= 1.0f)
	{
		HPTween.bIsPlaying = false;
		HPBarTween->SetPercent(HPTween.TargetPercent);
		World->GetTimerManager().ClearTimer(HPTweenHandle);
	}
}

void UVDPlayerHUDStateWidget::UpdateMPBarTween()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	if (!MPTween.bIsPlaying || MPTween.DurationTime <= 0.0f)
	{
		World->GetTimerManager().ClearTimer(MPTweenHandle);
		return;
	}
	MPTween.ElapsedTime += World->GetDeltaSeconds();
	const float Alpha = FMath::Clamp(MPTween.ElapsedTime / MPTween.DurationTime, 0.0f, 1.0f);
	const float NewPercent = FMath::Lerp(MPTween.StartPercent, MPTween.TargetPercent, Alpha);
	MPBarTween->SetPercent(NewPercent);
	if (Alpha >= 1.0f)
	{
		MPTween.bIsPlaying = false;
		MPBarTween->SetPercent(MPTween.TargetPercent);
		World->GetTimerManager().ClearTimer(MPTweenHandle);
	}
}
