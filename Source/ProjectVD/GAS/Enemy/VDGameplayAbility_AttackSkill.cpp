// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Enemy/VDGameplayAbility_AttackSkill.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Engine/OverlapResult.h"

UVDGameplayAbility_AttackSkill::UVDGameplayAbility_AttackSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVDGameplayAbility_AttackSkill::SetOnAttackSkillAbilityEndedDelegate(FOnAttackSkillAbilityEnded InDelegate)
{
	OnAttackSkillAbilityEnded = InDelegate;
}

void UVDGameplayAbility_AttackSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!AttackSkillMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("UVDGameplayAbility_AttackSkill::ActivateAbility AttackSkillMontage is nullptr"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// DESC :: 몽타주 재생 태스크 생성
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackSkillMontage,
		1.0f,
		NAME_None,
		false
	);

	MontageTask->OnCompleted.AddDynamic(this, &UVDGameplayAbility_AttackSkill::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UVDGameplayAbility_AttackSkill::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UVDGameplayAbility_AttackSkill::OnMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UVDGameplayAbility_AttackSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	OnAttackSkillAbilityEnded.ExecuteIfBound();
}

void UVDGameplayAbility_AttackSkill::OnMontageCompleted()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVDGameplayAbility_AttackSkill::OnMontageCancelled()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVDGameplayAbility_AttackSkill::PerformSkillHitDetection()
{
	AVDCharacterBase* Owner = GetCharacterBaseOwner();

	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();

	UVDCharacterStatsBaseComponent* StatsComponent = Owner->GetBaseStatsComponent();
	if (!StatsComponent)
	{
		return;
	}

	FVector Origin = Owner->GetActorLocation();
	FVector Forward = Owner->GetActorForwardVector();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(PerformSkillHitDetection), false, Owner);

	bool bHasOverlapped = World->OverlapMultiByChannel(
		OverlapResults,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		CollisionQueryParams
	);

	if (bHasOverlapped)
	{
		for(const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (OverlappedActor && OverlappedActor != Owner)
			{
				FVector DirectionToTarget = OverlappedActor->GetActorLocation() - Origin;
				DirectionToTarget.Z = 0.0f;
				DirectionToTarget.Normalize();
				float DotProduct = FVector::DotProduct(Forward, DirectionToTarget);
				float AngleDegrees = FMath::Acos(DotProduct) * (180.0f / PI);
				if (AngleDegrees <= (AttackAngle * 0.5f))
				{
					// DESC :: 데미지 적용
					float AttackPower = StatsComponent->GetAttackPower();
					UGameplayStatics::ApplyDamage(OverlappedActor, AttackPower, Owner->GetController(), Owner, nullptr);
				}
			}
		}
	}
}

AVDCharacterBase* UVDGameplayAbility_AttackSkill::GetCharacterBaseOwner() const
{
	return Cast<AVDCharacterBase>(GetAvatarActorFromActorInfo());
}
