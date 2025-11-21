// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actor/ActorComponent/VDEnemyStatsBaseComponent.h"
#include "Interface/VDEnemyInterface.h"
#include "VDEnemyCharacterBase.generated.h"

class UAnimMontage;
class UVDEnemyAnimInstance;
UCLASS(Abstract)
class PROJECTVD_API AVDEnemyCharacterBase : public ACharacter, public IVDEnemyInterface
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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE bool IsBossEnemy() const { return bIsBossEnemy; }
	FORCEINLINE FName GetEnemyName() const { return EnemyName; }
	FORCEINLINE UVDEnemyStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent.Get(); }

	// IVDEnemyInterface을(를) 통해 상속됨
	virtual UVDEnemyAnimInstance* PrepareAnimMontagePlay() override;
	virtual UAnimMontage* GetFindPlayerAnimMontage() const override;
	float GetPatrolRadius() const override;
	float GetPatrolWaitTime() const override;
	float GetFindingRange() const override;
	float GetTurnSpeed() const override;
	float GetAttackRadius() const override;
	float GetAttackRange() const override;
};
