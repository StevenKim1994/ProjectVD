// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "UI/Global/VDLoadingPanelUserWidget.h"
#include "VDLevelSystem.generated.h"

DECLARE_DELEGATE(FOnLevelLoadedCompleteDelegate);
DECLARE_DELEGATE_OneParam(FOnLevelLoadedDelegate, float);
class UVDUISubSystem;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDLevelSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FString NextLevelName;

	FOnLevelLoadedCompleteDelegate LevelLoadedCompleteDelegate;
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	void LoadPrepareNextLevelAssets();
	void LoadChangeLevel();

	void ChangeLevelByName(const FString& LevelName);
	void OnLevelLoaded();

};
