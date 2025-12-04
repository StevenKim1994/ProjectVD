// Fill out your copyright notice in the Description page of Project Settings.

#include "VDKnightPlayerCharacter.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Animation/VDAnimInstance.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/OverlapResult.h"
#include "System/VDDataTableSubsystem.h"
#include "System/VDUISubsystem.h"
#include "DataTable/VDCharacterDefaultStats.h"
#include "Public/VDLockOnStateType.h"

AVDKnightPlayerCharacter::AVDKnightPlayerCharacter()
{
	bIsTargetLocked = EVDLockOnStateType::LockOff;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
}

void AVDKnightPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	FVDCharacterDefaultStats* DataTableInfo = GetGameInstance()->GetSubsystem<UVDDataTableSubSystem>()->GetDataTableRow<FVDCharacterDefaultStats>(FName(TEXT("CharacterDefaultStats")), FName(TEXT("1")));

	if (CastingAnimInstance)
	{
		CastingAnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	}

	if (DataTableInfo)
	{
		BaseStatsComponent
			->SetAttackRange(DataTableInfo->AttackRange)
			->SetAttackSpeed(DataTableInfo->AttackSpeed)
			->SetAttackPower(DataTableInfo->AttackPower)
			->SetMaxHealth(DataTableInfo->MaxHealth)
			->SetMaxMana(DataTableInfo->MaxMana)
			->SetHealth(DataTableInfo->MaxHealth)
			->SetMana(DataTableInfo->MaxMana);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AVDStagePlayerCharacter::BeginPlay() DataTableInfo is nullptr"));
	}

	CurrentAttackComboCount = 0;
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
	if (EquippedWeapon == nullptr)
	{
		GetGameInstance()->GetSubsystem<UVDUISubsystem>()->ShowToastMessage(FText::FromString(TEXT("무기가 장착되어 있지 않습니다.")));
		return;
	}

	const float StaminaCost = 0.0f;//15.0f;

	if (DefaultAttackAM)
	{
		CurrentAttackComboCount++;
		if (CurrentAttackComboCount > MaxAttackComboCount)
		{
			CurrentAttackComboCount = 1;
		}
		FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), CurrentAttackComboCount));
		CastingAnimInstance->Montage_JumpToSection(SectionName, DefaultAttackAM);
	}

}

void AVDKnightPlayerCharacter::Zoom(const FInputActionValue& Value)
{
	Super::Zoom(Value);

	UE_LOG(LogTemp, Warning, TEXT("AVDKnightPlayerCharacter::Zoom"));
}

void AVDKnightPlayerCharacter::LockOnTarget(const FInputActionValue& Value)
{
	AActor* TargetActor = nullptr;

	const float MaxLockOnDistance = 1000.0f;
	const FVector Origin = GetActorLocation();
	UWorld* World = GetWorld();
	if (World)
	{
		TArray<FOverlapResult> Overlaps;
		FCollisionObjectQueryParams ObjQueryParams;
		ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LockOnOverlap), false, this);

		const FCollisionShape SphereShape = FCollisionShape::MakeSphere(MaxLockOnDistance);
		const bool bHasOverlaps = World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, ObjQueryParams, SphereShape, QueryParams);

		if (bHasOverlaps)
		{
			float ClosestDistSqr = TNumericLimits<float>::Max();
			for (const FOverlapResult& Result : Overlaps)
			{
				AActor* OverlappedActor = Result.GetActor();
				if (!OverlappedActor || OverlappedActor == this)
				{
					continue;
				}

				const float DistSqr = FVector::DistSquared(Origin, OverlappedActor->GetActorLocation());
				if (DistSqr < ClosestDistSqr)
				{
					ClosestDistSqr = DistSqr;
					TargetActor = OverlappedActor;
				}
			}
		}
	}

	if (TargetActor)
	{
		bIsTargetLocked = (bIsTargetLocked == EVDLockOnStateType::LockOff) ? EVDLockOnStateType::LockOn : EVDLockOnStateType::LockOff;

		if (bIsTargetLocked == EVDLockOnStateType::LockOn)
		{
			LockedTargetActor = TargetActor;
		}
	}
	else
	{
		bIsTargetLocked = EVDLockOnStateType::LockOff;
	}
}

void AVDKnightPlayerCharacter::Jump()
{
	// DESC :: 점프 비활성화

	if (FowardRollingAM)
	{
		CastingAnimInstance->Montage_Play(FowardRollingAM);
	}
}

void AVDKnightPlayerCharacter::TargetLockOn(AActor* TargetActor)
{
	if (TargetActor)
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->bOrientRotationToMovement = true;

		if (CastingAnimInstance)
		{
			CastingAnimInstance->SetIsLockOnTarget(true);
		}
	}
}

void AVDKnightPlayerCharacter::TargetLockOff()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = false;

	if (CastingAnimInstance)
	{
		CastingAnimInstance->SetIsLockOnTarget(false);
	}
}

void AVDKnightPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTargetLocked == EVDLockOnStateType::LockOn)
	{
		if (LockedTargetActor.IsValid())
		{
			FRotator LookAtRotation = (LockedTargetActor->GetActorLocation() - GetActorLocation()).Rotation();
			LookAtRotation.Pitch = 0.0f;
			LookAtRotation.Roll = 0.0f;

			SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, 10.0f));
			if (CastPlayerController)
			{
				CastPlayerController->SetControlRotation(LookAtRotation);
			}
		}
		else
		{
			bIsTargetLocked = EVDLockOnStateType::LockOff;
		}
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
	bIsNextComboInputOn = bIsOn;
	EquippedWeapon->SetColider(bIsOn);
	EquippedWeapon->SetDectedHitListReset();
}

void AVDKnightPlayerCharacter::DefaultAttackHit()
{
	
}
