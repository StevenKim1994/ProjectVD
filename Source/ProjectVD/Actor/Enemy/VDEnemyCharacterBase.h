// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDEnemyCharacterBase.generated.h"

UCLASS()
class PROJECTVD_API AVDEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AVDEnemyCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
