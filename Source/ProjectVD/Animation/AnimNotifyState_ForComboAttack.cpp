// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_ForComboAttack.h"
#include "Interface/VDAttackable.h"

void UAnimNotifyState_ForComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetComboInputOn(true);
	}
}

void UAnimNotifyState_ForComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetComboInputOn(false);
	}
}
