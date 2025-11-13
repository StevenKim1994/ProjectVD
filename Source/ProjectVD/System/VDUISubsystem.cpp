// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/AssetManager.h"
#include "Public/VDConstrants.h"
#include "DataAsset/VDUIRegistry.h"
#include "Game/VDGameInstance.h"
#include "System/VDUISubsystem.h"
#include "System/VDResourceSystem.h"
#include "Blueprint/UserWidget.h"
#include "UI/Title/VDTitlePanelUserWidget.h"

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
	Super::Initialize(Collection);

	ResourceSystem = Collection.InitializeDependency<UVDResourceSystem>();
}

void UVDUISubsystem::Deinitialize()
{
	Super::Deinitialize();
	ResourceSystem = nullptr;
}

void UVDUISubsystem::ShowUIWidget(APlayerController* PlayerController, const FName& WidgetName)
{
	if (!PlayerController)
	{
		return;
	}

	if (!ResourceSystem)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			ResourceSystem = GI->GetSubsystem<UVDResourceSystem>();
		}
	}
	if (!ensure(ResourceSystem)) return;

	TSoftClassPtr<UUserWidget> WidgetClassPtr = GetUIWidgetClassPathByName(WidgetName);
	if (WidgetClassPtr.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget not found in UIRegistry: %s"), *WidgetName.ToString());
		return;
	}

	if (CachedWidgetClassMap.Contains(WidgetName))
	{
		if (UClass* CachedClass = CachedWidgetClassMap[WidgetName].Get())
		{
			if (UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController, CachedClass))
			{
				Widget->AddToViewport();
			}
		}
	}
	else
	{ 
		const FSoftClassPath ClassPath(WidgetClassPtr.ToString());
		ResourceSystem->LoadResourceAsync(
			ClassPath,
			[this, PlayerController, WidgetName](UClass* LoadedClass)
			{
				if (!LoadedClass)
				{
					return;
				}

				if (UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController, LoadedClass))
				{
					CachedWidgetClassMap.Add(WidgetName, TSoftClassPtr<UUserWidget>(LoadedClass));
					Widget->AddToViewport();
				}
			});
	}
}

TSoftClassPtr<UUserWidget> UVDUISubsystem::GetUIWidgetClassPathByName(const FName& WidgetName)
{
	if (UIRegistry.IsValid())
	{
		return UIRegistry->GetWidgetClassByName(WidgetName);
	}
	return nullptr;
}

template<typename TWidget>
TSubclassOf<TWidget> UVDUISubsystem::GetCachedWidgetClass(const FName& WidgetName, bool bLoadSyncIfNeeded)
{
	static_assert(TIsDerivedFrom<TWidget, UUserWidget>::IsDerived, "TWidget must derive from UUserWidget");

	const TSoftClassPtr<UUserWidget>* Found = CachedWidgetClassMap.Find(WidgetName);
	TSoftClassPtr<UUserWidget> SoftClass = Found ? *Found : TSoftClassPtr<UUserWidget>();

	if (SoftClass.IsNull() && UIRegistry.IsValid())
	{
		SoftClass = UIRegistry->GetWidgetClassByName(WidgetName);
		if (!SoftClass.IsNull())
		{
			CachedWidgetClassMap.Add(WidgetName, SoftClass);
		}
	}

	UClass* RawClass = SoftClass.Get();
	if (!RawClass && bLoadSyncIfNeeded)
	{
		RawClass = SoftClass.LoadSynchronous();
		if (RawClass)
		{
			CachedWidgetClassMap.Add(WidgetName, TSoftClassPtr<UUserWidget>(RawClass));
		}
	}

	return TSubclassOf<TWidget>(RawClass);
}
