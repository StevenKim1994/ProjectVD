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
	virtual uint8 IsPreAttacking() const; // DESC :: 0 : 공격준비중이 아님, 1: 공격준비중 (캐릭터가 해당 값을 참조해서 퍼펙트 막기 가능)
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

	virtual void ShowPerfectDodgeNotify();
	virtual void HidePerfectDodgeNotify();
};
