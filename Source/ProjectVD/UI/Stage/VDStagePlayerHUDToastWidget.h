// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStagePlayerHUDToastWidget.generated.h"

class UTextBlock;
class URichTextBlock;
class UImage;

UCLASS()
class PROJECTVD_API UVDStagePlayerHUDToastWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MessageIcon;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void ShowToast(const FString& InTitle, const FString& InMessage);	
	
};
