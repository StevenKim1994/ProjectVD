// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_PatrolPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Interface/VDPatrolPositionProvider.h"
#include "AIController.h"

UBTTask_SetNextPatrolIndex::UBTTask_SetNextPatrolIndex()
{
	NodeName = TEXT("Set Next Patrol Position Index");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_SetNextPatrolIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!AIController)
	{
		return EBTNodeResult::Failed;
	}

	IVDPatrolPositionProvider* PatrolProvider = Cast<IVDPatrolPositionProvider>(AIController->GetPawn());
	if(!PatrolProvider)
	{
		return EBTNodeResult::Failed;
	}

	int CurrentPatrolIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt("CurrentPatrolIndex");

	if(CurrentPatrolIndex >= PatrolProvider->GetPatrolPosition().Num())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("CurrentPatrolIndex", 0);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPosition", PatrolProvider->GetPatrolPosition()[0]);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("CurrentPatrolIndex", CurrentPatrolIndex + 1);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPosition", PatrolProvider->GetPatrolPosition()[CurrentPatrolIndex]);
	}
	
	return EBTNodeResult::Succeeded;
}
