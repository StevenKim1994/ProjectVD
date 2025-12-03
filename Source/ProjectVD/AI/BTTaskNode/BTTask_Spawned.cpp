// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_Spawned.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Public/VDBlackboardInfo.h"
#include "Interface/VDEnemyInterface.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_Spawned::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (nullptr == OwnerComp.GetOwner())
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsVector(VDBB_KEY_SPAWN_POS, OwnerComp.GetOwner()->GetActorLocation());
	BlackboardComp->SetValueAsBool(VDBB_KEY_IS_SPAWNED, true);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = (nullptr != AIController) ? AIController->GetPawn() : nullptr;
	if (ControlledPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(ControlledPawn);
	if (EnemyInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UVDEnemyStatsBaseComponent* StatsComp = EnemyInterface->GetStatsComp();
	BlackboardComp->SetValueAsFloat(VDBB_KEY_PARTOL_RANGE, StatsComp->GetPatrolRange());
	BlackboardComp->SetValueAsFloat(VDBB_KEY_PATROL_WAIT_TIME, StatsComp->GetPatrolWaitTime());
	BlackboardComp->SetValueAsFloat(VDBB_KEY_ATTACK_RANGE, StatsComp->GetAttackRange());
	BlackboardComp->SetValueAsFloat(VDBB_KEY_FIND_PLAYER_RANGE, StatsComp->GetFindPlayerRange());

	return Result;
}
