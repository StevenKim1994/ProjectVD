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
	HPTween.bIsPlaying = true;

	HPBar->SetPercent(HPTween.TargetPercent);
	
	if (HPTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(HPTickerHandle);
	}
	
	HPTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVDPlayerHUDStateWidget::TickHPBarTween), 0.01f);
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
	MPTween.bIsPlaying = true;
	
	MPBar->SetPercent(MPTween.TargetPercent);
	
	// DESC :: 기존 틱커 제거
	if (MPTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(MPTickerHandle);
	}
	
	// DESC :: 비동기 틱커 등록
	MPTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVDPlayerHUDStateWidget::TickMPBarTween), 0.01f);
}

bool UVDPlayerHUDStateWidget::TickHPBarTween(float DeltaTime)
{
	if (!HPTween.bIsPlaying || HPTween.DurationTime <= 0.0f)
	{
		return false; // DESC :: false 반환 시 자동으로 틱커 제거됨
	}

	HPTween.ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(HPTween.ElapsedTime / HPTween.DurationTime, 0.0f, 1.0f);
	const float NewPercent = FMath::Lerp(HPTween.StartPercent, HPTween.TargetPercent, Alpha);

	UE_LOG(LogTemp, Warning, TEXT("HP Bar Tween Update: NewPercent = %f"), NewPercent);
	HPBarTween->SetPercent(NewPercent);
	
	if (Alpha >= 1.0f)
	{
		HPTween.bIsPlaying = false;
		HPBarTween->SetPercent(HPTween.TargetPercent);
		return false; // DESC :: 완료 시 틱커 제거
	}

	return true; // DESC :: 계속 실행
}

bool UVDPlayerHUDStateWidget::TickMPBarTween(float DeltaTime)
{
	if (!MPTween.bIsPlaying || MPTween.DurationTime <= 0.0f)
	{
		return false; 
	}
	
	MPTween.ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(MPTween.ElapsedTime / MPTween.DurationTime, 0.0f, 1.0f);
	const float NewPercent = FMath::Lerp(MPTween.StartPercent, MPTween.TargetPercent, Alpha);
	
	MPBarTween->SetPercent(NewPercent);
	
	if (Alpha >= 1.0f)
	{
		MPTween.bIsPlaying = false;
		MPBarTween->SetPercent(MPTween.TargetPercent);
		return false; 
	}

	return true; 
}
