// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Title/VDTitlePanelUserWidget.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"	
#include "System/VDLevelSystem.h"
#include "Public/VDConstrants.h"
#include "Containers/Ticker.h"        
#include "Components/CanvasPanelSlot.h" 
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CheckBox.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationState.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Animation/UMGSequencePlayer.h"
#include "MediaTexture.h"
#include "Styling/SlateBrush.h"

void UVDTitlePanelUserWidget::OnClickStartButton()
{
	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();
	if (GI)
	{
		GI->GetSubsystem<UVDLevelSystem>()->ChangeLevelByName(TEXT("Stage")); 
		GI->GetSubsystem<UVDUISubsystem>()->HideUIWidget(VDConstants::TitlePanel);
	}
}

void UVDTitlePanelUserWidget::OnClickOptionButton()
{
	if (ButtonsParentBox)
	{
		bIsMainMenuButtonToggledOn = false;
		OptionsParentBox->SetVisibility(ESlateVisibility::Visible);
		OnMainButtonToggle(bIsMainMenuButtonToggledOn);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ButtonsParentsBox is nullptr"));
	}
}

void UVDTitlePanelUserWidget::OnClickExitButton()
{
	if (OnClickExitButtonEvent.IsBound())
	{
		OnClickExitButtonEvent.Execute();
	}
}

void UVDTitlePanelUserWidget::OnClickOptionsBackButton()
{
	if (OptionsParentBox)
	{
		bIsMainMenuButtonToggledOn = true;
		ButtonsParentBox->SetVisibility(ESlateVisibility::Visible);
		OnMainButtonToggle(bIsMainMenuButtonToggledOn);

	}
}

void UVDTitlePanelUserWidget::OnToggleTitleMovieMute(bool ChangedToggleValue)
{
	if (OnToggleTitleMovieMuteEvent.IsBound())
	{
		OnToggleTitleMovieMuteEvent.Execute(ChangedToggleValue);
	}
}

void UVDTitlePanelUserWidget::OnHoverExitButton()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered!"));
}

void UVDTitlePanelUserWidget::SetBackgroundMediaTexture(UMediaTexture* Texture)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Texture);
	MediaPlayerImage->SetBrush(Brush);
}

void UVDTitlePanelUserWidget::OnMainButtonToggle(bool IsOn)
{
	if (IsOn)
	{
		PlayAnimationReverse(OptionTween);
		PlayAnimation(MenuTween);
	}
	else
	{
		PlayAnimationReverse(MenuTween);
		PlayAnimation(OptionTween);
	}
}

void UVDTitlePanelUserWidget::OnChangedMenuStateTweenComplete()
{
	ButtonsParentBox->SetVisibility(bIsMainMenuButtonToggledOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	OptionsParentBox->SetVisibility(bIsMainMenuButtonToggledOn ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UVDTitlePanelUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FWidgetAnimationDynamicEvent EndDelegate;
	EndDelegate.BindDynamic(this, &UVDTitlePanelUserWidget::OnChangedMenuStateTweenComplete);
	BindToAnimationFinished(MenuTween, EndDelegate);
	BindToAnimationFinished(OptionTween, EndDelegate);
	bIsMainMenuButtonToggledOn = true;

	if (GameTitleName)
	{
		GameTitleName->SetText(VDConstants::TitleProjectName);
	}

	if (StartButton)
	{
		UTextBlock* StartButtonText = Cast<UTextBlock>(StartButton->GetChildAt(0));
		if (StartButtonText)
		{
			StartButtonText->SetText(VDConstants::GetTitleStartButtonText());
		}

		StartButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickStartButton);
	}

	if (OptionButton)
	{
		UTextBlock* OptionButtonText = Cast<UTextBlock>(OptionButton->GetChildAt(0));
		if (OptionButtonText)
		{
			OptionButtonText->SetText(VDConstants::GetTitleOptionButtonText());
		}
		OptionButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickOptionButton);
	}

	if (ExitButton)
	{
		UTextBlock* ExitButtonText = Cast<UTextBlock>(ExitButton->GetChildAt(0));
		if (ExitButtonText)
		{
			ExitButtonText->SetText(VDConstants::GetTitleExitButtonText());
		}
		ExitButton->OnHovered.AddDynamic(this, &UVDTitlePanelUserWidget::OnHoverExitButton);
		ExitButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickExitButton);
	}

	if (OptionsParentBox)
	{
		OptionsParentBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OptionsBackButton)
	{
		OptionsBackButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickOptionsBackButton);
	}

	if (TitleMovieMuteToggle)
	{
		UTextBlock* ToggleText = Cast<UTextBlock>(TitleMovieMuteToggle->GetChildAt(0));
		if (ToggleText)
		{
			ToggleText->SetText(VDConstants::GetTitleSoundMuteToggleText());
		}
		TitleMovieMuteToggle->OnCheckStateChanged.AddDynamic(this, &UVDTitlePanelUserWidget::OnToggleTitleMovieMute);
		TitleMovieMuteToggle->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UVDTitlePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OptionsParentBox->SetVisibility(ESlateVisibility::Collapsed);
	ButtonsParentBox->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(MenuTween);
}

void UVDTitlePanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDTitlePanelUserWidget::SetToggleBackgroundSound(bool IsOn)
{

}
