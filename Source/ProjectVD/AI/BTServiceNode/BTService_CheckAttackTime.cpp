// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceNode/BTService_CheckAttackTime.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Public/VDBlackboardInfo.h"

UBTService_CheckAttackTime::UBTService_CheckAttackTime()
{
	NodeName = "Check Attack Time";
}

void UBTService_CheckAttackTime::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (OwnerComp.GetBlackboardComponent() == nullptr)
	{
		return;
	}

	AttackTime = 0;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsBool(VDBB_KEY_IS_ATTACK_TIME,true);
}

void UBTService_CheckAttackTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AttackTime += DeltaSeconds;
	if (AttackTime >= MaxAttackTime)
	{
		UBlackboardComponent*BlackboardComp = OwnerComp.GetBlackboardComponent();
		BlackboardComp->SetValueAsBool(VDBB_KEY_IS_ATTACK_TIME, false);
	}

}
