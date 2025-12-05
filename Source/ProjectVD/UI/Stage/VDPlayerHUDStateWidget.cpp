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
	HPBar->SetPercent(CurrentHP / MaxHP);
	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));
}

void UVDPlayerHUDStateWidget::SetMPBarPercent(float CurrentMP, float MaxMP)
{

}
