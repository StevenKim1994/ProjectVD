// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Title/VDTitlePanelUserWidget.h"
#include "Game/VDGameInstance.h"

void UVDTitlePanelUserWidget::OnClickStartButton()
{
	if(OnClickStartButtonEvent.IsBound())
	{
		OnClickStartButtonEvent.Broadcast();
	}

	UGameInstance* GI = GetGameInstance();
	if (GI != nullptr)
	{
		UVDGameInstance* VDGI = Cast<UVDGameInstance>(GI);
		if (VDGI != nullptr)
		{
			VDGI->GotoInGameLevel("Stage");
		}
	}
}

void UVDTitlePanelUserWidget::OnClickExitButton()
{
	if (OnClickExitButtonEvent.IsBound())
	{
		OnClickExitButtonEvent.Broadcast();
	}
}

void UVDTitlePanelUserWidget::OnToggleTitleMovieMute(bool ChangedToggleValue) 
{
	if (TitleMediaSoundComponent)
	{
		TitleMediaSoundComponent->SetVolumeMultiplier(ChangedToggleValue ? 0.0f : 1.0f);
	}
	if(OnToggleTitleMovieMuteEvent.IsBound())
	{
		OnToggleTitleMovieMuteEvent.Broadcast(ChangedToggleValue);
	}
}

void UVDTitlePanelUserWidget::OnHoverExitButton()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered!"));
}

void UVDTitlePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(TitleBackgroundMediaPlayer && TitleBackgroundMediaSource)
	{
		TitleMediaSoundComponent = NewObject<UMediaSoundComponent>(this);
		TitleMediaSoundComponent->SetMediaPlayer(TitleBackgroundMediaPlayer);
		TitleMediaSoundComponent->RegisterComponentWithWorld(GetWorld());
		if (TitleBackgroundMediaPlayer->OpenSource(TitleBackgroundMediaSource))
		{
			TitleBackgroundMediaPlayer->Play();
		}
	}

	TitleTextWidget = Cast<UTextBlock>(GetWidgetFromName("GameTitleName"));
	if (TitleTextWidget)
	{
		TitleTextWidget->SetText(FText::FromString(FString(TEXT("ProjectVD"))));
	}

	StartButtonWidget = Cast<UButton>(GetWidgetFromName("StartButton"));
	if(StartButtonWidget)
	{
		UTextBlock* StartButtonText = Cast<UTextBlock>(StartButtonWidget->GetChildAt(0));
		if(StartButtonText)
		{
			StartButtonText->SetText(FText::FromString(FString(TEXT("Start"))));
		}

		StartButtonWidget->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickStartButton);
	}

	ExitButtonWidget = Cast<UButton>(GetWidgetFromName("ExitButton"));
	if(ExitButtonWidget)
	{
		UTextBlock* ExitButtonText = Cast<UTextBlock>(ExitButtonWidget->GetChildAt(0));
		if(ExitButtonText)
		{
			ExitButtonText->SetText(FText::FromString(FString(TEXT("Exit"))));
		}
		ExitButtonWidget->OnHovered.AddDynamic(this, &UVDTitlePanelUserWidget::OnHoverExitButton);
		ExitButtonWidget->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickExitButton);
	}

	TitleMovieSoundMuteToggleWidget = Cast<UCheckBox>(GetWidgetFromName("TitleMovieMuteToggle"));
	if(TitleMovieSoundMuteToggleWidget)
	{
		UTextBlock* ToggleText = Cast<UTextBlock>(TitleMovieSoundMuteToggleWidget->GetChildAt(0));
		if(ToggleText)
		{
			ToggleText->SetText(FText::FromString(FString(TEXT("MovieSoundMute"))));
		}
		TitleMovieSoundMuteToggleWidget->OnCheckStateChanged.AddDynamic(this, &UVDTitlePanelUserWidget::OnToggleTitleMovieMute);
		TitleMovieSoundMuteToggleWidget->SetCheckedState(ECheckBoxState::Unchecked);
	}


}
