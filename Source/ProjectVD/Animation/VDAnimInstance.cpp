// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

UVDAnimInstance::UVDAnimInstance()
{
	MovingThreshold = 3.f;
}

void UVDAnimInstance::SetIsLockOnTarget(uint8 bIsLockOn)
{
	bIsLockOnTarget = bIsLockOn;
}

void UVDAnimInstance::SetIsDead(uint8 InIsDead)
{
	bIsDead = InIsDead;
}

void UVDAnimInstance::SetIsDefence(uint8 InIsDefence)
{
	bIsDefence = InIsDefence;
}

void UVDAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwnerCharacter)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void UVDAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacterMovement)
	{
		if (bIsLockOnTarget)
		{
			const FVector Vel = OwnerCharacterMovement->Velocity;
			const float FS = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), Vel);
			const float RS = FVector::DotProduct(OwnerCharacter->GetActorRightVector(), Vel);

			ForwardSpeed = FS;
			RightSpeed = RS;

			bIsIdle = (ForwardSpeed == 0 && RightSpeed == 0) && (bIsDead == false) && (bIsDefence == false);
		}
		else
		{
			const FVector Vel = OwnerCharacterMovement->Velocity;
			const float FS = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), Vel);
			const float RS = FVector::DotProduct(OwnerCharacter->GetActorRightVector(), Vel);
			
			ForwardSpeed = FS;
			RightSpeed = RS;

			bIsIdle = (ForwardSpeed == 0 && RightSpeed == 0) && (bIsDead == false) && (bIsDefence == false);
		}
	}
}
