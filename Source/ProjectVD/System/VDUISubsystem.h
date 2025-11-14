// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h" // for UUserWidget in UPROPERTY
#include "DataAsset/VDUIRegistry.h"
#include "VDUISubsystem.generated.h"

class UVDResourceSystem;
class UVDTitlePanelUserWidget; // forward declaration for extern template
class APlayerController; // forward declare parameter type

UCLASS()
class PROJECTVD_API UVDUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
    UPROPERTY()
    TSoftObjectPtr<UVDUIRegistry> UIRegistry;

    UPROPERTY()
    TObjectPtr<UVDResourceSystem> ResourceSystem;

	UPROPERTY()
	TMap<FName, TSoftClassPtr<UUserWidget>> CachedWidgetClassMap;

public:
	UVDUISubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

    void ShowUIWidgetAsync(APlayerController* PlayerController, const FName& WidgetName);
	UUserWidget* ShowUIWidget(APlayerController* PlayerController, const FName& WidgetName);

	TSoftClassPtr<UUserWidget> GetUIWidgetClassPathByName(const FName& WidgetName);

	template<typename TWidget>
	TSubclassOf<TWidget> GetCachedWidgetClass(const FName& WidgetName, bool bLoadSyncIfNeeded = false);
};
