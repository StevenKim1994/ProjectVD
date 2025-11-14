#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VDTitleController.generated.h"

class UMediaSource;
class UMediaPlayer;
class UMediaTexture;
class UMediaSoundComponent;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess = "true"))
	UMediaSoundComponent* TitleBackgroundMediaSoundComponent;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetTitleBackgroundMovie(bool Pause = false);

	UFUNCTION(BlueprintCallable)
	void SetTitleMovieSoundMute(bool bMute = false);

public:
	AVDTitleController();
};
