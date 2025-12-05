// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stage/VDHUDInteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"

UVDHUDInteractionWidget* UVDHUDInteractionWidget::SetInteractionText(const FText& InText)
{
	InteractionText->SetText(InText);

	return this;
}

UVDHUDInteractionWidget* UVDHUDInteractionWidget::SetInteractionKeyText(const FText& InText)
{
	InteractionKeyText->SetText(InText);

	return this;
}

void UVDHUDInteractionWidget::ShowInteractionWidget(bool bIsShow)
{
	if (bIsShow)
	{
		SetVisibility(ESlateVisibility::Visible);
		StopAnimation(HideInteractionAnim);
		if (IsAnimationPlaying(ShowInteractionAnim))
		{
			return;
		}

		PlayAnimation(ShowInteractionAnim);
	}
	else
	{
		StopAnimation(ShowInteractionAnim);
		if (IsAnimationPlaying(HideInteractionAnim))
		{
			return;
		}

		PlayAnimation(HideInteractionAnim);
	}
}

void UVDHUDInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(ShowInteractionAnim)
	{
		FWidgetAnimationDynamicEvent ShowDelegate;
		ShowDelegate.BindDynamic(this, &UVDHUDInteractionWidget::NativeConstruct);
		BindToAnimationFinished(ShowInteractionAnim, ShowDelegate);
	}

	if (HideInteractionAnim)
	{
		FWidgetAnimationDynamicEvent HideDelegate;
		HideDelegate.BindDynamic(this, &UVDHUDInteractionWidget::NativeDestruct);// OnHideInteractionAnimFinished);
		BindToAnimationFinished(HideInteractionAnim, HideDelegate);
	}
}

void UVDHUDInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDHUDInteractionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDHUDInteractionWidget::OnHideInteractionAnimFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
