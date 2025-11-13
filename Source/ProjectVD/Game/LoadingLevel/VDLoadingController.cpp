// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadingLevel/VDLoadingController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"

void AVDLoadingController::BeginPlay()
{
	Super::BeginPlay();

	UVDGameInstance* GI = GetGameInstance<UVDGameInstance>();

	if (GI)
	{
		UVDUISubsystem* UISubSystem = GI->GetSubsystem<UVDUISubsystem>();
		if (UISubSystem)
		{
			UISubSystem->ShowUIWidget(this, VDConstants::LoadingPanel);
		}
	}
}

void AVDLoadingController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

AVDLoadingController::AVDLoadingController()
{

}
