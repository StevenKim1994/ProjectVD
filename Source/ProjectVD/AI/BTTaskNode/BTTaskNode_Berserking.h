// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Berserking.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTTaskNode_Berserking : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_Berserking();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
