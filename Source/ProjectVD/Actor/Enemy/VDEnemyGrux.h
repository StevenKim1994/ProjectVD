// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Interface/VDAttackable.h"
#include "Interface/VDSequenceable.h"
#include "VDEnemyGrux.generated.h"

class UAnimMontage;
class UVDEnemyStatsBaseComponent;
class ULevelSequence;

UCLASS()
class PROJECTVD_API AVDEnemyGrux : public AVDEnemyCharacterBase, public IVDAttackable, public IVDSequenceable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Animation", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> HitReactAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULevelSequence> CutSceneSequencer;

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

	// IVDAttackable을(를) 통해 상속됨
	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void StartCutScene();

	// IVDSequenceable을(를) 통해 상속됨
	void OnSequenceStart() override;
	void OnSequenceStop() override;
	void OnSequencePause() override;
	void OnSequenceResume() override;
	ULevelSequence* GetSequence() const override { return CutSceneSequencer; }
};
