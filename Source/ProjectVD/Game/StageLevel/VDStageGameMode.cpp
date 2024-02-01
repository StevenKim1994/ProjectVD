// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StageLevel/VDStageGameMode.h"
#include "Game/StageLevel/VDStagePlayerController.h"

AVDStageGameMode::AVDStageGameMode()
{
	PlayerControllerClass = AVDStagePlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVD.VDStagePlayerCharacter'"));
	if(DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
}

void AVDStageGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Init Stage Level"));
	
}


