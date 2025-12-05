// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStageGameOverWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE(FOnRetryButtonClickedDelegate);
DECLARE_DELEGATE(FOnExitButtonClickedDelegate);

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDStageGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOnRetryButtonClickedDelegate(const FOnRetryButtonClickedDelegate& Delegate) { OnRetryButtonClickedDelegate = Delegate; }
	FORCEINLINE void SetOnExitButtonClickedDelegate(const FOnExitButtonClickedDelegate& Delegate) { OnExitButtonClickedDelegate = Delegate; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameOverText;

	UPROPERTY(meta = (BindWidget))
	UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RetryButtonText;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExitButtonText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowUpAnim;

	UFUNCTION()
	void OnRetryButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	FOnRetryButtonClickedDelegate OnRetryButtonClickedDelegate;
	FOnExitButtonClickedDelegate OnExitButtonClickedDelegate;
};
