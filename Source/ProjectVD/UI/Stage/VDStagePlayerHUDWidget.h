// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStagePlayerHUDWidget.generated.h"

class UVDPlayerHUDEnemyStatusWidget;
class UVDStagePlayerHUDToastWidget;
class UVDPlayerHUDStateWidget;
class UVDCharacterStatsBaseComponent;

UCLASS()
class PROJECTVD_API UVDStagePlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UVDPlayerHUDEnemyStatusWidget> BossState;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UVDStagePlayerHUDToastWidget> Toast;

	UPROPERTY(meta = (BineWidget))
	TObjectPtr<UVDPlayerHUDStateWidget> State;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitializeWidget();

public:
	void ShowToast(const FString& InTitle, const FString& InMessage);
	void ShowBossStatus(AActor* Boss);
	void SetCharacterState(UVDCharacterStatsBaseComponent* StatsBaseComponent);
	void HideBossStatus();
};
