// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Global/VDToastEntryWidget.h"
#include "Components/TextBlock.h"

void UVDToastEntryWidget::SetToastMessageText(const FText& InText)
{
	if (ToastMessageText)
	{
		ToastMessageText->SetText(InText);
	}
}
