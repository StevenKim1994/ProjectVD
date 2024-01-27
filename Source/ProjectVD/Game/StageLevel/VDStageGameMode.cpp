// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StageLevel/VDStageGameMode.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"

AVDStageGameMode::AVDStageGameMode()
{
	PlayerControllerClass = AVDStagePlayerController::StaticClass();
	DefaultPawnClass = AVDStagePlayerCharacter::StaticClass();
}

void AVDStageGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Init Stage Level"));
	
}


