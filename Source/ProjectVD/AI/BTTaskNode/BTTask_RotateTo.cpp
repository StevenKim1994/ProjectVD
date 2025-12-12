// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_RotateTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

UBTTask_RotateTo::UBTTask_RotateTo()
{
	NodeName = TEXT("Rotate To");
	bNotifyTick = true; // DESC :: 회전 업데이트를 위해 틱 활성화
}

EBTNodeResult::Type UBTTask_RotateTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (TargetToFaceKey.SelectedKeyName.IsNone())
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetToFaceKey.SelectedKeyName);
	if (TargetObject == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UObject* Self = BlackboardComp->GetValueAsObject("SelfActor");
	if (Self == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* SelfActor = Cast<AActor>(Self);
	if (SelfActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// DESC :: 즉시 완료되지 않고 TickTask에서 회전을 처리하도록 InProgress 반환
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	UObject* Self = BlackboardComp->GetValueAsObject("SelfActor");
	AActor* SelfActor = Cast<AActor>(Self);

	if (TargetActor == nullptr || SelfActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector LookVector = TargetActor->GetActorLocation() - SelfActor->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	FRotator NewRot = FMath::RInterpTo(SelfActor->GetActorRotation(), TargetRot, DeltaSeconds, RotationSpeed);
	SelfActor->SetActorRotation(NewRot);

	// DESC :: 목표 회전값과 현재 회전값이 거의 일치하는지 확인 (오차 범위 2도)
	if (SelfActor->GetActorRotation().Equals(TargetRot, 2.0f))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_RotateTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_RotateTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
