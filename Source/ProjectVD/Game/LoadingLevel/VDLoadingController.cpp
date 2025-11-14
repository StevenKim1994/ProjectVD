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

void AVDLoadingController::ShowLoadingPanel()
{
	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();
	if (GI)
	{
		UVDUISubsystem* UISubsystem = GI->GetSubsystem<UVDUISubsystem>();
		if (UISubsystem)
		{
			UISubsystem->SetPlayerControllerRootUIWidget(this);
			FOnUIWidgetLoadedDelegate Delegate;
			Delegate.BindWeakLambda(this, [this](UUserWidget* LoadedUIWidget)
			{
				LoadingPanelWidget = Cast<UVDLoadingPanelUserWidget>(LoadedUIWidget);
				// TODO :: 로딩 시작
			});
			
			UISubsystem->ShowUIWidgetAsync(VDConstants::LoadingPanel, Delegate);
		}
	}
}

void AVDLoadingController::HideLoadingPanel()
{
	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();
	if (GI)
	{
		UVDUISubsystem* UISubsystem = GI->GetSubsystem<UVDUISubsystem>();
		if (UISubsystem)
		{
			UISubsystem->HideUIWidget(VDConstants::LoadingPanel);
		}
	}
}

void AVDLoadingController::UpdateLoadingPercent(float InPercent)
{
	if (LoadingPanelWidget)
	{
		LoadingPanelWidget->SetLoadingPercent(InPercent);
	}
}

void AVDLoadingController::BeginPlay()
{
	Super::BeginPlay();

}

void AVDLoadingController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


