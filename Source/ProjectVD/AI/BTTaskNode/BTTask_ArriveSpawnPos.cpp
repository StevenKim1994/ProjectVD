// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_ArriveSpawnPos.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Public/VDBlackboardInfo.h"

EBTNodeResult::Type UBTTask_ArriveSpawnPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(VDBB_KEY_RETURN_TO_SPAWNPOS, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(VDBB_KEY_TARGET, nullptr);

    return Result;
}
