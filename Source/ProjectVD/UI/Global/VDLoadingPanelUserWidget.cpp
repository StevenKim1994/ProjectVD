// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Global/VDLoadingPanelUserWidget.h"

UVDLoadingPanelUserWidget* UVDLoadingPanelUserWidget::SetLoadingText(const FText& InText)
{
	LoadingDescText->SetText(InText);

	return this;
}

UVDLoadingPanelUserWidget* UVDLoadingPanelUserWidget::SetLoadingPercent(float InPercent)
{
	FText PercentText = FText::AsPercent(InPercent);
	LoadingPercentText->SetText(PercentText);

	return this;
}
