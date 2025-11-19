// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "VDEnemyCharacterBase.generated.h"

class UAnimMontage;
UCLASS(Abstract)
class PROJECTVD_API AVDEnemyCharacterBase : public ACharacter
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
	TObjectPtr<UVDCharacterStatsBaseComponent> BaseStatsComponent;

	UFUNCTION()
	virtual void FindPlayer();

	UFUNCTION()
	virtual void Move(const FVector& Direction, float Value);

	UFUNCTION()
	virtual void Die();

	UFUNCTION()
	virtual void DefaultAttack();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AVDEnemyCharacterBase();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	FORCEINLINE bool IsBossEnemy() const { return bIsBossEnemy; }

	UFUNCTION()
	FORCEINLINE FName GetEnemyName() const { return EnemyName; }
	
	FORCEINLINE UVDCharacterStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent.Get(); }
};
