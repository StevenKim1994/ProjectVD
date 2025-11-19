// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Interface/VDAttackable.h"
#include "VDEnemyGrux.generated.h"

class UAnimMontage;

UCLASS()
class PROJECTVD_API AVDEnemyGrux : public AVDEnemyCharacterBase, public IVDAttackable
{
	GENERATED_BODY()
	
private:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	AVDEnemyGrux();

	virtual void SetComboInputOn_Implementation(bool bIsOn) override;
	virtual void DefaultAttackHit_Implementation() override;
};
