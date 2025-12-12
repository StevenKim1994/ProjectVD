// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TimeProgress.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTService_TimeProgress : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	

protected:

public:
	UBTService_TimeProgress();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
