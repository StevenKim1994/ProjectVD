// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"
#include "Interface/VDEnemyInterface.h"
#include "Interface/VDAttackable.h"
#include "VDEnemyCharacterBase.generated.h"

class UAnimMontage;
class UVDEnemyAnimInstance;
class AVDEnemyAIController;

UCLASS(Abstract)
class PROJECTVD_API AVDEnemyCharacterBase : public ACharacter, public IVDEnemyInterface, public IVDAttackable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo", Meta = (AllowPrivateAccess = "true"))
	FName EnemyName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo", Meta = (AllowPrivateAccess = "true"))
	bool bIsBossEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FindPlayerAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DefaultAttackAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeathAM;

	UPROPERTY()
	TObjectPtr<AVDEnemyAIController> EnemyAIController;

	UPROPERTY()
	bool bIsDead = false;

	UPROPERTY()
	TObjectPtr<UVDEnemyStatsBaseComponent> BaseStatsComponent;

	virtual void FindPlayer();
	virtual void Move(const FVector& Direction, float Value);
	virtual void Die();
	virtual void DefaultAttack();
	virtual void HitReact(const FVector& HitPos);
	virtual void EndDieAM(UAnimMontage* AnimMontage, bool bInterept);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AVDEnemyCharacterBase();

	FORCEINLINE bool IsBossEnemy() const { return bIsBossEnemy; }

	UFUNCTION(BlueprintCallable, Category = "EnemyInfo")
	FORCEINLINE FName GetEnemyName() const { return EnemyName; }

	FORCEINLINE UVDEnemyStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent.Get(); }

	// IVDAttackable을(를) 통해 상속됨
	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// IVDEnemyInterface을(를) 통해 상속됨
	virtual UVDEnemyAnimInstance* PrepareAnimMontagePlay() override;
	virtual UVDEnemyAnimInstance* DefaultAttackMontagePlay(FOnAttackMontageEnded AttackMontageEndedDelegate = nullptr) override;
	virtual UAnimMontage* GetFindPlayerAnimMontage() const override;
	virtual UVDEnemyStatsBaseComponent* GetStatsComp() const override;

	float GetPatrolRadius() const override;
	float GetPatrolWaitTime() const override;
	float GetFindingRange() const override;
	float GetTurnSpeed() const override;
	float GetAttackRadius() const override;
	float GetAttackRange() const override;
};
