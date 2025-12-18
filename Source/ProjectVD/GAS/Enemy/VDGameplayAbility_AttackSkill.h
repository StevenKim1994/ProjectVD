// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VDGameplayAbility_AttackSkill.generated.h"

class UAnimMontage;
class AVDCharacterBase;

DECLARE_DELEGATE(FOnAttackSkillAbilityEnded);

UCLASS()
class PROJECTVD_API UVDGameplayAbility_AttackSkill : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AttackSkill")
	TObjectPtr<UAnimMontage> AttackSkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AttackSkill")
	float AttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = "AttackSkill")
	float AttackAngle;

	FOnAttackSkillAbilityEnded OnAttackSkillAbilityEnded;

public:
	UVDGameplayAbility_AttackSkill();

	void SetOnAttackSkillAbilityEndedDelegate(FOnAttackSkillAbilityEnded InDelegate);

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void PerformSkillHitDetection();

	AVDCharacterBase* GetCharacterBaseOwner() const;
};
