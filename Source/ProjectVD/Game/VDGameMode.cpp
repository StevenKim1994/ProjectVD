// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VDGameMode.h"
AVDGameMode::AVDGameMode() 
{
	static ConstructorHelpers::FClassFinder<APlayerController> TitleControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVD.VDTitleController'"));
	if(TitleControllerClassRef.Class)
	{
		PlayerControllerClass = TitleControllerClassRef.Class;
	}
}

void AVDGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
