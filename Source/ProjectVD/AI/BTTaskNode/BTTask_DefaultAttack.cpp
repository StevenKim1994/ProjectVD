// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_DefaultAttack.h"
#include "BehaviorTree/BehaviorTree.h"	
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"	
#include "Public/VDBlackboardInfo.h"
#include "Interface/VDEnemyInterface.h"
#include "Actor/Character/VDCharacterBase.h"

UBTTask_DefaultAttack::UBTTask_DefaultAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(VDBB_KEY_TARGET);
	float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(VDBB_KEY_ATTACK_RANGE);
	if (Target)
	{
		AVDCharacterBase* TargetCharacter = Cast<AVDCharacterBase>(Target);
		if (TargetCharacter)
		{
			if (ControlledPawn->GetDistanceTo(TargetCharacter) > AttackRange)
			{
				return EBTNodeResult::Failed;
			}
			else
			{
				EnemyInterface->DefaultAttackMontagePlay();
				return EBTNodeResult::Succeeded;
			}
		}
		return EBTNodeResult::Failed;
	}


	return Result;
}
