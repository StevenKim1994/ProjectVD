// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_UseSkillAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UAnimNotify_UseSkillAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", Meta = (AllowPrivateAccess = true))
	int32 SkillIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", Meta = (AllowPrivateAccess = true))
	int32 SkillAttackCount;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
