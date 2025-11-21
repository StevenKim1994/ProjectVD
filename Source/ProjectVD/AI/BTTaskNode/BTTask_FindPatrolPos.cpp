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
	
	FVector SpawnPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(VDBB_KEY_SPAWN_POS);
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(VDBB_KEY_PATROL_START_POS);
	float PatrolRadius = EnemyInterface->GetPatrolRadius();
	FNavLocation NextPatrolPos;

	// 최대 시도 횟수 설정 (무한 루프 방지)
	const int32 MaxAttempts = 10;
	for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
	{
		if (NS->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
		{
			// 스폰 위치와의 거리 검사
			float DistanceFromSpawn = FVector::Dist(NextPatrolPos.Location, SpawnPos);
			if (DistanceFromSpawn <= PatrolRadius)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(VDBB_KEY_PATROL_END_POS, NextPatrolPos.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	// 모든 시도가 실패한 경우, 스폰 위치를 기준으로 한 번 더 시도
	if (NS->GetRandomPointInNavigableRadius(SpawnPos, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(VDBB_KEY_PATROL_END_POS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
