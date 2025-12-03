// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ProbabilityCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTDecorator_ProbabilityCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ProbabilityCheck();

	UPROPERTY(EditAnywhere, Category = "Probability")
	float Probability = 0.5f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
