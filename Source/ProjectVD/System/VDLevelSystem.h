// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "UI/Global/VDLoadingPanelUserWidget.h"
#include "VDLevelSystem.generated.h"

DECLARE_DELEGATE(FOnLevelLoadedCompleteDelegate);
DECLARE_DELEGATE_OneParam(FOnLevelLoadedDelegate, float);

class UPrimaryAssetLabel;
struct FStreamableHandle;
UCLASS()
class PROJECTVD_API UVDLevelSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	UVDLevelSystem();
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	void LoadPrepareNextLevelAssets();
	void LoadChangeLevel();

	void ChangeLevelByName(const FString& LevelName);
	void OnLevelLoaded();
	void ChangeToNextLevel();

	FORCEINLINE FOnLevelLoadedDelegate& GetLevelLoadedDelegate() { return LevelLoadedDelegate; }
	FORCEINLINE FOnLevelLoadedCompleteDelegate& GetLevelLoadedCompleteDelegate() { return LevelLoadedCompleteDelegate; }


private:
	TMap<FPrimaryAssetId, TWeakObjectPtr<UPrimaryAssetLabel>> LevelPrepareAssetsMap;

	UFUNCTION()
	void OnLoadSingleAsset();
	int32 CurrentCount = 0;
	int32 AssetNum = 0;

	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FString NextLevelName;

	UPROPERTY()
	TWeakObjectPtr<UVDLoadingPanelUserWidget> LoadingPanelWidget;

	TSharedPtr<FStreamableHandle> LevelStreamableHandle;

	FOnLevelLoadedDelegate LevelLoadedDelegate;
	FOnLevelLoadedCompleteDelegate LevelLoadedCompleteDelegate;
};
