// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/GothicKnight/VDEnemyGothicKnight.h"
#include "Actor/Enemy/AIController/VDEnemyAIController.h"
#include "Animation/VDEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

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

