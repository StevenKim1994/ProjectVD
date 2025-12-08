// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "VDEnemySkeleton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDEnemySkeleton : public AVDEnemyCharacterBase
{
	GENERATED_BODY()

protected:
	virtual void DefaultAttack() override;
};
