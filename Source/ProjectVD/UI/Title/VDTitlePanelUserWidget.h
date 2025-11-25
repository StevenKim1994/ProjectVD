// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDTitlePanelUserWidget.generated.h"

class UMediaPlayer;
class UMediaTexture;
class UButton;
class UTextBlock;
class UVerticalBox;
class UImage;
class UMediaSoundComponent;
class UMediaPlayer;
class UCheckBox;
class UMediaSource;
class UWidgetAnimation;

struct FWidgetAnimationState;

DECLARE_DELEGATE(FOnClickExitButtonEvent);
DECLARE_DELEGATE_OneParam(FOnToggleTitleMovieMuteEvent, bool);

UCLASS()
class PROJECTVD_API UVDTitlePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	bool bIsMainMenuButtonToggledOn = true;

	// UISection
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> OptionsParentBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonsParentBox;

	UPROPERTY(meta = (BindWidget))	
	TObjectPtr<UTextBlock> GameTitleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCheckBox> TitleMovieMuteToggle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
	float TitleButtonSlideDuration = 2.5f;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsBackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY( meta = (BindWidget)) // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MediaPlayerImage;

	UPROPERTY()
	TObjectPtr<UMediaTexture> MediaTexture;

	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MenuTween;

	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OptionTween;

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
	void OnChangedMenuStateTweenComplete(); // 트윈 완료시 콜백

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	FOnClickExitButtonEvent OnClickExitButtonEvent;
	FOnToggleTitleMovieMuteEvent OnToggleTitleMovieMuteEvent;

	void SetToggleBackgroundSound(bool IsOn);
	void SetBackgroundMediaTexture(UMediaTexture* Texture);
};
