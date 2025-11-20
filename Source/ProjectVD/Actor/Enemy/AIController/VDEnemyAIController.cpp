// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Public/VDBlackboardInfo.h"
#include "Interface/VDEnemyInterface.h"

AVDEnemyAIController::AVDEnemyAIController()
{

}

void AVDEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}


void AVDEnemyAIController::RunAI()
{
	if(BTAsset)
	{
		if (IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(GetPawn()))
		{
			UBlackboardComponent* BlackboardPtr = Blackboard.Get();
			if (UseBlackboard(BBAsset, BlackboardPtr))
			{
				if (!BlackboardPtr->GetValueAsBool(VDBB_KEY_IS_SPAWNED))
				{
					BlackboardPtr->SetValueAsFloat(VDBB_KEY_FIND_PLAYER_RANGE, EnemyInterface->GetFindingRange());
					BlackboardPtr->SetValueAsFloat(VDBB_KEY_PATROL_WAIT_TIME, EnemyInterface->GetPatrolWaitTime());
					BlackboardPtr->SetValueAsFloat(VDBB_KEY_TURN_SPEED, EnemyInterface->GetTurnSpeed());
					BlackboardPtr->SetValueAsFloat(VDBB_KEY_PARTOL_RANGE, EnemyInterface->GetPatrolRadius());
				}

				BlackboardPtr->SetValueAsFloat(VDBB_KEY_ATTACK_RANGE, EnemyInterface->GetAttackRange());
				BlackboardPtr->SetValueAsVector(VDBB_KEY_PATROL_START_POS, GetPawn()->GetActorLocation());

				RunBehaviorTree(BTAsset);
			}
		}
	}
}

void AVDEnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		BTComponent->StopTree(EBTStopMode::Safe);
	}

}
