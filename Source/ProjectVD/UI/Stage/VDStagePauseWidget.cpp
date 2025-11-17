// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDStagePauseWidget.h"
#include "Components/Button.h"

void UVDStagePauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UVDStagePauseWidget::OnClickResume);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UVDStagePauseWidget::OnClickOptions);
	}

	if (ExitToTitleButton)
	{
		ExitToTitleButton->OnClicked.AddDynamic(this, &UVDStagePauseWidget::OnClickExitToTitle);
	}
}

void UVDStagePauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDStagePauseWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDStagePauseWidget::OnClickResume()
{
	if (OnPauseMenuButtonClickedDelegate.IsBound())
	{
		OnPauseMenuButtonClickedDelegate.Execute(PauseMenuButtonEnum::Resume);
	}
}

void UVDStagePauseWidget::OnClickOptions()
{
	if (OnPauseMenuButtonClickedDelegate.IsBound())
	{
		OnPauseMenuButtonClickedDelegate.Execute(PauseMenuButtonEnum::Options);
	}
}

void UVDStagePauseWidget::OnClickExitToTitle()
{
	if (OnPauseMenuButtonClickedDelegate.IsBound())
	{
		OnPauseMenuButtonClickedDelegate.Execute(PauseMenuButtonEnum::ExitToTitle);
	}
}
