// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadingLevel/VDLoadingLevelGameMode.h"
#include "Game/LoadingLevel/VDLoadingController.h"
#include "System/VDLevelSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "System/VDLevelSystem.h"
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
	if (!LoadingController.IsValid())
	{
		LoadingController = Cast<AVDLoadingController>(NewPlayer);
	}
}

void AVDLoadingLevelGameMode::OnLoadingProgressUpdated(float Percent)
{
	if (LoadingController.IsValid())
	{
		LoadingController->UpdateLoadingPercent(Percent);
	}
}

void AVDLoadingLevelGameMode::OnLoadingFinished()
{
	if (LoadingController.IsValid())
	{
		LoadingController->HideLoadingPanel();
	}

	UVDLevelSystem* LevelSystem = GetGameInstance()->GetSubsystem<UVDLevelSystem>();
	LevelSystem->ChangeToNextLevel();
}

void AVDLoadingLevelGameMode::OnLoadingStarted()
{
	if (LoadingController.IsValid())
	{
		LoadingController->ShowLoadingPanel();


		UVDLevelSystem* LevelSystem = GetGameInstance()->GetSubsystem<UVDLevelSystem>();
		LevelSystem->GetLevelLoadedDelegate().BindUObject(this, &AVDLoadingLevelGameMode::OnLoadingProgressUpdated);
		LevelSystem->GetLevelLoadedCompleteDelegate().BindUObject(this, &AVDLoadingLevelGameMode::OnLoadingFinished);
		LevelSystem->LoadPrepareNextLevelAssets();
		LevelSystem->LoadChangeLevel();
	}
}
