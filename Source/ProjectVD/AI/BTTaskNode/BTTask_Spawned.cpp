// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_Spawned.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Public/VDBlackboardInfo.h"

EBTNodeResult::Type UBTTask_Spawned::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (nullptr == OwnerComp.GetOwner())
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(VDBB_KEY_IS_SPAWNED, true);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(VDBB_KEY_SPAWN_POS, OwnerComp.GetOwner()->GetActorLocation());
	return Result;
}
