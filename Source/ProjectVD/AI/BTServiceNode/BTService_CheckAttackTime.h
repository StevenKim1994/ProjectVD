// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTService_CheckAttackTime.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class PROJECTVD_API UBTService_CheckAttackTime : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckAttackTime();

protected:
	float AttackTime;

	UPROPERTY(EditAnywhere, Category = "AttackTime")
	float MaxAttackTime;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
