// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTaskNode_Berserking.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/VDEnemyInterface.h"
#include "AIController.h"
#include "System/VDUISubsystem.h"

UBTTaskNode_Berserking::UBTTaskNode_Berserking()
{
}

EBTNodeResult::Type UBTTaskNode_Berserking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if(EnemyInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UGameInstance* GI = World->GetGameInstance();
	GI->GetSubsystem<UVDUISubsystem>()->ShowToastMessage(FText::FromString(TEXT("적이 광폭화 중입니다.")));

	FOnBerserkingModeChanged BerserkingEndDelegate;
	BerserkingEndDelegate.BindWeakLambda(OwnerComp.GetAIOwner(), [&, this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Berserking Ended"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});

	EnemyInterface->SetBerserking(true, BerserkingEndDelegate);
	UE_LOG(LogTemp, Warning, TEXT("Berserking Started"));
	return EBTNodeResult::InProgress;
}
