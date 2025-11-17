// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDStagePlayerHUDWidget.generated.h"

class UVDPlayerHUDEnemyStatusWidget;
class UVDStagePlayerHUDToastWidget;
UCLASS()
class PROJECTVD_API UVDStagePlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UVDPlayerHUDEnemyStatusWidget> BossState;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UVDStagePlayerHUDToastWidget> Toast;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void ShowToast(const FString& InTitle, const FString& InMessage);
	void ShowBossStatus(AActor* Boss);
	void HideBossStatus();
};
