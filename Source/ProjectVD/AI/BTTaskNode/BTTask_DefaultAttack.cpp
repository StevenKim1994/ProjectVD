// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTaskNode/BTTask_DefaultAttack.h"
#include "BehaviorTree/BehaviorTree.h"	
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"	
#include "Public/VDBlackboardInfo.h"
#include "Interface/VDEnemyInterface.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Animation/VDEnemyAnimInstance.h"

UBTTask_DefaultAttack::UBTTask_DefaultAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (nullptr == ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(ControlledPawn);
	if (nullptr == EnemyInterface)
	{
		return EBTNodeResult::Failed;
	}

	UBehaviorTreeComponent& BTComp = OwnerComp;
	UBlackboardComponent* BlackboardComp = BTComp.GetBlackboardComponent();
	if (nullptr == BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UObject* Target = BlackboardComp->GetValueAsObject(VDBB_KEY_TARGET);

	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	const float AttackRange = BlackboardComp->GetValueAsFloat(VDBB_KEY_ATTACK_RANGE);
	AVDCharacterBase* TargetCharacter = Cast<AVDCharacterBase>(Target);
	if (TargetCharacter)
	{
		if (ControlledPawn->GetDistanceTo(TargetCharacter) > AttackRange)
		{
			return EBTNodeResult::Failed;
		}
		else
		{
			// Target 방향으로 회전
			const FVector PawnLocation = ControlledPawn->GetActorLocation();
			const FVector TargetLocation = TargetCharacter->GetActorLocation();
			const FRotator LookAtRotation = (TargetLocation - PawnLocation).Rotation();
			const FRotator NewPawnRotation(0.f, LookAtRotation.Yaw, 0.f);
			ControlledPawn->SetActorRotation(NewPawnRotation);
			if (AAIController* AICon = OwnerComp.GetAIOwner())
			{
				AICon->SetControlRotation(NewPawnRotation);
			}

			// 공격 몽타주 재생 시작
			FOnAttackMontageEnded AttackMontageEndedDelegate;
			AttackMontageEndedDelegate.BindLambda([this, &OwnerComp]()
			{
				// 몽타주 종료 시점에 태스크 종료
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});

			CurrentAnimInstance = EnemyInterface->DefaultAttackMontagePlay(AttackMontageEndedDelegate);
			CurrentAttackAM = CurrentAnimInstance->GetCurrentActiveMontage();
			// 몽타주 종료까지 대기하기 위해 InProgress 반환
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_DefaultAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (CurrentAnimInstance == nullptr)
	{
		return;
	}

	if (!CurrentAnimInstance->Montage_IsPlaying(CurrentAttackAM.Get()))
	{
		// 몽타주가 재생되지 않으면 태스크 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}

}
