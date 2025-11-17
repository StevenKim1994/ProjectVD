// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDCutSceneSubSystem.h"

void UVDCutSceneSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVDCutSceneSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVDCutSceneSubSystem::StartCutScene(AActor* CutSceneActor, FOnCutSceneFinishedDelegate OnFinishedDelegate)
{
	bIsInCutScene = true;

	if (OnFinishedDelegate.IsBound())
	{
		OnFinishedDelegate.Execute();
	}
	bIsInCutScene = false;
}
