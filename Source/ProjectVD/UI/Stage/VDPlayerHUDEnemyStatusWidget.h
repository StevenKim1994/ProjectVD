// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


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

	FTimerHandle BossHPVisibleTimerHandle;
	FTimerHandle TweenTimerHandle;

	void UpdateEnemyHealthBar(float CurrentHP, float MaxHP);
	void UpdateTweenBar();

	void OnEnemyHPHideTimerExpired();
};
