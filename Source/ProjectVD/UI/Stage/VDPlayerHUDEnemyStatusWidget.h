// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VDPlayerHUDEnemyStatusWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class PROJECTVD_API UVDPlayerHUDEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> BossNameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> BossHealthBar;
	
	UPROPERTY()
	TWeakObjectPtr<AActor> BossActor;

public:
	void SetBossActor(AActor* Boss);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
