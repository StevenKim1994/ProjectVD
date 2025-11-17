// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStagePauseWidget.generated.h"

UENUM()
enum class PauseMenuButtonEnum : uint8
{
	Resume,
	Options,
	ExitToTitle
};

DECLARE_DELEGATE_OneParam(FOnPauseMenuButtonClicked, PauseMenuButtonEnum)

class UButton;
UCLASS()
class PROJECTVD_API UVDStagePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitToTitleButton;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnClickResume();

	UFUNCTION()
	void OnClickOptions();

	UFUNCTION()
	void OnClickExitToTitle();


public:
	FOnPauseMenuButtonClicked OnPauseMenuButtonClickedDelegate;
};
