// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/Character/VDStagePlayerCharacter.h"

AVDStagePlayerController::AVDStagePlayerController()
{
}

void AVDStagePlayerController::BeginPlay()
{
	Super::BeginPlay();

	const AVDStagePlayerCharacter* PlayerCharacter = GetPawn<AVDStagePlayerCharacter>();
	if(PlayerCharacter->GetClassType() == EClassType::None)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Cyan, TEXT("State : Character Select"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, TEXT("Start!"));
	}
}
