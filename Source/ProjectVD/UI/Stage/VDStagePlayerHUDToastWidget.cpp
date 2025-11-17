// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDStagePlayerHUDToastWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/Image.h"

void UVDStagePlayerHUDToastWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVDStagePlayerHUDToastWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDStagePlayerHUDToastWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDStagePlayerHUDToastWidget::ShowToast(const FString& InTitle, const FString& InMessage)
{
	SetVisibility(ESlateVisibility::Visible);
	TitleText->SetText(FText::FromString(InTitle));
	MessageText->SetText(FText::FromString(InMessage));
}
