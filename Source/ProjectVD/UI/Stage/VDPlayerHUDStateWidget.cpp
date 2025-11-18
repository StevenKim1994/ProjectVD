// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDPlayerHUDStateWidget::ShowPerformanceTween()
{
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime)
	{
		float CurrentHPPercent = HPBar->GetPercent();
		float TargetHPPercent = HPBar->GetPercent();
		float NewHPPercent = FMath::FInterpTo(CurrentHPPercent, TargetHPPercent, DeltaTime, 3.0f);
		HPBar->SetPercent(NewHPPercent);
		float CurrentMPPercent = MPBar->GetPercent();
		float TargetMPPercent = MPBar->GetPercent();
		float NewMPPercent = FMath::FInterpTo(CurrentMPPercent, TargetMPPercent, DeltaTime, 3.0f);
		MPBar->SetPercent(NewMPPercent);
		bool bIsHPFinished = FMath::IsNearlyEqual(NewHPPercent, TargetHPPercent, KINDA_SMALL_NUMBER);
		bool bIsMPFinished = FMath::IsNearlyEqual(NewMPPercent, TargetMPPercent, KINDA_SMALL_NUMBER);
		return !(bIsHPFinished && bIsMPFinished);
	}), 0.0f);
}

void UVDPlayerHUDStateWidget::SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats)
{
	PlayerName->SetText(FText::FromString(BaseStats->GetOwner()->GetName()));

	HPBar->SetPercent(BaseStats->GetHealth() / BaseStats->GetMaxHealth());
	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), BaseStats->GetHealth(), BaseStats->GetMaxHealth())));

	MPBar->SetPercent(Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMana() / Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMaxMana());
	MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMana(), Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMaxMana())));
}
