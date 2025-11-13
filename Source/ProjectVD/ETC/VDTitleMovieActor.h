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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MediaComponents", meta = (AllowPrivateAccess ="true"))
	UMediaPlayer* TitleMoviePlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MediaComponents", meta = (AllowPrivateAccess = "true"))
	UMediaSoundComponent* TitleMovieSoundComponent;

public:	
	// Sets default values for this actor's properties
	AVDTitleMovieActor();

protected:
	// Called when the game starts or when spawned

	UFUNCTION(BlueprintCallable, Category = "MediaComponents", meta = (AllowPrivateAccess = "true"))
	virtual void BeginPlay() override;
	
public:
	void SetChangeState(bool ChangeState);

	void SetPlayTitleMovie(UMediaSource* MovieMedia);
	void SetPauseTitleMovie();
};

