// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDToastEntryWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

DECLARE_DELEGATE_OneParam(FOnToastAnimationFinished, UVDToastEntryWidget*);

UCLASS()
class PROJECTVD_API UVDToastEntryWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ToastMessageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowAnim;

	float ToastDuration;

	UFUNCTION()
	void OnToastShowingAnimationFinished();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	FOnToastAnimationFinished ToastAnimationFinishedDelegate;

public:
	UFUNCTION()
	UVDToastEntryWidget* SetToastMessageText(const FText& InText);

	UFUNCTION()
	UVDToastEntryWidget* SetToastDuration(float InDuration);

	FORCEINLINE FOnToastAnimationFinished& OnToastAnimationFinished() { return ToastAnimationFinishedDelegate; }
};
