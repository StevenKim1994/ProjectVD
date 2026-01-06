// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_PreAttacking.h"
#include "Interface/VDAttackable.h"

void UAnimNotifyState_PreAttacking::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetPreAttackingState(true);
	}
}

void UAnimNotifyState_PreAttacking::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IVDAttackable* Attackable = Cast<IVDAttackable>(MeshComp->GetOwner()))
	{
		Attackable->SetPreAttackingState(false);
	}
}
