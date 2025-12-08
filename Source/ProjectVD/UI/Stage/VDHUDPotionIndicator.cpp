// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Stage/VDHUDPotionIndicator.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

void UVDHUDPotionIndicator::SetCoolTimeText(const FText& InText)
{
	CoolTimeText->SetText(InText);
}

void UVDHUDPotionIndicator::SetCoolTimeVisibility(bool bIsVisible)
{
	CoolTimeOverlay->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UVDHUDPotionIndicator::SetPotionCount(int32 InCount)
{
	PotionCountText->SetText(FText::AsNumber(InCount));
}

void UVDHUDPotionIndicator::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetCoolTimeVisibility(false);
	SetPotionCount(0);
}

void UVDHUDPotionIndicator::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDHUDPotionIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UVDHUDPotionIndicator::NativeDestruct()
{
	Super::NativeDestruct();
}