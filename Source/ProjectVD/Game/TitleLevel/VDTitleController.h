// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VDTitleController.generated.h"

class UMediaSource;
class UMediaPlayer;
class UMediaTexture;
class AVDTitleMovieActor;
class UVDTitlePanelUserWidget;
UCLASS()
class PROJECTVD_API AVDTitleController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TWeakObjectPtr<UVDTitlePanelUserWidget> TitlePanelUserWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true"))
	UMediaSource* TitleBackgroundMediaSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true"))
	UMediaPlayer* TitleBackgroundMediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true"))
	UMediaTexture* TitleBackgroundMediaTexture;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AVDTitleMovieActor> TitleMovieActor;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetTitleBackgroundMovie(bool Pause = false);

	UFUNCTION(BlueprintCallable)
	void SetTitleMovieSoundMute(bool bMute = false);

public:
	AVDTitleController();

};
