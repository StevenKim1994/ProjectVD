// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTaskNode/BTTask_PrepareAnimMontagePlay.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/VDEnemyAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "Interface/VDEnemyInterface.h"
#include "Public/VDBlackboardInfo.h"

UBTTask_PrepareAnimMontagePlay::UBTTask_PrepareAnimMontagePlay()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_PrepareAnimMontagePlay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AVDEnemyAIController* AIController = Cast<AVDEnemyAIController>(OwnerComp.GetAIOwner());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(AIPawn);
	if (!EnemyInterface)
	{
		return EBTNodeResult::Failed;
	}

	UAnimMontage* AnimMontage = EnemyInterface->GetFindPlayerAnimMontage();
	UVDEnemyAnimInstance* AnimInstance = EnemyInterface->PrepareAnimMontagePlay();

	if ((nullptr == AnimMontage) || (nullptr == AnimInstance))
	{
		return EBTNodeResult::Failed;
	}

	ClearEndDelegate();
	CachedMontage = AnimMontage;
	CachedAnimInstance = AnimInstance;
	CachedOwnerComp = &OwnerComp;

	// 몽타주 재생
	const float PlayedLen = AnimInstance->Montage_Play(AnimMontage, PlayRate);
	if (PlayedLen <= 0.f)
	{
		ClearEndDelegate();
		return EBTNodeResult::Failed;
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UBTTask_PrepareAnimMontagePlay::HandleMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AnimMontage);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PrepareAnimMontagePlay::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	UVDEnemyAnimInstance* AnimInstance = CachedAnimInstance.Get();
	UAnimMontage* Montage = CachedMontage.Get();

	if (AnimInstance && Montage)
	{
		// 델리게이트 해제 먼저
		ClearEndDelegate();

		if (bStopWhenAborted)
		{
			AnimInstance->Montage_Stop(AbortBlendOutTime, Montage);
		}
	}

	CachedAnimInstance = nullptr;
	CachedMontage = nullptr;
	CachedOwnerComp = nullptr;

	return EBTNodeResult::Aborted;
}

void UBTTask_PrepareAnimMontagePlay::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	ClearEndDelegate();
	CachedAnimInstance = nullptr;
	CachedMontage = nullptr;
	CachedOwnerComp = nullptr;

	Super::OnTaskFinished(OwnerComp,NodeMemory, TaskResult);
}

void UBTTask_PrepareAnimMontagePlay::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != CachedMontage.Get())
	{
		return;
	}

	UBehaviorTreeComponent* BTC = CachedOwnerComp.Get();
	if (!BTC)
	{
		ClearEndDelegate();
		return;
	}

	ClearEndDelegate();

	const EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
	FinishLatentTask(*BTC, Result);
}

void UBTTask_PrepareAnimMontagePlay::ClearEndDelegate()
{
	if (UVDEnemyAnimInstance* AnimInstance = CachedAnimInstance.Get())
	{
		if (UAnimMontage* Montage = CachedMontage.Get())
		{
			// 해당 몽타주에 설정된 엔드 델리게이트만 리셋
			FOnMontageEnded EmptyDelegate;
			AnimInstance->Montage_SetEndDelegate(EmptyDelegate, Montage);
		}
	}
}
