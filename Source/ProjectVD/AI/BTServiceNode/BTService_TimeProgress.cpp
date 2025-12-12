// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceNode/BTService_TimeProgress.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_TimeProgress::UBTService_TimeProgress()
{
	NodeName = "Time Progress";

	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_TimeProgress, BlackboardKey));
}

void UBTService_TimeProgress::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp)
	{
		float CurrentTime = BlackboardComp->GetValueAsFloat(BlackboardKey.SelectedKeyName);
		CurrentTime += DeltaSeconds;
		BlackboardComp->SetValueAsFloat(BlackboardKey.SelectedKeyName, CurrentTime);
	}
}
