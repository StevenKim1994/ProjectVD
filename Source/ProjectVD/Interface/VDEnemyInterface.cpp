// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/VDEnemyInterface.h"
#include "Animation/VDEnemyAnimInstance.h"

// Add default functionality here for any IVDEnemyInterface functions that are not pure virtual.

UVDEnemyAnimInstance* IVDEnemyInterface::PrepareAnimMontagePlay()
{
	// DESC :: 자식에서 오버라이드 해서 사용
	return nullptr;
}

UAnimMontage* IVDEnemyInterface::GetFindPlayerAnimMontage() const
{
	// DESC :: 자식에서 오버라이드 해서 사용
	return nullptr;
}
