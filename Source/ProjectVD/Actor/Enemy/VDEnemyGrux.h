// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Interface/VDAttackable.h"
#include "VDEnemyGrux.generated.h"

class UAnimMontage;

UCLASS()
class PROJECTVD_API AVDEnemyGrux : public AVDEnemyCharacterBase, public IVDAttackable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Animation", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> HitReactAM;

protected:
	virtual void FindPlayer() override;
	virtual void Move(const FVector& Direction, float Value) override;
	virtual void Die() override;
	virtual void DefaultAttack() override;
	virtual void HitReact(const FVector& HitPos) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	AVDEnemyGrux();

	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
