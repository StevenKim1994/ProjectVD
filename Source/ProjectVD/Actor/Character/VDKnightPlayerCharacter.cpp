// Fill out your copyright notice in the Description page of Project Settings.

#include "VDKnightPlayerCharacter.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AVDKnightPlayerCharacter::AVDKnightPlayerCharacter()
{

}

void AVDKnightPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AVDKnightPlayerCharacter::Look(const FInputActionValue& Value)
{
	Super::Look(Value);

	UE_LOG(LogTemp, Warning, TEXT("AVDKnightPlayerCharacter::Look"));
}

void AVDKnightPlayerCharacter::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
}

void AVDKnightPlayerCharacter::DefaultAttack(const FInputActionValue& Value)
{

}

void AVDKnightPlayerCharacter::Zoom(const FInputActionValue& Value)
{
	Super::Zoom(Value);

	UE_LOG(LogTemp, Warning, TEXT("AVDKnightPlayerCharacter::Zoom"));
}

void AVDKnightPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetCharacterMovement()->IsFalling())
	{

	}
}

void AVDKnightPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CastPlayerController = Cast<AVDStagePlayerController>(GetController());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (CastPlayerController && EnhancedInputComponent)
	{
		UInputMappingContext* DefaultMappingContext = CastPlayerController->GetCharacterControllerIMC();
		if (DefaultMappingContext)
		{
			const TArray<FEnhancedActionKeyMapping>& Mappings = DefaultMappingContext->GetMappings();
			for (const FEnhancedActionKeyMapping& Mapping : Mappings)
			{
				const UInputAction* Action = Mapping.Action;
				if (Action)
				{
					FString ActionName = Action->GetName();
					if (ActionName.StartsWith(TEXT("IA_")))
					{
						ActionName = ActionName.RightChop(3);
						EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, FName(ActionName));
					}
				}
			}
		}
	}
}

float AVDKnightPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Result;
}

void AVDKnightPlayerCharacter::SetComboInputOn(bool bIsOn)
{

}

void AVDKnightPlayerCharacter::DefaultAttackHit()
{
}
