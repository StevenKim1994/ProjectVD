// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleController.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Title/VDTitlePanelUserWidget.h"
#include "ETC/VDTitleMovieActor.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"

AVDTitleController::AVDTitleController()
{
	SetShowMouseCursor(true);
}

void AVDTitleController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());

	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

	if (GI)
	{
		UVDUISubsystem* UISubsystem = GI->GetSubsystem<UVDUISubsystem>();
		if (UISubsystem)
		{
			UISubsystem->SetPlayerControllerRootUIWidget(this);
			UISubsystem->ShowUIWidgetAsync(VDConstants::TitlePanel);
		}
	}
}

