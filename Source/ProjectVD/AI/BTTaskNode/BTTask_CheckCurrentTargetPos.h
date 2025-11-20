// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckCurrentTargetPos.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTTask_CheckCurrentTargetPos : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTask_CheckCurrentTargetPos();
};
