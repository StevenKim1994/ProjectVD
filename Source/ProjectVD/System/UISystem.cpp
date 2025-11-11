// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UISystem.h"

void UUISystem::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("UUISystem Initialized"));
}

void UUISystem::Release()
{
	UE_LOG(LogTemp, Log, TEXT("UUISystem Released"));
}

void UUISystem::LoadSystem()
{
	Initialize();
}

void UUISystem::UnloadSystem()
{
	Release();
}
