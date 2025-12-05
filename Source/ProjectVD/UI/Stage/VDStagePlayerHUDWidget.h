// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStagePlayerHUDWidget.generated.h"

class UVDPlayerHUDEnemyStatusWidget;
class UVDStagePlayerHUDToastWidget;
class UVDPlayerHUDStateWidget;
class UVDHUDInteractionWidget;
class UVDHUDStaminaWidget;
class AVDCharacterBase;
class AVDEnemyCharacterBase;

UCLASS()
class PROJECTVD_API UVDStagePlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta =(BindWidget, FieldNotify))
	TObjectPtr<UVDPlayerHUDEnemyStatusWidget> BossState;

	UPROPERTY(meta = (BineWidget))
	TObjectPtr<UVDPlayerHUDStateWidget> State;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVDHUDStaminaWidget> StaminaIndicator;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVDHUDInteractionWidget> InteractionWidget;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitializeWidget();

public:
	void ShowBossStatus(AVDEnemyCharacterBase* Boss);
	void SetCharacter(AVDCharacterBase* Character);
	void ShowInteractionWidget(const FText& InText, const FText& InKeyText);
	void HideInteractionWidget();
	void HideBossStatus();
};
