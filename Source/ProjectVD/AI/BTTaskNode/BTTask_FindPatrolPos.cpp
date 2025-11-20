// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_FindPatrolPos.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Public/VDBlackboardInfo.h"
#include "Interface/VDEnemyInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{

}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(ControlledPawn);
	if (nullptr == EnemyInterface)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NS = UNavigationSystemV1::GetNavigationSystem(ControlledPawn->GetWorld());
	if (nullptr == NS)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(VDBB_KEY_PATROL_START_POS);
	FNavLocation NextPatrolPos;

	if (NS->GetRandomPointInNavigableRadius(Origin, EnemyInterface->GetPatrolRadius(), NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(VDBB_KEY_PATROL_END_POS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
