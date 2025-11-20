// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()

private:
	APawn* DetectedTargetActor(UBehaviorTreeComponent& OwnerComp, const TArray<FOverlapResult>& OverlapResult);

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTService_DetectTarget();
};
