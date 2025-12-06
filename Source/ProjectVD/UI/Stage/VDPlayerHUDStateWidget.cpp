// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDPlayerHUDStateWidget::ShowPerformanceTween()
{

}

void UVDPlayerHUDStateWidget::SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats)
{
	PlayerName->SetVisibility(ESlateVisibility::Collapsed); // 임시로 이름 숨김
	HPBar->SetPercent(BaseStats->GetHealth() / BaseStats->GetMaxHealth());
	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), BaseStats->GetHealth(), BaseStats->GetMaxHealth())));

	if (UVDCharacterStatsBaseComponent* CastStats = Cast<UVDCharacterStatsBaseComponent>(BaseStats))
	{
		MPBar->SetPercent(CastStats->GetMana() / CastStats->GetMaxMana());
		MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CastStats->GetMana(), CastStats->GetMaxMana())));

		CastStats->GetOnChangeHealth().AddUObject(this, &UVDPlayerHUDStateWidget::SetHPBarPercent);
	}
}

void UVDPlayerHUDStateWidget::SetHPBarPercent(float CurrentHP, float MaxHP)
{
	TargetHPPercent = CurrentHP / MaxHP;
	bIsHPTweenPlaying = true;

	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));
}



void UVDPlayerHUDStateWidget::SetMPBarPercent(float CurrentMP, float MaxMP)
{
	TargetMPPercent = CurrentMP / MaxMP;
	bIsMPTweenPlaying = true;

	MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));
}

void UVDPlayerHUDStateWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIsHPTweenPlaying)
	{
		float CurrentPercent = HPBar->GetPercent();
		float NewPercent = FMath::FInterpTo(CurrentPercent, TargetHPPercent, InDeltaTime, 5.0f);
		HPBar->SetPercent(NewPercent);
		if (FMath::IsNearlyEqual(NewPercent, TargetHPPercent, 0.001f))
		{
			HPBar->SetPercent(TargetHPPercent);
			bIsHPTweenPlaying = false;
		}
	}
	if (bIsMPTweenPlaying)
	{
		float CurrentPercent = MPBar->GetPercent();
		float NewPercent = FMath::FInterpTo(CurrentPercent, TargetMPPercent, InDeltaTime, 5.0f);
		MPBar->SetPercent(NewPercent);
		if (FMath::IsNearlyEqual(NewPercent, TargetMPPercent, 0.001f))
		{
			MPBar->SetPercent(TargetMPPercent);
			bIsMPTweenPlaying = false;
		}
	}
}
