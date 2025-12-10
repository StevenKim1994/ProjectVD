// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Interface/VDAttackable.h"
#include "Interface/VDDefencable.h"
#include "VDKnightPlayerCharacter.generated.h"

enum class EVDLockOnStateType : uint8;
class AVDStagePlayerController;
class AActor;

UCLASS()
class PROJECTVD_API AVDKnightPlayerCharacter : public AVDCharacterBase, public IVDAttackable, public IVDDefencable
{
	GENERATED_BODY()

private:

protected:

	UPROPERTY()
	TWeakObjectPtr<AActor> LockedTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="FowardRolling", Meta = (AllowPrivateAccess ="true"))
	TObjectPtr<UAnimMontage> FowardRollingAM;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "PositonDrink", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PotionDrinkAM;

	UPROPERTY()
	int32 CurrentAttackComboCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackCombo", Meta = (AllowPrivateAccess = "true"))
	int32 MaxAttackComboCount = 3;

	UPROPERTY()
	bool bIsNextComboInputOn = false;

	void DefaultAttack(const FInputActionValue& Value) override;
	void DefaultAttackCombo();
	void DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo);
	void CheckComboInput();

	virtual void BeginPlay() override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Zoom(const FInputActionValue& Value) override;
	virtual void LockOnTarget(const FInputActionValue& Value) override;
	virtual void Rooting(const FInputActionValue& Value) override;
	virtual void Defence(const FInputActionValue& Value) override;
	virtual void Jump() override;

	virtual void RollLeft(const FInputActionValue& Value) override;
	virtual void RollRight(const FInputActionValue& Value) override;
	virtual void WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint) override;

	virtual void PostInitializeComponents() override;

public:
	AVDKnightPlayerCharacter();
	virtual void SetEquippedWeapon(AVDEquipItemVisualActor* NewWeapon) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
	virtual void ResetHitList() override;
};
