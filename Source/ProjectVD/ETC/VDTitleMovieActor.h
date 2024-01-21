// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "VDTitleMovieActor.generated.h"

UCLASS()
class PROJECTVD_API AVDTitleMovieActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMediaPlayer* TitleMoviePlayer;

	UPROPERTY()
	UMediaSoundComponent* TitleMovieSoundComponent;

	UPROPERTY()
	UMediaSource* TitleMovieSource;
	
public:	
	// Sets default values for this actor's properties
	AVDTitleMovieActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetChangeState(bool ChangeState);
};
