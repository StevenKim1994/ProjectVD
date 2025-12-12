// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/GothicKnight/VDEnemyGothicKnight.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "GameFramework/MovementComponent.h"
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

	CheckPatrolPosition();
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
			->SetMaxMovementSpeed(DataTableInfo->MaxMovementSpeed)
			->SetAttackRange(DataTableInfo->AttackRange)
			->SetAttackSpeed(DataTableInfo->AttackSpeed)
			->SetAttackPower(DataTableInfo->AttackPower)
			->SetMaxHealth(DataTableInfo->MaxHealth)
			->SetHealth(DataTableInfo->MaxHealth);
	}
}

void AVDEnemyGothicKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEnemyGothicKnight::CheckPatrolPosition()
{
	// TODO :: 현재 위치로 부터 3개의 패트롤 위치를 생성하는 로직 필요 현재위치 반경 500~550 유닛사이로 3개의 삼각형 패트롤 위치 생성
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()); // DESC :: 네비게이션 시스템 가져오기

	if (NavSystem == nullptr)
	{
		return;
	}

	PatrolPositions.Empty(); // DESC :: 기존 패트롤 위치 초기화

	const FVector OriginLocation = GetActorLocation(); // DESC :: 현재 위치
	const float MinRange = 500.0f;
	const float MaxRange = 550.0f;
	const int32 NumPoints = 3;
	const float AngleInterval = 360.0f / NumPoints; // DESC :: 120도 간격

	float CurrentAngle = FMath::RandRange(0.0f, 360.0f); // DESC :: 랜덤 시작 각도

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const float Distance = FMath::RandRange(MinRange, MaxRange); // DESC :: 500 ~ 550 유닛 사이 거리
		const float Radian = FMath::DegreesToRadians(CurrentAngle);

		// DESC :: 각도와 거리를 기반으로 오프셋 계산
		FVector Offset(FMath::Cos(Radian) * Distance, FMath::Sin(Radian) * Distance, 0.0f);
		FVector TargetLocation = OriginLocation + Offset;

		FNavLocation NavLocation;

		// DESC :: 네비게이션 시스템을 통해 유효한 위치인지 확인하고 투영
		if (NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation, FVector(200.0f, 200.0f, 200.0f)))
		{
			PatrolPositions.Add(NavLocation.Location); // DESC :: 유효한 위치 저장
		}

		CurrentAngle += AngleInterval; // DESC :: 다음 각도로 변경
	}
}

TArray<FVector> AVDEnemyGothicKnight::GetPatrolPosition() const
{
	return PatrolPositions;
}

