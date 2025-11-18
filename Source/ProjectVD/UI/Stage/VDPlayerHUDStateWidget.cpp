// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Stage/VDPlayerHUDStateWidget.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"

void UVDPlayerHUDStateWidget::SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats)
{
	PlayerName->SetText(FText::FromString(BaseStats->GetOwner()->GetName()));

	HPBar->SetPercent(BaseStats->GetHealth() / BaseStats->GetMaxHealth());
	HPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), BaseStats->GetHealth(), BaseStats->GetMaxHealth())));

	MPBar->SetPercent(Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMana() / Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMaxMana());
	MPBarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMana(), Cast<UVDCharacterStatsBaseComponent>(BaseStats)->GetMaxMana())));
}
