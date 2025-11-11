// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Title/VDTitlePanelUserWidget.h"
#include "Game/VDGameInstance.h"
#include "Public/VDConstrants.h"
#include "Containers/Ticker.h"        
#include "Components/CanvasPanelSlot.h" 


void UVDTitlePanelUserWidget::OnClickStartButton()
{
	if (OnClickStartButtonEvent.IsBound())
	{
		OnClickStartButtonEvent.Broadcast();
	}

}

void UVDTitlePanelUserWidget::OnClickOptionButton()
{
	if (ButtonsParentsBox)
	{
		bIsMainMenuButtonToggledOn = !bIsMainMenuButtonToggledOn;
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
		OnClickExitButtonEvent.Broadcast();
	}
}

void UVDTitlePanelUserWidget::OnClickOptionsBackButton()
{
	if (ButtonsParentsBox)
	{
		bIsMainMenuButtonToggledOn = !bIsMainMenuButtonToggledOn;
		OnMainButtonToggle(bIsMainMenuButtonToggledOn);
	}
}

void UVDTitlePanelUserWidget::OnToggleTitleMovieMute(bool ChangedToggleValue)
{

	if (OnToggleTitleMovieMuteEvent.IsBound())
	{
		OnToggleTitleMovieMuteEvent.Broadcast(ChangedToggleValue);
	}
}

void UVDTitlePanelUserWidget::OnHoverExitButton()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered!"));
}


void UVDTitlePanelUserWidget::OnMainButtonToggle(bool IsOn)
{
	FIntPoint ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();
	const FVector2D StartTranslation(ScreenSize.X, 0.f);
	const FVector2D FinalTranslation(0.f, 0.f);
	const float Duration = 2.5f;
	FVector2D TargetPosition;
	FVector2D CurrentPosition;

	if (IsOn)
	{
		TargetPosition = FinalTranslation;
		CurrentPosition = StartTranslation;
	}
	else
	{
		TargetPosition = StartTranslation;
		CurrentPosition = FinalTranslation;
	}
	
	ButtonsParentsBox->SetRenderTranslation(CurrentPosition);

	struct FTweenState
	{
		TWeakObjectPtr<UVerticalBox> InBox;
		TWeakObjectPtr<UVerticalBox> OutBox;
		TWeakObjectPtr<UVDTitlePanelUserWidget> Widget;
		FVector2D From, To;
		float Duration = 0.f;
		float Elapsed = 0.f;
		bool bIsOn = false;
	};

	TSharedRef<FTweenState, ESPMode::ThreadSafe> State = MakeShared<FTweenState, ESPMode::ThreadSafe>();
	State->InBox = ButtonsParentsBox;
	State->OutBox = OptionsParentsBox;
	State->Widget = this;
	State->From = CurrentPosition;
	State->To = TargetPosition;
	State->Duration = FMath::Max(0.f, TitleButtonSlideDuration);
	State->bIsOn = IsOn;

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([State](float DeltaTime)
			{
				if (!State->InBox.IsValid() || !State->Widget.IsValid())
				{
					return false;
				}

				State->Elapsed += DeltaTime;
				const float Alpha = (State->Duration > 0.f) ? FMath::Clamp(State->Elapsed / State->Duration, 0.f, 1.f) : 1.f;
				const float EaseExp = 2.0f;
				const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, EaseExp);

				const FVector2D NewT = FMath::Lerp(State->From, State->To, EasedAlpha);
				State->InBox->SetRenderTranslation(NewT);

				if (Alpha >= 1.f)
				{
					State->Widget->OnChangedMenuStateTweenComplete(State->bIsOn);
					return false;
				}
				else
				{
					return true;
				}
			}),
		0.0f
	);
}

void UVDTitlePanelUserWidget::OnChangedMenuStateTweenComplete(bool IsOn)
{
	ButtonsParentsBox->Visibility = IsOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	OptionsParentsBox->Visibility = IsOn ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
}

void UVDTitlePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TitleBackgroundMediaPlayer && TitleBackgroundMediaSource)
	{
		TitleMediaSoundComponent = NewObject<UMediaSoundComponent>(this);
		TitleMediaSoundComponent->SetMediaPlayer(TitleBackgroundMediaPlayer);
		TitleMediaSoundComponent->RegisterComponentWithWorld(GetWorld());
		if (TitleBackgroundMediaPlayer->OpenSource(TitleBackgroundMediaSource))
		{
			TitleBackgroundMediaPlayer->Play();
		}
	}

	if (ButtonsParentsBox)
	{
		bIsMainMenuButtonToggledOn = true;
		OnMainButtonToggle(bIsMainMenuButtonToggledOn);
	}

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

	if(OptionsBackButton)
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

void UVDTitlePanelUserWidget::SetToggleBackgroundSound(bool IsOn)
{
	if (TitleMediaSoundComponent)
	{
		TitleMediaSoundComponent->SetVolumeMultiplier(IsOn ? 0.0f : 1.0f);
	}
}
