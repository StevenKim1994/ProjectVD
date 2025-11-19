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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HeavyAttackAM;

protected:

public:
	AVDEnemyGrux();
};
