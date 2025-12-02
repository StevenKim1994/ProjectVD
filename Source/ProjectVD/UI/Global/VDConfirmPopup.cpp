// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Global/VDConfirmPopup.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "System/VDUISubsystem.h"
void UVDConfirmPopup::OnClickCancel()
{
	OnClickButtonEvent.ExecuteIfBound(EConfirmPopupResult::Cancel);
	GetGameInstance()->GetSubsystem<UVDUISubsystem>()->HideUIWidget(this);
}

void UVDConfirmPopup::OnClickConfirm()
{
	OnClickButtonEvent.ExecuteIfBound(EConfirmPopupResult::Confirm);
	GetGameInstance()->GetSubsystem<UVDUISubsystem>()->HideUIWidget(this);
}

void UVDConfirmPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConfirmButton->OnClicked.AddDynamic(this, &UVDConfirmPopup::OnClickConfirm);
	CancelButton->OnClicked.AddDynamic(this, &UVDConfirmPopup::OnClickCancel);
}

void UVDConfirmPopup::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVDConfirmPopup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVDConfirmPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

UVDConfirmPopup* UVDConfirmPopup::SetTitleText(const FText& InText)
{
	TitleText->SetText(InText);

	return this;
}

UVDConfirmPopup* UVDConfirmPopup::SetDescriptionText(const FText& InText)
{
	DescriptionText->SetText(InText);

	return this;
}

UVDConfirmPopup* UVDConfirmPopup::SetConfirmButtonText(const FText& InText)
{
	ConfirmButtonText->SetText(InText);

	return this;
}

UVDConfirmPopup* UVDConfirmPopup::SetCancelButtonText(const FText& InText)
{
	CancelButtonText->SetText(InText);

	return this;
}
