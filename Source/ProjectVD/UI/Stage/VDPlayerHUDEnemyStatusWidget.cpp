// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDPlayerHUDEnemyStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UVDPlayerHUDEnemyStatusWidget::SetBossActor(AActor* Boss)
{
	BossActor = Boss;
	if (BossActor.IsValid())
	{
		// TODO :: 보스인지 캐스팅해서 처리해야함
		BossNameText->SetText(FText::FromString(BossActor->GetName()));
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
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
	Super::NativeDestruct();
}
