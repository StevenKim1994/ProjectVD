// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceNode/BTService_DetectTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/VDEnemyInterface.h"
#include "Engine/OverlapResult.h"
#include "Public/VDBlackboardInfo.h"
#include "Public/VDPhysicInfo.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Dectect Target");
	Interval = 1.f;
}

APawn* UBTService_DetectTarget::DetectedTargetActor(UBehaviorTreeComponent& OwnerComp, const TArray<FOverlapResult>& OverlapResult)
{
	APawn* Result = nullptr;

	for (auto const& OverlapResult : OverlapResult)
	{
		Result = Cast<APawn>(OverlapResult.GetActor());
		if (Result->GetController()->IsPlayerController())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(VDBB_KEY_TARGET, Result);
			DrawDebugSphere(Result->GetWorld(), Result->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
			DrawDebugLine(Result->GetWorld(), OwnerComp.GetOwner()->GetActorLocation(), Result->GetActorLocation(), FColor::Blue, false, 0.2f);
			break;
		}
	}

	return Result;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IVDEnemyInterface* EnemyPawn = Cast<IVDEnemyInterface>(ControllingPawn);
	if (nullptr == EnemyPawn)
	{
		return;
	}

	float DetectRadius = EnemyPawn->GetFindingRange() / 2.f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(DetectRange), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PROFILE_CHACRACTER_ACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		if (DetectedTargetActor(OwnerComp, OverlapResults))
			return;
	}
}


