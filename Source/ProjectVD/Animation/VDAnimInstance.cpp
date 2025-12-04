// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VDAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

UVDAnimInstance::UVDAnimInstance()
{
	MovingThreshold = 3.f;
	JumpingThreshold = 100.f;
}

void UVDAnimInstance::SetIsLockOnTarget(uint8 bIsLockOn)
{
	bIsLockOnTarget = bIsLockOn;
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
			Velocity = OwnerCharacterMovement->Velocity;
			GroundSpeed = Velocity.Size2D();
			Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());

			const FVector Forward = OwnerCharacter->GetActorForwardVector();
			const FVector Vel2D(Velocity.X, Velocity.Y, 0.f);
			const float Dot = FVector::DotProduct(Forward, Vel2D.GetSafeNormal());

			// 반대 방향(뒷걸음질) 입력 시 음수 방향으로 처리
			Direction = (Dot < 0.f) ? -FMath::Abs(Direction) : FMath::Abs(Direction);

			// Direction이 음수일 시 GroundSpeed도 음수 처리
			GroundSpeed = (Direction < 0.f) ? -FMath::Abs(GroundSpeed) : FMath::Abs(GroundSpeed);

			bIsIdle = GroundSpeed < MovingThreshold;
			bIsFalling = OwnerCharacterMovement->IsFalling();
			bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
			UE_LOG(LogTemp, Log, TEXT("UVDAnimInstance::NativeUpdateAnimation() GroundSpeed: %f, Direction: %f, bIsIdle: %d, bIsFalling: %d, bIsJumping: %d"),
			GroundSpeed, Direction, bIsIdle, bIsFalling, bIsJumping);
		}
		else
		{
			Velocity = OwnerCharacterMovement->Velocity;
			GroundSpeed = Velocity.Size2D();
			Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
			bIsIdle = GroundSpeed < MovingThreshold;
			bIsFalling = OwnerCharacterMovement->IsFalling();
			bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		}
	}
}
