// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PrepareAnimMontagePlay.generated.h"

class UVDEnemyAnimInstance;
class UAnimMontage;

UCLASS()
class PROJECTVD_API UBTTask_PrepareAnimMontagePlay : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PrepareAnimMontagePlay();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	// 몽타주 종료시 호출
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 설정값
	UPROPERTY(EditAnywhere, Category="Montage")
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category="Montage")
	bool bStopWhenAborted = true;

	UPROPERTY(EditAnywhere, Category="Montage")
	float AbortBlendOutTime = 0.2f;

private:
	// 실행 중 캐시
	TWeakObjectPtr<UVDEnemyAnimInstance> CachedAnimInstance;
	TWeakObjectPtr<UAnimMontage> CachedMontage;
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	// 델리게이트 해제
	void ClearEndDelegate();
};
