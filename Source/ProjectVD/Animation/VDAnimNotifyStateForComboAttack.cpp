// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDAnimNotifyStateForComboAttack.h"
#include "Interface/VDAttackComboable.h"

void UVDAnimNotifyStateForComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp->GetOwner()->Implements<UVDAttackComboable>())
	{
		IVDAttackComboable::Execute_SetComboInputOn(MeshComp->GetOwner(), true);
	}
}

void UVDAnimNotifyStateForComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp->GetOwner()->Implements<UVDAttackComboable>())
	{
		IVDAttackComboable::Execute_SetComboInputOn(MeshComp->GetOwner(), false);
	}
}
