// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"

void UVDGameInstance::GotoInGameLevel(FName _TargetLevelName)
{
	if (DoesLevelExist(_TargetLevelName) == true)
	{
		TargetLevelName = _TargetLevelName;

		UGameplayStatics::OpenLevel(this, "Loading");
	}
	else
	{

	}
}

bool UVDGameInstance::DoesLevelExist(FName _TargetLevelName)
{
	FString LevelPath = FString::Printf(TEXT("/Game/ProjectVD/Level/%s"), *_TargetLevelName.ToString());

	return FPackageName::DoesPackageExist(LevelPath);
}

void UVDGameInstance::OnPostLoadLevel()
{
	UGameplayStatics::OpenLevel(this, TargetLevelName);
}
