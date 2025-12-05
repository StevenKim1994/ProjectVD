// Fill out your copyright notice in the Description page of Project Settings.


#include "VDStageGameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UVDStageGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GameOverText)
	{
		GameOverText->SetText(FText::FromString(TEXT("Game Over")));
	}

	if (RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UVDStageGameOverWidget::OnRetryButtonClicked);
	}

	if(RetryButtonText)
	{
		RetryButtonText->SetText(FText::FromString(TEXT("Retry")));
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UVDStageGameOverWidget::OnExitButtonClicked);
	}

	if(ExitButtonText)
	{
		ExitButtonText->SetText(FText::FromString(TEXT("Exit")));
	}
}

void UVDStageGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowUpAnim)
	{
		if (IsAnimationPlaying(ShowUpAnim))
		{
			return;
		}
		PlayAnimation(ShowUpAnim);
	}
}

void UVDStageGameOverWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDStageGameOverWidget::OnRetryButtonClicked()
{
	OnRetryButtonClickedDelegate.ExecuteIfBound();
}

void UVDStageGameOverWidget::OnExitButtonClicked()
{
	OnExitButtonClickedDelegate.ExecuteIfBound();
}
