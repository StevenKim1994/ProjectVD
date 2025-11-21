// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VDLoadingLevelGameMode.generated.h"

/**
 * 
 */
class AVDLoadingController;

UCLASS()
class PROJECTVD_API AVDLoadingLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
	TWeakObjectPtr<AVDLoadingController> LoadingController;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void OnLoadingProgressUpdated(float Progress);
	void OnLoadingFinished();
	void OnLoadingStarted();
};
