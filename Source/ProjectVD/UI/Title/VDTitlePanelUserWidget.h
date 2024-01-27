// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "VDTitlePanelUserWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClickStartButtonEvent);
DECLARE_MULTICAST_DELEGATE(FOnClickExitButtonEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleTitleMovieMuteEvent, bool);
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDTitlePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:

// MediaSection -> 추후 Actor로 이동시켜야함 어짜피 재생되면 알아서 Image에 그려줌.
	UPROPERTY()
	UMediaPlayer* TitleBackgroundMediaPlayer;

	UPROPERTY()
	UMediaSource* TitleBackgroundMediaSource;

// UISection
	UPROPERTY()
	UTextBlock* TitleTextWidget;

	UPROPERTY()
	UButton* StartButtonWidget;

	UPROPERTY()
	UButton* ExitButtonWidget;

	UPROPERTY()
	UCheckBox* TitleMovieSoundMuteToggleWidget;

	UFUNCTION()
	void OnClickStartButton();

	UFUNCTION()
	void OnClickExitButton();

	UFUNCTION()
	void OnToggleTitleMovieMute(bool ChangedToggleValue);

	UFUNCTION()
	void OnHoverExitButton();
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

public:
	FOnClickStartButtonEvent OnClickStartButtonEvent;
	FOnClickExitButtonEvent OnClickExitButtonEvent;
	FOnToggleTitleMovieMuteEvent OnToggleTitleMovieMuteEvent;
};
