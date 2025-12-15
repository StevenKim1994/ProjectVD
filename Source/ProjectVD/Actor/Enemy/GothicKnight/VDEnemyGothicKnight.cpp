// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/GothicKnight/VDEnemyGothicKnight.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "Animation/VDEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"

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
	Super::DefaultAttackHit();

	if (BaseStatsComponent == nullptr)
	{
		return;
	}

	FVector Origin = GetActorLocation(); // DESC :: 공격 원점 위치
	FVector Forward = GetActorForwardVector(); // DESC :: 액터 전방 벡터
	float AttackRadius = BaseStatsComponent->GetAttackRange(); // DESC :: 공격 범위
	float AttackAngle = 90.0f; // DESC :: 부채꼴 각도 (90도)

	DrawDebugCone(GetWorld(), Origin, Forward, AttackRadius, FMath::DegreesToRadians(AttackAngle * 0.5f), FMath::DegreesToRadians(AttackAngle * 0.5f), 12, FColor::Green, false, 1.0f); // DESC :: 공격 각도 디버그 드로잉

	TArray<FOverlapResult> OverlapResults; // DESC :: 오버랩 결과 배열
	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(DefaultAttackHit), false, this); // DESC :: 충돌 쿼리 파라미터

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		CollisionParams
	); // DESC :: 구체 오버랩 검사

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor(); // DESC :: 오버랩된 액터

			if (OverlappedActor && OverlappedActor != this)
			{
				FVector DirectionToTarget = (OverlappedActor->GetActorLocation() - Origin).GetSafeNormal(); // DESC :: 타겟 방향 벡터
				float DotProduct = FVector::DotProduct(Forward, DirectionToTarget); // DESC :: 내적 계산
				float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(AttackAngle * 0.5f)); // DESC :: 각도 코사인 값

				if (DotProduct >= CosHalfAngle)
				{
					UGameplayStatics::ApplyDamage(
						OverlappedActor,
						BaseStatsComponent->GetAttackPower(),
						GetController(),
						this,
						UDamageType::StaticClass()
					); // DESC :: 데미지 적용
				}
			}
		}
	}
}

void AVDEnemyGothicKnight::SkillAttackHit(int32 SkillIndex, int32 SkillAttackCount)
{
	Super::SkillAttackHit(SkillIndex, SkillAttackCount);

	switch(SkillIndex)
	{
		case 0:
		{
			if (HeavyAttackEffect)
			{
				UWorld* World = GetWorld();
				if (World == nullptr)
				{
					break;
				}

				const FVector SpawnOrigin = GetActorLocation(); // DESC :: 이펙트 스폰 기준 위치
				const FVector Forward = GetActorForwardVector(); // DESC :: 액터 전방 벡터
				const float SpawnDistance = 150.0f; // DESC :: 전방 오프셋 거리
				const FVector TraceStart = SpawnOrigin + Forward * SpawnDistance + FVector(0.0f, 0.0f, 100.0f); // DESC :: 라인트레이스 시작 지점
				const FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 500.0f); // DESC :: 라인트레이스 종료 지점
				FHitResult HitResult;
				FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HeavyAttackEffectTrace), false, this); // DESC :: 충돌 쿼리 파라미터

				FVector EffectScale = FVector(5.0f, 5.0f, 5.0f); // DESC :: 이펙트 스케일
				FVector EffectLocation = TraceEnd; // DESC :: 기본 이펙트 위치
				if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
				{
					EffectLocation = HitResult.Location; // DESC :: 지면에 맞춘 이펙트 위치
					EffectLocation.Z += 50.0f; // DESC :: 이펙트 약간 띄우기
				}

				const FRotator EffectRotation = Forward.Rotation(); // DESC :: 이펙트 회전값
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HeavyAttackEffect,  EffectLocation, EffectRotation, EffectScale); // DESC :: 나이아가라 이펙트 스폰
			}
		}
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Gothic Knight Skill %d Hit - %d"), SkillIndex, SkillAttackCount);
}

void AVDEnemyGothicKnight::UseSkill(uint8 SkillIndex, FOnSkillUsedEnded SkillUseEndedDelegate)
{
	// TODO :: GAS로 바꾸면 여기 수정 필요
	Super::UseSkill(SkillIndex, SkillUseEndedDelegate);

	switch (SkillIndex)
	{
		case 0:
		{
			UE_LOG(LogTemp, Warning, TEXT("Gothic Knight Skill 0 Used"));
			UVDEnemyAnimInstance* AnimInstance = Cast<UVDEnemyAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance == nullptr)
			{
				return;
			}

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindWeakLambda(this, [SkillUseEndedDelegate](UAnimMontage* Montage, bool bInterrupted)
			{
				SkillUseEndedDelegate.ExecuteIfBound();
			});

			AnimInstance->Montage_Play(HeavyAttackAM);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, HeavyAttackAM);
		}
		break;
	}
}

float AVDEnemyGothicKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return Result;
}

FName AVDEnemyGothicKnight::GetEnemyStatsRowKey() const
{
	return FName(TEXT("GothicKnight"));
}

void AVDEnemyGothicKnight::BeginPlay()
{
	Super::BeginPlay();
}

void AVDEnemyGothicKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AVDEnemyGothicKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEnemyGothicKnight::DefaultAttack()
{
	Super::DefaultAttack();
}

