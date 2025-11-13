// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"

void UVDGameInstance::Init()
{
	Super::Init();
}

void UVDGameInstance::SetFrameRate(int32 NewFrameRate)
{
	GEngine->SetMaxFPS(NewFrameRate);

	UE_LOG(LogTemp, Log, TEXT("Frame rate set to: %d"), NewFrameRate);
}
