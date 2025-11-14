// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaSoundComponent.h"
#include "VDTitlePanelUserWidget.generated.h"

class UMediaPlayer;
class UMediaTexture;

DECLARE_MULTICAST_DELEGATE(FOnClickExitButtonEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnToggleTitleMovieMuteEvent, bool);

UCLASS()
class PROJECTVD_API UVDTitlePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	bool bIsMainMenuButtonToggledOn = true;

	// UISection
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OptionsParentBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ButtonsParentBox;

	UPROPERTY(meta = (BindWidget))	
	UTextBlock* GameTitleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	UCheckBox* TitleMovieMuteToggle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
	float TitleButtonSlideDuration = 2.5f;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsBackButton;

	UPROPERTY(meta = (BindWidget))// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget)) // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	UButton* OptionButton;

	UPROPERTY( meta = (BindWidget)) // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UImage* MediaPlayerImage;

	UPROPERTY()
	UMediaTexture* MediaTexture;

	UFUNCTION()
	void OnClickStartButton();

	UFUNCTION()
	void OnClickOptionButton();

	UFUNCTION()
	void OnClickExitButton();

	UFUNCTION()
	void OnClickOptionsBackButton();

	UFUNCTION()
	void OnToggleTitleMovieMute(bool ChangedToggleValue);

	UFUNCTION()
	void OnHoverExitButton();

	

	UFUNCTION()
	void OnMainButtonToggle(bool IsOn);

	UFUNCTION()
	void OnChangedMenuStateTweenComplete(bool IsOn); // 트윈 완료시 콜백

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

public:
	FOnClickExitButtonEvent OnClickExitButtonEvent;
	FOnToggleTitleMovieMuteEvent OnToggleTitleMovieMuteEvent;

	UFUNCTION()
	void SetToggleBackgroundSound(bool IsOn);

	void SetBackgroundMediaTexture(UMediaTexture* Texture);
};
