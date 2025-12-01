// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Global/VDLoadingPanelUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"

void UVDLoadingPanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadingDescTextChangeCurrentTime = 0.0f;
	LoadingDescText->SetText(FText::FromString(TEXT("Loading")));
	LoadingDescOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UVDLoadingPanelUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	LoadingDescTextChangeCurrentTime += InDeltaTime;
	if (LoadingDescTextChangeCurrentTime >= LoadingDescTextChangeInterval)
	{
		UpdateLoadingDescText(LoadingDescTextChangeCurrentTime);
		LoadingDescTextChangeCurrentTime = 0.0f;
	}
}

void UVDLoadingPanelUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDLoadingPanelUserWidget::UpdateLoadingDescText(float DeltaTime)
{
	if (DeltaTime >= LoadingDescTextChangeInterval)
	{
		FString CurrentText = LoadingDescText->GetText().ToString();
		CurrentText.Append(TEXT("."));
		if (CurrentText.Len() > 13) // "Loading......"
		{
			CurrentText = TEXT("Loading");
		}
		LoadingDescText->SetText(FText::FromString(CurrentText));
	}

}

UVDLoadingPanelUserWidget* UVDLoadingPanelUserWidget::SetLoadingText(const FText& InText)
{
	LoadingDescText->SetText(InText);

	return this;
}

UVDLoadingPanelUserWidget* UVDLoadingPanelUserWidget::SetLoadingPercent(float InPercent)
{
	FText PercentText = FText::AsPercent(InPercent);

	if (LoadingDescOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		if (InPercent > 0.0f)
		{
			LoadingDescOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}

	LoadingPercentText->SetText(PercentText);

	return this;
}
