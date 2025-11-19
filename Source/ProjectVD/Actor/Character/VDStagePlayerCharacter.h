// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Interface/VDAttackable.h"
#include "VDStagePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class AVDStagePlayerController;
class UVDCharacterStatsBaseComponent;

UCLASS()
class PROJECTVD_API AVDStagePlayerCharacter : public AVDCharacterBase, public IVDAttackable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirAttack", Meta =(AllowPrivateAccess ="true"))
	TObjectPtr<UAnimMontage> AirAttackAM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackCombo", Meta = (AllowPrivateAccess = "true"))
	int32 MaxAttackComboCount = 3;

	UPROPERTY()
	int32 CurrentAttackComboCount = 0;

	UPROPERTY()
	bool bIsNextComboInputOn = false;

	UFUNCTION()
	void DefaultAttackCombo();

	UFUNCTION()
	void DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo);

	UFUNCTION()
	void CheckComboInput();
protected:
	// 플레이어 전용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVDCharacterStatsBaseComponent> BaseStatsComponent;

	UPROPERTY()
	TObjectPtr<AVDStagePlayerController> CastPlayerController;

	UFUNCTION()
	void Escape(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	void JumpBegin(const FInputActionValue& Value);

	UFUNCTION()
	void JumpEnd(const FInputActionValue& Value);

	UFUNCTION()
	void DefendHold(const FInputActionValue& Value);

	UFUNCTION()
	void DefendCancel(const FInputActionValue& InputActionValue);

	bool IsAirAttack();
	bool bHasAirAttacked;

	virtual void DefaultAttack(const FInputActionValue& Value) override;

	virtual void Jump() override;

	virtual void Move(const FInputActionValue& Value) override;


public:
	AVDStagePlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UVDCharacterStatsBaseComponent* GetBaseStatsComponent() const { return BaseStatsComponent; }

	void SetComboInputOn_Implementation(bool bIsOn);
	void DefaultAttackHit_Implementation() override;
};
