// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceNode/BTService_CheckTargetDistance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Public/VDBlackboardInfo.h"

void UBTService_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(VDBB_KEY_TARGET) == nullptr)
	{
		return;
	}

	float Range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(VDBB_KEY_FIND_PLAYER_RANGE);
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(VDBB_KEY_TARGET));
	if (TargetActor == nullptr)
	{
		return;
	}

	if (TargetActor->GetDistanceTo(OwnerComp.GetOwner()) > Range)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(VDBB_KEY_TARGET, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(VDBB_KEY_RETURN_TO_SPAWNPOS, true);
	}

}
