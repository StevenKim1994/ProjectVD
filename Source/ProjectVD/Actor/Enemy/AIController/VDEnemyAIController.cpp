// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

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
