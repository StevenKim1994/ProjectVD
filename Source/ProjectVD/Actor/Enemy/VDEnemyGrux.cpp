// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/VDEnemyGrux.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "System/VDCutSceneSubSystem.h"
#include "Animation/VDEnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "DataTable/VDEnemyStatsInfo.h"
#include "System/VDDataTableSubsystem.h"

AVDEnemyGrux::AVDEnemyGrux()
{
	PrimaryActorTick.bCanEverTick = true;
	BaseStatsComponent = CreateDefaultSubobject<UVDEnemyStatsBaseComponent>(TEXT("BaseStatsComponent"));

	WeakPointCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeakPointCollision"));
	WeakPointCollision->SetupAttachment(GetMesh(), TEXT("WeakPoint"));
	WeakPointCollision->SetRelativeLocation(FVector(-20.000000, 0.000000,-50.000000));
	WeakPointCollision->SetRelativeRotation(FRotator::ZeroRotator);
	WeakPointCollision->SetCapsuleHalfHeight(60.f);
	WeakPointCollision->SetCapsuleRadius(30.f);
}

void AVDEnemyGrux::FindPlayer()
{
	Super::FindPlayer();
}

void AVDEnemyGrux::Move(const FVector& Direction, float Value)
{
	Super::Move(Direction, Value);
}

void AVDEnemyGrux::Die()
{
	Super::Die();
}

void AVDEnemyGrux::DefaultAttack()
{
	Super::DefaultAttack();
}

void AVDEnemyGrux::HitReact(const FVector& HitPos)
{
	//Super::HitReact(HitPos);
	FVector CurrentLocation = GetActorLocation();
	FVector HitDirection = (CurrentLocation - HitPos).GetSafeNormal();

	// 액터의 전방 벡터 가져오기
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	// 내적을 통해 방향 판별
	float ForwardDot = FVector::DotProduct(ForwardVector, HitDirection);
	float RightDot = FVector::DotProduct(RightVector, HitDirection);

	UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	// 방향 체크 (전방, 후방, 좌측, 우측)
	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		if (ForwardDot > 0)
		{
			// 전방에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Front"));
			AnimInstance->Montage_Play(HitReactAM[3]);
		}
		else
		{
			// 후방에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Back"));
			AnimInstance->Montage_Play(HitReactAM[2]);
		}
	}
	else
	{
		if (RightDot > 0)
		{
			// 우측에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Right"));
			AnimInstance->Montage_Play(HitReactAM[0]);
		}
		else
		{
			// 좌측에서 맞음
			UE_LOG(LogTemp, Warning, TEXT("Hit from Left"));
			AnimInstance->Montage_Play(HitReactAM[1]);
		}
	}
}

void AVDEnemyGrux::BeginPlay()
{
	Super::BeginPlay();
	FVDEnemyStatsInfo* DataTableInfo = GetGameInstance()->GetSubsystem<UVDDataTableSubSystem>()->GetDataTableRow<FVDEnemyStatsInfo>(FName(TEXT("EnemyStatsInfo")), FName(TEXT("Grux")));

	if (DataTableInfo)
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
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	Movement->MinAnalogWalkSpeed = 50.f;
	Movement->MaxWalkSpeed = BaseStatsComponent->GetMaxMovementSpeed();

	StartCutScene();
}

void AVDEnemyGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
	if (WeakPointCollision)
	{
		const FVector Center = WeakPointCollision->GetComponentLocation();
		const FQuat Rotation = WeakPointCollision->GetComponentQuat();
		const float HalfHeight = WeakPointCollision->GetScaledCapsuleHalfHeight();
		const float Radius = WeakPointCollision->GetScaledCapsuleRadius();

		// 프레임마다 WeakPoint 캡슐을 시각화
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius, Rotation, FColor::Red, false, 0.0f, 0, 1.5f);
	}
#endif
}

void AVDEnemyGrux::SetComboInputOn(bool bIsOn)
{
}

void AVDEnemyGrux::DefaultAttackHit()
{
	const float AttackRange = BaseStatsComponent ? BaseStatsComponent->GetAttackRange() : 0.f;
	if (AttackRange <= 0.f)
	{
		return;
	}

	const float AttackAngleDegrees = 60.f;
	const int32 SegmentCount = 24;
	const FColor SectorColor = FColor::Orange;
	const float LineThickness = 1.5f;

	const FVector Origin = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	const FVector Forward = GetActorForwardVector();
	const FVector Up = FVector::UpVector;
	const float HalfAngleRad = FMath::DegreesToRadians(AttackAngleDegrees * 0.5f);

	const FVector LeftDir = Forward.RotateAngleAxis(-AttackAngleDegrees * 0.5f, Up);
	const FVector RightDir = Forward.RotateAngleAxis(AttackAngleDegrees * 0.5f, Up);
	const FVector LeftEnd = Origin + LeftDir * AttackRange;
	const FVector RightEnd = Origin + RightDir * AttackRange;

	DrawDebugLine(GetWorld(), Origin, LeftEnd, SectorColor, false, 2.0f, 0, LineThickness);
	DrawDebugLine(GetWorld(), Origin, RightEnd, SectorColor, false, 2.0f, 0, LineThickness);

	FVector PrevPoint = LeftEnd;
	for (int32 i = 1; i <= SegmentCount; ++i)
	{
		const float T = static_cast<float>(i) / static_cast<float>(SegmentCount);
		const float Angle = FMath::Lerp(-HalfAngleRad, HalfAngleRad, T);
		const FVector Dir = Forward.RotateAngleAxis(FMath::RadiansToDegrees(Angle), Up);
		const FVector Point = Origin + Dir * AttackRange;

		DrawDebugLine(GetWorld(), PrevPoint, Point, SectorColor, false, 2.0f, 0, LineThickness);
		PrevPoint = Point;
	}

	TArray<FOverlapResult> OverlapResults;
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRange);

	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		CollisionShape
	);

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* OverlappedActor = Result.GetActor();
		if (OverlappedActor == this) // DESC :: 자기자신은 무시함.
		{
			continue;
		}

		if (OverlappedActor)
		{
			OverlappedActor->TakeDamage(BaseStatsComponent->GetAttackPower(), FDamageEvent(), GetController(), this);
		}
	}
}

float AVDEnemyGrux::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(bIsDead)
	{
		return Result;
	}

	HitReact(DamageCauser->GetActorLocation());

	return Result;
}

void AVDEnemyGrux::StartCutScene()
{
	if (CutSceneSequencer)
	{
		UVDCutSceneSubSystem* CutSceneSubsystem = GetWorld()->GetSubsystem<UVDCutSceneSubSystem>();
		if (CutSceneSubsystem)
		{
			CutSceneSubsystem->StartCutScene(this);
		}
	}
}

void AVDEnemyGrux::OnSequenceStart()
{
	if(EnemyAIController)
	{ 
		EnemyAIController->StopAI();
	}
}

void AVDEnemyGrux::OnSequenceStop()
{
	if (EnemyAIController)
	{
		EnemyAIController->RunAI();
	}
}

void AVDEnemyGrux::OnSequencePause()
{

}

void AVDEnemyGrux::OnSequenceResume()
{

}
