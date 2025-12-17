// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDPlayerHUDStateWidget::SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats)
{
	PlayerName->SetVisibility(ESlateVisibility::Collapsed); // DESC :: 임시로 이름 숨김
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

	const float CurrentTweenPercent = HPBarTween->GetPercent();
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
	HPTween.DurationTime = 3.5f;
	HPTween.LastRealTime = -1.0; // DESC :: 실제 시간 추적 초기화
	HPTween.bIsPlaying = true;

	HPBar->SetPercent(HPTween.TargetPercent);
	
	if (HPTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(HPTickerHandle);
	}
	
	HPTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVDPlayerHUDStateWidget::TickHPBarTween));
}

void UVDPlayerHUDStateWidget::SetMPBarPercent(float CurrentMP, float MaxMP)
{
	const float NewPercent = FMath::Clamp(CurrentMP / MaxMP, 0.0f, 1.0f);

	MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));

	const float CurrentTweenPercent = MPBarTween->GetPercent();
	if (FMath::IsNearlyEqual(CurrentTweenPercent, NewPercent))
	{
		MPTween.bIsPlaying = false;
		if (MPTickerHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(MPTickerHandle);
			MPTickerHandle.Reset();
		}
		return;
	}

	MPTween.StartPercent = CurrentTweenPercent;
	MPTween.TargetPercent = NewPercent;
	MPTween.ElapsedTime = 0.0f;
	MPTween.LastRealTime = -1.0; // DESC :: 실제 시간 추적 초기화
	MPTween.DurationTime = 1.5f;
	MPTween.bIsPlaying = true;
	
	MPBar->SetPercent(MPTween.TargetPercent);
	
	// DESC :: 기존 틱커 제거
	if (MPTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(MPTickerHandle);
	}
	
	// DESC :: 비동기 틱커 등록
	MPTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVDPlayerHUDStateWidget::TickMPBarTween));
}

bool UVDPlayerHUDStateWidget::TickHPBarTween(float DeltaTime)
{
	if (!HPTween.bIsPlaying || HPTween.DurationTime <= 0.0f)
	{
		return false;
	}

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

	const float EasedAlpha = FMath::InterpEaseOut(0.0f, 1.0f, RawAlpha, 3.0f);
	const float NewValue = FMath::Lerp(HPTween.StartPercent, HPTween.TargetPercent, EasedAlpha);
	HPBarTween->SetPercent(NewValue);
	
	if (RawAlpha >= 1.0f)
	{
		HPTween.bIsPlaying = false;
		HPTween.LastRealTime = -1.0;
		HPBarTween->SetPercent(HPTween.TargetPercent);
		return false;
	}

	return true;
}

bool UVDPlayerHUDStateWidget::TickMPBarTween(float DeltaTime)
{
	if (!MPTween.bIsPlaying || MPTween.DurationTime <= 0.0f)
	{
		return false; 
	}
	
	const double CurrentRealTime = FPlatformTime::Seconds();
	if (MPTween.LastRealTime < 0.0)
	{
		MPTween.LastRealTime = CurrentRealTime;
	}

	const float UnscaledDeltaTime = static_cast<float>(CurrentRealTime - MPTween.LastRealTime);
	MPTween.LastRealTime = CurrentRealTime;
	MPTween.ElapsedTime += UnscaledDeltaTime;

	float RawAlpha = MPTween.ElapsedTime / MPTween.DurationTime;
	RawAlpha = FMath::Clamp(RawAlpha, 0.0f, 1.0f);

	float EasedAlpha = FMath::InterpEaseOut(0.0f, 1.0f, RawAlpha, 3.0f);
	const float NewValue = FMath::Lerp(MPTween.StartPercent, MPTween.TargetPercent, EasedAlpha);
	
	MPBarTween->SetPercent(NewValue);
	
	if (RawAlpha >= 1.0f)
	{
		MPTween.bIsPlaying = false;
		MPTween.LastRealTime = -1.0;
		MPBarTween->SetPercent(MPTween.TargetPercent);
		return false; 
	}

	return true; 
}
