// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VDTitleController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDTitleController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<class UVDTitlePanelUserWidget> TitlePanelUserWidgetClass;

	UPROPERTY()
	TWeakObjectPtr<class UVDTitlePanelUserWidget> TitlePanelUserWidget;

	UPROPERTY()
	TObjectPtr <class AVDTitleMovieActor> TitleMovieActor;

public:
	AVDTitleController();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AsyncLevelLoad(const FString& LevelDir, const FString& LevelName);

private:
	void AsyncLevelLoadFinished(const FString LevelName);

};
