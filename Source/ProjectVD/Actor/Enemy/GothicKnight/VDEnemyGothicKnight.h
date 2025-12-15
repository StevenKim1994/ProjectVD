// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Interface/VDSequenceable.h"
#include "VDEnemyGothicKnight.generated.h"

class ULevelSequence;
class UStaticMeshComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDEnemyGothicKnight : public AVDEnemyCharacterBase, public IVDSequenceable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CutSceneSequencer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> StaggredAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> HeavyAttackAM;

	FTimerHandle BerserkScaleTimerHandle; // DESC :: 버서크 스케일 타이머 핸들

public:
	AVDEnemyGothicKnight();
	// IVDSequenceable을(를) 통해 상속됨
	void OnSequenceStart() override;
	void OnSequenceStop() override;
	void OnSequencePause() override;
	void OnSequenceResume() override;
	ULevelSequence* GetSequence() const override { return CutSceneSequencer; }

	virtual void UseSkill(uint8 SkillIndex, FOnSkillUsedEnded SkillUseEndedDelegate) override;
	// IVDAttackable을(를) 통해 상속됨
	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual void SkillAttackHit(int32 SkillIndex, int32 SkillAttackCount = 0) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void StartCutScene();
	
	
	virtual void SetBerserking(bool InMode, FOnBerserkingModeChanged EndCallback) override;

protected:

	virtual FName GetEnemyStatsRowKey() const override;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	void DefaultAttack() override;

};
