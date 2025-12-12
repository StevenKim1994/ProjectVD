// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolPosition.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTVD_API UBTTask_SetNextPatrolIndex : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	
public:
	UBTTask_SetNextPatrolIndex();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
