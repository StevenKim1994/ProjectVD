// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDConfirmPopup.generated.h"

class UButton;
class UTextBlock;
class UWidgetAnimation;

UENUM(BlueprintType)
enum class EConfirmPopupResult : uint8
{
	Confirm,
	Cancel
};

DECLARE_DELEGATE_OneParam(FOnConfirmPopupResult, EConfirmPopupResult);

UCLASS()
class PROJECTVD_API UVDConfirmPopup : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ConfirmButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CancelButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowPopupAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HidePopupAnim;

	FOnConfirmPopupResult OnClickButtonEvent;

	UFUNCTION()
	void OnClickCancel();

	UFUNCTION()
	void OnClickConfirm();

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UVDConfirmPopup* SetTitleText(const FText& InText);
	UVDConfirmPopup* SetDescriptionText(const FText& InText);
	UVDConfirmPopup* SetConfirmButtonText(const FText& InText);
	UVDConfirmPopup* SetCancelButtonText(const FText& InText);

	FORCEINLINE FOnConfirmPopupResult& OnClickEvent() { return OnClickButtonEvent; }
};
