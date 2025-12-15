// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDEnemyInterface.generated.h"

DECLARE_DELEGATE(FOnAttackMontageEnded);
DECLARE_DELEGATE(FOnSkillUsedEnded);

class UVDEnemyAnimInstance;
class UVDEnemyStatsBaseComponent;
class UAnimMontage;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnBerserkingModeChanged);
/**
 * 
 */
class PROJECTVD_API IVDEnemyInterface
{
	GENERATED_BODY()

public:
	virtual UVDEnemyAnimInstance* PrepareAnimMontagePlay();
	virtual UVDEnemyAnimInstance* DefaultAttackMontagePlay(FOnAttackMontageEnded AttackMontageEndedDelegate);
	virtual UAnimMontage* GetFindPlayerAnimMontage() const;
	virtual UVDEnemyStatsBaseComponent* GetStatsComp() const;
	virtual void SetBerserking(bool InMode, FOnBerserkingModeChanged EndCallback);
	virtual float GetPatrolRadius() const = 0;
	virtual float GetPatrolWaitTime() const = 0;
	virtual float GetFindingRange() const = 0;
	virtual float GetTurnSpeed() const = 0;
	virtual float GetAttackRadius() const = 0;
	virtual float GetAttackRange() const = 0;
	virtual void ChangePatrolMoveSpeed() = 0;
	virtual void ChangeChaseMoveSpeed() = 0;
	virtual void UseSkill(uint8 SkillIndex, FOnSkillUsedEnded SkillUseEndedDelegate) = 0;
};
