// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VDCharacterBase.generated.h"

class UAnimMontage;
class AVDWeapon;

UCLASS(Abstract)
class PROJECTVD_API AVDCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSpeed", Meta = (AllowPrivateAccess ="true"))
	float AttackSpeedRate = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DefaultAttackAM;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	UFUNCTION()
	virtual void DefaultAttack(const FInputActionValue& Value);

public:	
	AVDCharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void EquipWeapon(AVDWeapon* NewWeapon);
};
