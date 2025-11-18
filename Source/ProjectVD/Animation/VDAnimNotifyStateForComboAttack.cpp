// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDAnimNotifyStateForComboAttack.h"
#include "Actor/Character/VDStagePlayerCharacter.h"

void UVDAnimNotifyStateForComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AVDStagePlayerCharacter* Player = Cast<AVDStagePlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->SetComboPossible(true);
		UE_LOG(LogTemp, Log, TEXT("Combo Input On"));
	}
}

void UVDAnimNotifyStateForComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AVDStagePlayerCharacter* Player = Cast<AVDStagePlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->SetComboPossible(false);
		UE_LOG(LogTemp, Log, TEXT("Combo Input Off"));
	}
}
