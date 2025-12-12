// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/GothicKnight/VDEnemyGothicKnight.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

#include "DataTable/VDEnemyStatsInfo.h"
#include "System/VDDataTableSubSystem.h"

AVDEnemyGothicKnight::AVDEnemyGothicKnight()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AVDEnemyGothicKnight::OnSequenceStart()
{
	if (EnemyAIController)
	{
		EnemyAIController->StopAI();
	}
}

void AVDEnemyGothicKnight::OnSequenceStop()
{
	if(EnemyAIController)
	{
		EnemyAIController->RunAI();
	}
}

void AVDEnemyGothicKnight::OnSequencePause()
{
}

void AVDEnemyGothicKnight::OnSequenceResume()
{
}

void AVDEnemyGothicKnight::StartCutScene()
{

}

void AVDEnemyGothicKnight::SetComboInputOn(bool bIsOn)
{
	Super::SetComboInputOn(bIsOn);
}

void AVDEnemyGothicKnight::DefaultAttackHit()
{
	Super::DefaultAttack();
}

float AVDEnemyGothicKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return Result;
}

void AVDEnemyGothicKnight::BeginPlay()
{
	Super::BeginPlay();
}

void AVDEnemyGothicKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance == nullptr)
	{
		return;
	}

	FVDEnemyStatsInfo* DataTableInfo = GameInstance->GetSubsystem<UVDDataTableSubSystem>()->GetDataTableRow<FVDEnemyStatsInfo>(FName(TEXT("EnemyStatsInfo")), FName(TEXT("GothicKnight")));

	if(DataTableInfo)
	{
		BaseStatsComponent
			->SetFindPlayerRange(DataTableInfo->FindPlayerRange)
			->SetPatrolRange(DataTableInfo->PatrolRange)
			->SetPatrolWaitTime(DataTableInfo->PatrolWaitTime)
			->SetTurnSpeed(DataTableInfo->TurnSpeed)
			->SetMaxPatrolMoveSpeed(DataTableInfo->MaxPatrolMoveSpeed)
			->SetAttackRange(DataTableInfo->AttackRange)
			->SetAttackSpeed(DataTableInfo->AttackSpeed)
			->SetAttackPower(DataTableInfo->AttackPower)
			->SetMaxHealth(DataTableInfo->MaxHealth)
			->SetHealth(DataTableInfo->MaxHealth);

		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		Movement->MinAnalogWalkSpeed = 10.f;
		Movement->MaxWalkSpeed = BaseStatsComponent->GetMaxPatrolMoveSpeed();
	}

}

void AVDEnemyGothicKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEnemyGothicKnight::DefaultAttack()
{
	Super::DefaultAttack();
}

