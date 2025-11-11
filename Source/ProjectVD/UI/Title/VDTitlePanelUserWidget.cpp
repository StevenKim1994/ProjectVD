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
	const FVector2D FinalTranslation(0.f, 0.f); // 최종 위치(원래 배치 지점)
	const float Duration = 2.5f;                 // 트윈 시간
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
		TWeakObjectPtr<UVerticalBox> Box;
		FVector2D From, To;
		float Duration = 0.f;
		float Elapsed = 0.f;
	};

	TSharedRef<FTweenState, ESPMode::ThreadSafe> State = MakeShared<FTweenState, ESPMode::ThreadSafe>();
	State->Box = ButtonsParentsBox;
	State->From = CurrentPosition;
	State->To = TargetPosition;
	State->Duration = FMath::Max(0.f, Duration);

	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([State](float DeltaTime)
			{
				if (!State->Box.IsValid())
				{
					return false; // 위젯이 파괴되면 중단
				}

				State->Elapsed += DeltaTime;
				const float Alpha = (State->Duration > 0.f) ? FMath::Clamp(State->Elapsed / State->Duration, 0.f, 1.f) : 1.f;
				const float EaseExp = 2.0f;
				const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, EaseExp);

				const FVector2D NewT = FMath::Lerp(State->From, State->To, EasedAlpha);
				State->Box->SetRenderTranslation(NewT);

				return Alpha < 1.f; // 완료 시 false 반환하여 Ticker 해제
			}),
		0.0f // 0.0 = 매 프레임
	);
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

	//ButtonsParentsBox = Cast<UVerticalBox>(GetWidgetFromName(VDConstants::TitleWidgetButtonsParentsBox));
	if (ButtonsParentsBox)
	{
		bIsMainMenuButtonToggledOn = true;
		OnMainButtonToggle(bIsMainMenuButtonToggledOn);
	}

	TitleTextWidget = Cast<UTextBlock>(GetWidgetFromName(VDConstants::TitleWidgetTitleName));
	if (TitleTextWidget)
	{
		TitleTextWidget->SetText(VDConstants::TitleProjectName);
	}

	//StartButton= Cast<UButton>(GetWidgetFromName(VDConstants::TitleStartButtonName));
	if (StartButton)
	{
		UTextBlock* StartButtonText = Cast<UTextBlock>(StartButton->GetChildAt(0));
		if (StartButtonText)
		{
			StartButtonText->SetText(VDConstants::GetTitleStartButtonText());
		}

		StartButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickStartButton);
	}

	//OptionButton= Cast<UButton>(GetWidgetFromName(VDConstants::TitleOptionButtonName));
	if (OptionButton)
	{
		UTextBlock* OptionButtonText = Cast<UTextBlock>(OptionButton->GetChildAt(0));
		if (OptionButtonText)
		{
			OptionButtonText->SetText(VDConstants::GetTitleOptionButtonText());
		}
		OptionButton->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickOptionButton);
	}

	//ExitButton= Cast<UButton>(GetWidgetFromName(VDConstants::TitleExitButtonName));
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

	TitleMovieSoundMuteToggleWidget = Cast<UCheckBox>(GetWidgetFromName(VDConstants::TitleMovieSoundMuteToggleName));
	if (TitleMovieSoundMuteToggleWidget)
	{
		UTextBlock* ToggleText = Cast<UTextBlock>(TitleMovieSoundMuteToggleWidget->GetChildAt(0));
		if (ToggleText)
		{
			ToggleText->SetText(VDConstants::GetTitleSoundMuteToggleText());
		}
		TitleMovieSoundMuteToggleWidget->OnCheckStateChanged.AddDynamic(this, &UVDTitlePanelUserWidget::OnToggleTitleMovieMute);
		TitleMovieSoundMuteToggleWidget->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UVDTitlePanelUserWidget::SetToggleBackgroundSound(bool IsOn)
{
	if (TitleMediaSoundComponent)
	{
		TitleMediaSoundComponent->SetVolumeMultiplier(IsOn ? 0.0f : 1.0f);
	}
}
