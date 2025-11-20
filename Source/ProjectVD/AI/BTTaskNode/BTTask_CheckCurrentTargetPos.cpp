// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_CheckCurrentTargetPos.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Public/VDBlackboardInfo.h"

EBTNodeResult::Type UBTTask_CheckCurrentTargetPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	if (nullptr == OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(VDBB_KEY_TARGET));
	if (nullptr == TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(VDBB_KEY_TARGET_POS, TargetActor->GetActorLocation());

	return Result;
}

UBTTask_CheckCurrentTargetPos::UBTTask_CheckCurrentTargetPos()
{

}
