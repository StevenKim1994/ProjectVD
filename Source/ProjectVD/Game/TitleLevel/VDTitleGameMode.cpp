// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleLevel/VDTitleGameMode.h"
AVDTitleGameMode::AVDTitleGameMode() 
{
/*
	static ConstructorHelpers::FClassFinder<APlayerController> TitleControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVD.VDTitleController'"));
	if(TitleControllerClassRef.Class)
	{
		PlayerControllerClass = TitleControllerClassRef.Class;
	}
*/
}

void AVDTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AVDTitleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


