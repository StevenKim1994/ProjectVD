// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_UseSkillAttackNotify.h"
#include "Interface/VDAttackable.h"

void UAnimNotify_UseSkillAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (IVDAttackable* AttackAble = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		AttackAble->SkillAttackHit(SkillIndex, SkillAttackCount);
	}
}
