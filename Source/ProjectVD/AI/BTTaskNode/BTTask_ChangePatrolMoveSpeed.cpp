// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_ChangePatrolMoveSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"
#include "Interface/VDEnemyInterface.h"

EBTNodeResult::Type UBTTask_ChangePatrolMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* AIOwnerActor = OwnerComp.GetAIOwner();
	if (!AIOwnerActor)
	{
		return EBTNodeResult::Failed;
	}

	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(AIOwnerActor);
	if (!EnemyInterface)
	{
		return EBTNodeResult::Failed;
	}

	EnemyInterface->ChangePatrolMoveSpeed();

	return EBTNodeResult::Succeeded;
}
