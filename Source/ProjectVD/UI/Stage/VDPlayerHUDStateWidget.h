// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDStateWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UVDCharacterStatsBaseComponent;

UCLASS()
class PROJECTVD_API UVDPlayerHUDStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCharacterState(UVDCharacterStatsBaseComponent* BaseStats);

protected:

private:

	struct FTweenInfo
	{
		float StartPercent;
		float TargetPercent;
		float ElapsedTime;
		float DurationTime;
		bool bIsPlaying;
		FTweenInfo()
			: StartPercent(0.0f)
			, TargetPercent(0.0f)
			, ElapsedTime(0.0f)
			, DurationTime(1.0f)
			, bIsPlaying(false)
		{}
	};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBarTween;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPBarText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> MPBarTween;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> MPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MPBarText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	FTweenInfo HPTween;
	FTweenInfo MPTween;

	FTimerHandle HPTweenHandle;
	FTimerHandle MPTweenHandle;

	void SetHPBarPercent(float CurrentHP, float MaxHP);
	void SetMPBarPercent(float CurrentMP, float MaxMP);

	void UpdateHPBarTween();
	void UpdateMPBarTween();
};
