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
#include "NiagaraAnimNotifies/Public/AnimNotify_PlayNiagaraEffect.h"

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
		if(TargetCharacter->IsDead())
		{
			BlackboardComp->SetValueAsObject(VDBB_KEY_TARGET, nullptr);
			return EBTNodeResult::Failed;
		}
		if (ControlledPawn->GetDistanceTo(TargetCharacter) > AttackRange)
		{
			return EBTNodeResult::Failed;
		}
		else
		{
			const FVector PawnLocation = ControlledPawn->GetActorLocation();
			const FVector TargetLocation = TargetCharacter->GetActorLocation();
			const FRotator LookAtRotation = (TargetLocation - PawnLocation).Rotation();
			const FRotator NewPawnRotation(0.f, LookAtRotation.Yaw, 0.f);
			ControlledPawn->SetActorRotation(NewPawnRotation);
			if (AAIController* AICon = OwnerComp.GetAIOwner())
			{
				AICon->SetControlRotation(NewPawnRotation);
			}

			FOnAttackMontageEnded AttackMontageEndedDelegate;
			AttackMontageEndedDelegate.BindLambda([this, &OwnerComp]()
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});

			APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
			if(Pawn == nullptr)
			{
				return EBTNodeResult::Failed;
			}

			CurrentAnimInstance = EnemyInterface->DefaultAttackMontagePlay(AttackMontageEndedDelegate);
			CurrentAttackAM = CurrentAnimInstance->GetCurrentActiveMontage();
			for (const FAnimNotifyEvent& NotifyEvent : CurrentAttackAM->Notifies)
			{
				if (UAnimNotify_PlayNiagaraEffect* Notify = Cast<UAnimNotify_PlayNiagaraEffect>(NotifyEvent.Notify))
				{
					Notify->Scale = Pawn->GetActorScale3D() * 3.0f;
					// DESC :: 현재 캐릭터 스케일에 맞게 이펙트 스케일 조정
				}
			}
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_DefaultAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
