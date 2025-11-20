// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Public/VDBlackboardInfo.h"

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
		UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		if (UseBlackboard(BBAsset, BlackboardPtr))
		{
			if (!BlackboardPtr->GetValueAsBool(VVBB_KEY_IS_SPAWNED))
			{
				BlackboardPtr->SetValueAsVector(VDBB_KEY_SPAWN_POS, GetPawn()->GetActorLocation());
				BlackboardPtr->SetValueAsBool(VVBB_KEY_IS_SPAWNED, true);
			}

			BlackboardPtr->SetValueAsVector(VVBB_KEY_PATROL_START_POS, GetPawn()->GetActorLocation());

			RunBehaviorTree(BTAsset);
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
