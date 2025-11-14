// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadingLevel/VDLoadingController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"
#include "UI/Global/VDLoadingPanelUserWidget.h"

AVDLoadingController::AVDLoadingController()
{
	SetShowMouseCursor(false);
}

void AVDLoadingController::BeginPlay()
{
	Super::BeginPlay();

	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

	if (GI)
	{
		UVDUISubsystem* UISubSystem = GI->GetSubsystem<UVDUISubsystem>();
		if (UISubSystem)
		{
			UVDLoadingPanelUserWidget* LoadingWidget = Cast<UVDLoadingPanelUserWidget>(UISubSystem->ShowUIWidget(this, VDConstants::LoadingPanel));
			if (LoadingWidget)
			{
				LoadingWidget
					->SetLoadingText(FText::FromString(TEXT("Loading...")))
					->SetLoadingPercent(0.0f);
			}
		}
	}
}

void AVDLoadingController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

