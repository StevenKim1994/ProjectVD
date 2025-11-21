// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDAnimNotifyStateForComboAttack.h"
#include "Interface/VDAttackable.h"

void UVDAnimNotifyStateForComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetComboInputOn(true);
	}
}

void UVDAnimNotifyStateForComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetComboInputOn(false);
	}
}
