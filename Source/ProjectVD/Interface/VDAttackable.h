// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VDAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVDAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVD_API IVDAttackable
{
	GENERATED_BODY()

protected:
	uint8 bIsPreAttacking : 1;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetComboInputOn(bool bIsOn);
	virtual void DefaultAttackHit();
	virtual void ResetHitList();

	/// <summary>
	/// 스킬 사용시 애님 노티파이에서 호출
	/// </summary>
	/// <param name="SkillIndex">스킬인덱스</param>
	/// <param name="SkillAttackCount">스킬애니메이션에서 해당 스킬이 몇번 공격하는지 구분자</param>
	virtual void SkillAttackHit(int32 SkillIndex, int32 SkillAttackCount = 0);

	/// <summary>
	/// 스킬 사용시 공격준비 상태 설정 (퍼펙트 막기용)
	/// </summary>
	/// <param name="bIsOn"></param>
	virtual void SetPreAttackingState(bool bIsOn);

};
