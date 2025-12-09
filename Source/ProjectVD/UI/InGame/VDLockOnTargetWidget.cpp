// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/VDLockOnTargetWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

void UVDLockOnTargetWidget::OnTargetLockOnChanged(AActor* TargetActor, bool bIsLocked)
{
	if (bIsLocked && TargetActor)
	{
		LockedOnTargetActor = TargetActor;
	}
	else
	{
		LockedOnTargetActor.Reset();
	}
}

void UVDLockOnTargetWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDLockOnTargetWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDLockOnTargetWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LockedOnTargetActor.IsValid())
	{
		FVector2D ScreenPosition;

		bool bIsOnScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), LockedOnTargetActor->GetActorLocation(), ScreenPosition, true);
		if (bIsOnScreen)
		{
			LockOnTargetImage->SetVisibility(ESlateVisibility::Visible);
			LockOnTargetImage->SetRenderTranslation(ScreenPosition - (LockOnTargetImage->GetDesiredSize() * 0.5f));
		}
		else
		{
			LockOnTargetImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		LockOnTargetImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UVDLockOnTargetWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RootPanel)
	{
		RootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
