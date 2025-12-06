// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDEnemyStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;
class AVDEnemyCharacterBase;
class UVDEnemyStatsBaseComponent;

UCLASS()
class PROJECTVD_API UVDPlayerHUDEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UTextBlock> BossNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> BossHealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr <UTextBlock> BossHealthBarText;

	UPROPERTY()
	TWeakObjectPtr<AVDEnemyCharacterBase> BossActor;

	float TargetBossHPPercent;
	bool bIsBossHPTweenPlaying;

	void UpdateBossHealthBar(float CurrentHP, float MaxHP);
public:
	void SetBossActor(AVDEnemyCharacterBase* Boss);
	

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
