// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (AllowPrivateAccess = true)) // TODO :: 추후 GAS
	uint8 SkillIndex;
	
public:
	UBTTask_UseSkill();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
