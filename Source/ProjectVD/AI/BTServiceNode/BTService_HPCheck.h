// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_HPCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVD_API UBTService_HPCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "HP Check", Meta = (AllowPrivateAccess = true))
	float CriticalHPPercent = 0.3f;

public:
	UBTService_HPCheck();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
