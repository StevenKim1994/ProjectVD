// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaSoundComponent.h"
#include "VDTitleLevelActor.generated.h"

UCLASS()
class PROJECTVD_API AVDTitleLevelActor : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true") )
	TObjectPtr<class AVDTitleMovieActor> TitleMovieActor;

	UFUNCTION(BlueprintCallable, Category = "LevelActor", meta = (AllowPrivateAccess = "true"))
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "LevelActor", meta = (AllowPrivateAccess = "true"))
	void InitializeTitleLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelActor", meta = (AllowPrivateAccess = "true"))
	void ShowTitleUI();

	UFUNCTION(BlueprintCallable, Category = "LevelActor", meta = (AllowPrivateAccess = "true"))
	void LoadTitleMovieResources();

public:
	AVDTitleLevelActor();

	UFUNCTION(BlueprintCallable, Category = "LevelActor")
	void SetToggleBackgroundMediaSoundMute(bool bIsMute);
};
