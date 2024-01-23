// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StageLevel/VDStageGameMode.h"

AVDStageGameMode::AVDStageGameMode()
{
	
}

void AVDStageGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Log, TEXT("StartLevel: %s"), *GetLevel()->GetOuter()->GetName());
}


