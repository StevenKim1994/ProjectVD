// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/VDItemPropNamePlateWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UVDItemPropNamePlateWidget::ShowNamePlate(bool bIsShow)
{
	if (bIsShow)
	{
		StopAnimation(HideNamePlateAnim);
		if (IsAnimationPlaying(ShowNamePlateAnim))
		{
			return;
		}

		PlayAnimation(ShowNamePlateAnim);
	}
	else
	{
		StopAnimation(ShowNamePlateAnim);
		if (IsAnimationPlaying(HideNamePlateAnim))
		{
			return;
		}
		PlayAnimation(HideNamePlateAnim);
	}
}

void UVDItemPropNamePlateWidget::SetItemNameText(const FText& InText)
{
	if (ItemNameTextBlock)
	{
		ItemNameTextBlock->SetText(InText);
	}
}

void UVDItemPropNamePlateWidget::NativeOnInitialized()
{
	if (ShowNamePlateAnim)
	{
		FWidgetAnimationDynamicEvent ShowAnimFinishedEvent;
		ShowAnimFinishedEvent.BindDynamic(this, &UVDItemPropNamePlateWidget::NativeConstruct);
		BindToAnimationFinished(ShowNamePlateAnim, ShowAnimFinishedEvent);
	}

	if (HideNamePlateAnim)
	{
		FWidgetAnimationDynamicEvent HideAnimFinishedEvent;
		HideAnimFinishedEvent.BindDynamic(this, &UVDItemPropNamePlateWidget::NativeDestruct);
		BindToAnimationFinished(HideNamePlateAnim, HideAnimFinishedEvent);
	}
}

void UVDItemPropNamePlateWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDItemPropNamePlateWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
