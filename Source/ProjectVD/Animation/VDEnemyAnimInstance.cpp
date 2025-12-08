// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UVDEnemyAnimInstance::UVDEnemyAnimInstance()
{
	MovingThreshold = 3.f;
}

void UVDEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwnerCharacter)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}

}
void UVDEnemyAnimInstance::SetIsDead(uint8 InIsDead)
{
	bIsDead = InIsDead;
}

void UVDEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacterMovement)
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = (GroundSpeed < MovingThreshold) && (bIsDead == false);
	}
}
