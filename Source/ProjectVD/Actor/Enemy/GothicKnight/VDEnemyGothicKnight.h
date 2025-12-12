// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Interface/VDSequenceable.h"
#include "Interface/VDPatrolPositionProvider.h"
#include "VDEnemyGothicKnight.generated.h"

class ULevelSequence;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class PROJECTVD_API AVDEnemyGothicKnight : public AVDEnemyCharacterBase, public IVDSequenceable, public IVDPatrolPositionProvider
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CutSceneSequencer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , Category = "Patrol", Meta = (AllowPrivateAccess = true))
	TArray<FVector> PatrolPositions;

public:
	AVDEnemyGothicKnight();
	// IVDSequenceable을(를) 통해 상속됨
	void OnSequenceStart() override;
	void OnSequenceStop() override;
	void OnSequencePause() override;
	void OnSequenceResume() override;
	ULevelSequence* GetSequence() const override { return CutSceneSequencer; }

	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void StartCutScene();
	
	// IVDPatrolPositionProvider을(를) 통해 상속됨
	TArray<FVector> GetPatrolPosition() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	void CheckPatrolPosition();

};
