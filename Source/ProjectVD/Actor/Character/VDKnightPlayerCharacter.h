// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Interface/VDAttackable.h"
#include "VDKnightPlayerCharacter.generated.h"

UCLASS()
class PROJECTVD_API AVDKnightPlayerCharacter : public AVDCharacterBase, public IVDAttackable
{
	GENERATED_BODY()

private:


protected:
	virtual void BeginPlay() override;
	virtual void Look(const FInputActionValue& Value) override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void DefaultAttack(const FInputActionValue& Value) override;
public:
	AVDKnightPlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetComboInputOn(bool bIsOn) override;
	virtual void DefaultAttackHit() override;
};
