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

void AVDEnemyGothicKnight::SetBerserking(bool InMode, FOnBerserkingModeChanged EndCallback)
{
	Super::SetBerserking(InMode, EndCallback);

	if (InMode)
	{
		USkeletalMeshComponent* MeshComponent = GetMesh(); // DESC :: 메시 컴포넌트 가져오기
		if (MeshComponent == nullptr)
		{
			return;
		}

		const FVector InitialScale = MeshComponent->GetRelativeScale3D(); 
		const FVector TargetScale = InitialScale * 2.0f; 
		const float TweeningDuration = 3.0f; // DESC :: 트위닝 지속 시간 (3초)
		const float UpdateInterval = 0.01f; 
		float ElapsedTime = 0.0f; // DESC :: 경과 시간

		FTimerManager& TimerManager = GetWorld()->GetTimerManager(); // DESC :: 타이머 매니저 가져오기
		
		TimerManager.SetTimer(
			BerserkScaleTimerHandle,
			[this, EndCallback, MeshComponent, InitialScale, TargetScale, TweeningDuration, &ElapsedTime]() mutable
			{
				ElapsedTime += 0.01f; 
				float Alpha = FMath::Clamp(ElapsedTime / TweeningDuration, 0.0f, 1.0f); 
				FVector NewScale = FMath::Lerp(InitialScale, TargetScale, Alpha); 
				
				UE_LOG(LogTemp, Warning, TEXT("Berserk Scaling Alpha: %f"), Alpha); 
				if (MeshComponent)
				{
					MeshComponent->SetRelativeScale3D(NewScale); // DESC :: 스케일 적용
				}

				if (Alpha >= 1.0f)
				{
					EndCallback.ExecuteIfBound(); // DESC :: 콜백 실행
					GetWorld()->GetTimerManager().ClearTimer(BerserkScaleTimerHandle); // DESC :: 타이머 정리
				}
			},
			UpdateInterval,
			true
		); // DESC :: 타이머 설정
	}
	else
	{
		USkeletalMeshComponent* MeshComponent = GetMesh(); // DESC :: 메시 컴포넌트 가져오기
		if (MeshComponent)
		{
			GetWorld()->GetTimerManager().ClearTimer(BerserkScaleTimerHandle); // DESC :: 기존 타이머 정리
			MeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); // DESC :: 원래 스케일로 복원
		}
	}
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

	//DrawDebugCone(GetWorld(), Origin, Forward, AttackRadius, FMath::DegreesToRadians(AttackAngle * 0.5f), FMath::DegreesToRadians(AttackAngle * 0.5f), 12, FColor::Green, false, 1.0f); // DESC :: 공격 각도 디버그 드로잉

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
			FVector Origin = GetActorLocation(); // DESC :: 공격 원점 위치
			FVector Forward = GetActorForwardVector(); // DESC :: 액터 전방 벡터
			float AttackRadius = BaseStatsComponent->GetAttackRange(); // DESC :: 공격 범위
			float AttackAngle = 90.0f; // DESC :: 부채꼴 각도 (90도)
			TArray<FOverlapResult> OverlapResults; // DESC :: 오버랩 결과 배열
			FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(DefaultAttackHit), false, this); // DESC :: 충돌 쿼리 파라미터

			DrawDebugCone(GetWorld(), Origin, Forward, AttackRadius, FMath::DegreesToRadians(AttackAngle * 0.5f), FMath::DegreesToRadians(AttackAngle * 0.5f), 12, FColor::Green, false, 1.0f); // DESC :: 공격 각도 디버그 드로잉
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

