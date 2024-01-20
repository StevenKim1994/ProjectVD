// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "VDTitlePanelUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDTitlePanelUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY()
	UTextBlock* TitleTextWidget;

	UPROPERTY()
	UButton* StartButtonWidget;

	UPROPERTY()
	UButton* ExitButtonWidget;

	UFUNCTION()
	void OnClickStartButton();

	UFUNCTION()
	void OnClickExitButton();

protected:
	virtual void NativeConstruct() override;
	
};
