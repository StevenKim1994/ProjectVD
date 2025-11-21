// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDEnemyStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;
class AVDEnemyCharacterBase;

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

	void UpdateBossHealthBar(UVDBaseStatsComponent* BossStats);
public:
	void SetBossActor(AVDEnemyCharacterBase* Boss);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
