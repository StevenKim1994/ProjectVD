// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/VDTitleLevelActor.h"
#include "ETC/VDTitleMovieActor.h"
#include "System/VDResourceSystem.h"
#include "Game/VDGameInstance.h"

AVDTitleLevelActor::AVDTitleLevelActor()
{

}

void AVDTitleLevelActor::SetToggleBackgroundMediaSoundMute(bool bIsMute)
{
	if (TitleMovieActor)
	{
		TitleMovieActor->SetChangeState(bIsMute);
		UE_LOG(LogTemp, Warning, TEXT("SetToggleBackgroundMediaSoundMute called with bIsMute: %s"), bIsMute ? TEXT("true") : TEXT("false"));
	}
}

void AVDTitleLevelActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVDTitleLevelActor::InitializeTitleLevel()
{
	LoadTitleMovieResources();
}

void AVDTitleLevelActor::ShowTitleUI()
{
}

void AVDTitleLevelActor::LoadTitleMovieResources()
{
	if (TitleMovieActor)
	{

	}
}


