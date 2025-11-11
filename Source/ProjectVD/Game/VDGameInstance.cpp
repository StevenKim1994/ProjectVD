// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include "System/UISystem.h"

void UVDGameInstance::Init()
{
	Super::Init();
	UISystem = NewObject<UUISystem>(this, UUISystem::StaticClass());
	if (UISystem)
	{
		UISystem->LoadSystem();
	}
}

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
