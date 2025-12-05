// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDHUDInteractionWidget.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDHUDInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UVDHUDInteractionWidget* SetInteractionText(const FText& InText);
	UVDHUDInteractionWidget* SetInteractionKeyText(const FText& InText);
	void ShowInteractionWidget(bool bIsShow);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> InteractionBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionKeyText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowInteractionAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideInteractionAnim;

	UFUNCTION()
	void OnHideInteractionAnimFinished();
};
