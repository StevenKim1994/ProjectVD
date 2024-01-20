// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"
#include "UI/Title/VDTitlePanelUserWidget.h"

AVDTitleController::AVDTitleController()
{
	static ConstructorHelpers::FClassFinder<UVDTitlePanelUserWidget> TitlePanelUserWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/TitleUI/TitleUIPanel.TitleUIPanel_C'"));
	if(TitlePanelUserWidgetRef.Class)
	{
		TitlePanelUserWidgetClass = TitlePanelUserWidgetRef.Class;
	}
	bShowMouseCursor = true;
}

void AVDTitleController::BeginPlay()
{
	Super::BeginPlay();

	TitlePanelUserWidget = CreateWidget<UVDTitlePanelUserWidget>(this, TitlePanelUserWidgetClass);
	if(TitlePanelUserWidget.IsValid())
	{
		TitlePanelUserWidget->AddToViewport();
	}
}


