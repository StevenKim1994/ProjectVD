// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "UI/Global/VDLoadingPanelUserWidget.h"
#include "VDLevelSystem.generated.h"

class UVDUISubSystem;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UVDLevelSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	FString CurrentLevelName;
	FString NextLevelName;

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	void ChangeLevelByName(const FString& LevelName);
	void OnLevelLoaded();
};
