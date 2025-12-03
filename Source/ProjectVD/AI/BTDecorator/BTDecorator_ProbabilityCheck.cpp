// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_ProbabilityCheck.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"

UBTDecorator_ProbabilityCheck::UBTDecorator_ProbabilityCheck()
{
}

bool UBTDecorator_ProbabilityCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	const float RandomValue = FMath::FRandRange(0.0f, 1.0f);

	if (RandomValue <= Probability)
	{
		return true;
	}
	else
	{
		return false;
	}
}
