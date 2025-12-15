// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_UseSkill.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "Interface/VDEnemyInterface.h"
#include "System/VDUISubsystem.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = "Use Skill";
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AActor* AIOwner = AIController->GetPawn();
	if (!AIOwner)
	{
		return EBTNodeResult::Failed;
	}

	if (IVDEnemyInterface* EnemyInterface = Cast<IVDEnemyInterface>(AIOwner))
	{
		FOnSkillUsedEnded SkillUseEndedDelegate;

		UGameInstance* GI = OwnerComp.GetAIOwner()->GetWorld()->GetGameInstance();
		if (UVDUISubsystem* VDUISubsystem = GI->GetSubsystem<UVDUISubsystem>())
		{
			VDUISubsystem->ShowToastMessage(FText::FromString(TEXT("적이 스킬을 사용합니다!")),5.f);
		}

		SkillUseEndedDelegate.BindWeakLambda(this, [this, OwnerCompPtr = &OwnerComp]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill Use Ended Delegate Called"));
			FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
		});
		EnemyInterface->UseSkill(SkillIndex, SkillUseEndedDelegate);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::InProgress;
}
