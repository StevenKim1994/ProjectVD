// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadingLevel/VDLoadingLevelGameMode.h"
#include "Game/LoadingLevel/VDLoadingController.h"
#include "System/VDLevelSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"

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

	GetGameInstance()->GetSubsystem<UVDLevelSystem>()->LoadChangeLevel();
}

FTimerHandle TimerHandle;
void AVDLoadingLevelGameMode::OnLoadingStarted()
{
	if (LoadingController.IsValid())
	{
		LoadingController->ShowLoadingPanel();
		
		// TODO :: DataAsset , StreamableManager로 다음 레벨에서 사용하는 에셋정보 미리 로드 한다음 OpenLevel 진행하기 .. 퍼센트 보이게 하려면 이방법을 써야할듯 추후 추가필요
		// 지금은 페이크 퍼센트이후 OpenLevel 호출
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this]()
			{
				static float FakePercent = 0.0f;
				FakePercent += 0.1f;
				if (FakePercent > 1.0f)
				{
					FakePercent = 0.0f;
					GetWorldTimerManager().ClearTimer(TimerHandle);
					OnLoadingFinished();
				}
				else
				{
					OnLoadingProgressUpdated(FakePercent);
				}
			},
			0.1f,
			true
		);
	}
}
