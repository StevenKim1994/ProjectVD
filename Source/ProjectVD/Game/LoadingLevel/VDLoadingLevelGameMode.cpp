// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadingLevel/VDLoadingLevelGameMode.h"
#include "Game/LoadingLevel/VDLoadingController.h"

void AVDLoadingLevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnLoadingStarted();
}

void AVDLoadingLevelGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnLoadingFinished();
}

void AVDLoadingLevelGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!LoadingController)
	{
		LoadingController = Cast<AVDLoadingController>(NewPlayer);
	}
}

void AVDLoadingLevelGameMode::OnLoadingProgressUpdated(float Percent)
{
	if(LoadingController)
	{
		LoadingController->UpdateLoadingPercent(Percent);
	}
}

void AVDLoadingLevelGameMode::OnLoadingFinished()
{
	if (LoadingController)
	{
		LoadingController->HideLoadingPanel();
	}
}

void AVDLoadingLevelGameMode::OnLoadingStarted()
{
	if (LoadingController)
	{
		LoadingController->ShowLoadingPanel();
	}
}
