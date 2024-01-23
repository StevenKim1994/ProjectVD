// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VDStageGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDStageGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVDStageGameMode();

	virtual void StartPlay() override;
};
