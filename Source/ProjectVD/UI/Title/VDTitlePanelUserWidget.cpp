// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Title/VDTitlePanelUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UVDTitlePanelUserWidget::OnClickStartButton()
{
	UGameplayStatics::OpenLevel(this, TEXT("Stage"));
}

void UVDTitlePanelUserWidget::OnClickExitButton()
{
	UE_LOG(LogTemp, Log, TEXT("ExitClick!"));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController() , EQuitPreference::Quit, false);
}

void UVDTitlePanelUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TitleTextWidget = Cast<UTextBlock>(GetWidgetFromName("GameTitleName"));
	if (TitleTextWidget)
	{
		TitleTextWidget->SetText(FText::FromString(FString(TEXT("ProjectVD"))));
	}

	StartButtonWidget = Cast<UButton>(GetWidgetFromName("StartButton"));
	if(StartButtonWidget)
	{
		UTextBlock* StartButtonText = Cast<UTextBlock>(StartButtonWidget->GetChildAt(0));
		if(StartButtonText)
		{
			StartButtonText->SetText(FText::FromString(FString(TEXT("Start"))));
		}

		StartButtonWidget->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickStartButton);
	}

	ExitButtonWidget = Cast<UButton>(GetWidgetFromName("ExitButton"));
	if(ExitButtonWidget)
	{
		UTextBlock* ExitButtonText = Cast<UTextBlock>(ExitButtonWidget->GetChildAt(0));
		if(ExitButtonText)
		{
			ExitButtonText->SetText(FText::FromString(FString(TEXT("Exit"))));
		}
		
		ExitButtonWidget->OnClicked.AddDynamic(this, &UVDTitlePanelUserWidget::OnClickExitButton);
	}

}
