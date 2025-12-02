// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDCutSceneNamePlateWidget.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"

void UVDCutSceneNamePlateWidget::SetNamePlateText(const FText& InText)
{
	NamePlateText->SetText(InText);
}

void UVDCutSceneNamePlateWidget::SetNamePlateVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		PlayAnimation(ShowNamePlateAnim);
	}
	else
	{
		PlayAnimation(HideNamePlateAnim);
	}
}
