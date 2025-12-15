// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Global/VDToastEntryWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UVDToastEntryWidget::OnToastShowingAnimationFinished()
{
	UE_LOG(LogTemp, Log, TEXT("UVDToastEntryWidget::OnToastShowingAnimationFinished"));

	if (!OnToastAnimationFinished().IsBound())
	{
		return;
	}

	if (ToastDuration <= 0.0f)
	{
		ToastAnimationFinishedDelegate.Execute(this);
		return;
	}

	if (UWorld* World = GetWorld())
	{
		TWeakObjectPtr<UVDToastEntryWidget> WeakThis = this;

		FTimerDelegate ToastFinishedDelegate;
		ToastFinishedDelegate.BindLambda([WeakThis]()
		{
			if (UVDToastEntryWidget* StrongThis = WeakThis.Get())
			{
				if (StrongThis->OnToastAnimationFinished().IsBound())
				{
					StrongThis->ToastAnimationFinishedDelegate.Execute(StrongThis);
				}
			}
		});

		FTimerHandle ToastFinishedTimerHandle;
		World->GetTimerManager().SetTimer(ToastFinishedTimerHandle, ToastFinishedDelegate, ToastDuration, false);
		return;
	}

	ToastAnimationFinishedDelegate.Execute(this);
}

void UVDToastEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ShowAnim)
	{
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UVDToastEntryWidget::OnToastShowingAnimationFinished);
		BindToAnimationFinished(ShowAnim, EndDelegate);
	}
}

void UVDToastEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowAnim)
	{
		PlayAnimation(ShowAnim);
	}
}

void UVDToastEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

UVDToastEntryWidget* UVDToastEntryWidget::SetToastMessageText(const FText& InText)
{
	if (ToastMessageText)
	{
		ToastMessageText->SetText(InText);
	}

	return this;
}

UVDToastEntryWidget* UVDToastEntryWidget::SetToastDuration(float InDuration)
{
	ToastDuration = InDuration;

	return this;
}

