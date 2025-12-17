// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Ticker.h"
#include "VDPlayerHUDEnemyStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCurveFloat;
class AVDEnemyCharacterBase;
class UVDEnemyStatsBaseComponent;

UCLASS()
class PROJECTVD_API UVDPlayerHUDEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetTargetEnemy(AVDEnemyCharacterBase* Boss);
	

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	struct FTweenInfo
	{
		float StartPercent;
		float TargetPercent;
		float ElapsedTime;
		float DurationTime;
		bool bIsPlaying;
		double LastRealTime;
		FTweenInfo()
			: StartPercent(0.0f)
			, TargetPercent(0.0f)
			, ElapsedTime(0.0f)
			, DurationTime(1.0f)
			, bIsPlaying(false)
			, LastRealTime(-1.0)
		{}
	};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBarTweenBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr <UTextBlock> HealthBarText;

	UPROPERTY()
	TWeakObjectPtr<AVDEnemyCharacterBase> TargetEnemy;

	FTweenInfo HPTween;

	FTSTicker::FDelegateHandle HPTickerHandle;
	FTSTicker::FDelegateHandle BossHPHideTickerHandle;

	void UpdateEnemyHealthBar(float CurrentHP, float MaxHP);
	bool TickHealthBarTween(float DeltaTime);

	void OnEnemyHPHideTimerExpired();
};
