// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceNode/BTService_HPCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"

UBTService_HPCheck::UBTService_HPCheck()
{
	NodeName = TEXT("HP Check Service");
	Interval = 1.0f;
}

void UBTService_HPCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}

	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	bool bIsCriticalHP = false;

	if (ControlledPawn != nullptr)
	{
		const UVDEnemyStatsBaseComponent* StatsComponent = ControlledPawn->FindComponentByClass<UVDEnemyStatsBaseComponent>();
		if (StatsComponent != nullptr)
		{
			const float MaxHealth = StatsComponent->GetMaxHealth();
			if (MaxHealth > KINDA_SMALL_NUMBER)
			{
				const float CurrentRatio = StatsComponent->GetHealth() / MaxHealth;
				bIsCriticalHP = CurrentRatio <= CriticalHPPercent;
			}
		}
	}

	BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, bIsCriticalHP);

}

