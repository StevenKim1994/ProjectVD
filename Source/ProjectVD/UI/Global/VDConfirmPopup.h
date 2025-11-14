// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
#include "VDConfirmPopup.generated.h"

UENUM(BlueprintType)
enum class EConfirmPopupResult : uint8
{
	Confirm,
	Cancel
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmPopupResult, EConfirmPopupResult, Result);

UCLASS()
class PROJECTVD_API UVDConfirmPopup : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "ButtonEventCallback")
	FOnConfirmPopupResult OnClickButtonEvent;
};
