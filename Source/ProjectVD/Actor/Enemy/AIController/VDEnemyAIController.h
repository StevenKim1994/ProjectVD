// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VDEnemyAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="AIAsset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="AIAsset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BTAsset;

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	AVDEnemyAIController();

	void RunAI();
	void StopAI();
};
