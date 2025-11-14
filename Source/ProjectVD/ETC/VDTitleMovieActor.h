// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDTitleMovieActor.generated.h"

class UMediaPlayer;

UCLASS()
class PROJECTVD_API AVDTitleMovieActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MediaComponents", meta = (AllowPrivateAccess = "true"))
	UMediaSoundComponent* TitleMovieSoundComponent;

	virtual void BeginPlay() override;
	
public:
	AVDTitleMovieActor();

	UFUNCTION(BlueprintCallable, Category = "TitleMovieActor")
	void SetTitleMovieSoundMute(bool bMute = false);

	UFUNCTION(BlueprintCallable, Category = "TitleMovieActor")
	void SetTitleMovieMediaPlayer(UMediaPlayer* MediaPlayer);
};

