// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Global/VDToastEntryWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UVDToastEntryWidget::OnToastShowingAnimationFinished()
{
	UE_LOG(LogTemp, Log, TEXT("UVDToastEntryWidget::OnToastShowingAnimationFinished"));

	if (OnToastAnimationFinished().IsBound())
	{
		ToastAnimationFinishedDelegate.Execute(this);
	}
}

void UVDToastEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(ShowAnim)
	{
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UVDToastEntryWidget::OnToastShowingAnimationFinished);
		BindToAnimationFinished(ShowAnim, EndDelegate);
	}
}

void UVDToastEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ShowAnim)
	{
		PlayAnimation(ShowAnim);
	}
}

void UVDToastEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDToastEntryWidget::SetToastMessageText(const FText& InText)
{
	if (ToastMessageText)
	{
		ToastMessageText->SetText(InText);
	}
}

