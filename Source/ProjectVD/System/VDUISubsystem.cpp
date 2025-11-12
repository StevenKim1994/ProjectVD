// Fill out your copyright notice in the Description page of Project Settings.


#include "System/VDUISubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/VDConstrants.h"
#include "DataAsset/VDUIRegistry.h"

UVDUISubsystem::UVDUISubsystem()
{
	ConstructorHelpers::FObjectFinder<UVDUIRegistry> UIRegistryObj(*VDConstants::UIRegistryDataAssetPath);
	
	if (UIRegistryObj.Succeeded())
	{
		UIRegistry = UIRegistryObj.Object;
	}
}

void UVDUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVDUISubsystem::Deinitialize()
{

}

TSoftClassPtr<UUserWidget> UVDUISubsystem::GetUIWidgetClassPathByName(const FName& WidgetName)
{
	if (UIRegistry.IsValid())
	{
		return UIRegistry->GetWidgetClassByName(WidgetName);
	}

	return nullptr;
}

