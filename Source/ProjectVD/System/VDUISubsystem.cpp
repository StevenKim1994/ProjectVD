// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDUISubsystem.h"
#include "Engine/AssetManager.h"
#include "Public/VDConstrants.h"
#include "DataAsset/VDUIRegistry.h"
#include "System/VDResourceSystem.h"

UVDUISubsystem::UVDUISubsystem()
{

}

void UVDUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVDUISubsystem::Deinitialize()
{

}

TSoftClassPtr<UUserWidget> UVDUISubsystem::GetUIWidgetClassPathByName(const FName& WidgetName)
{
	if (!UIRegistry)
	{
		UIRegistry = GetGameInstance()->GetSubsystem<UVDResourceSystem>()->GetLoadedPrimaryAsset<UVDUIRegistry>(UVDUIRegistry::StaticClass()->GetPrimaryAssetId());
	}

	if (UIRegistry.IsValid())
	{
		return UIRegistry->GetWidgetClassByName(WidgetName);
	}

	return nullptr;
}

