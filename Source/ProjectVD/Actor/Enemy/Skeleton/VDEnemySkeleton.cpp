// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/Skeleton/VDEnemySkeleton.h"

void AVDEnemySkeleton::DefaultAttack()
{
	Super::DefaultAttack();

	if (DefaultAttackAM)
	{
		PlayAnimMontage(DefaultAttackAM);
	}
}
