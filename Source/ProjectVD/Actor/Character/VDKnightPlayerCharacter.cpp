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
#include "Actor/Enemy/VDEnemyCharacterBase.h"

AVDKnightPlayerCharacter::AVDKnightPlayerCharacter()
{
	bIsTargetLocked = EVDLockOnStateType::LockOff;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
}

void AVDKnightPlayerCharacter::SetEquippedWeapon(AVDEquipItemVisualActor* NewWeapon)
{
	Super::SetEquippedWeapon(NewWeapon);
}

void AVDKnightPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVDKnightPlayerCharacter::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void AVDKnightPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (CastingAnimInstance->Montage_IsPlaying(FowardRollingAM))
	{
		return;
	}

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
		if (CurrentAttackComboCount == 0)
		{
			DefaultAttackCombo();
			return;
		}
		else
		{
			if (bIsNextComboInputOn)
			{
				CheckComboInput();
			}
		}
	}

}

void AVDKnightPlayerCharacter::DefaultAttackCombo()
{
	CurrentAttackComboCount = 1;

	if (DefaultAttackAM)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AVDKnightPlayerCharacter::DefaultAttackComboEnded);
		CastingAnimInstance->Montage_Play(DefaultAttackAM, BaseStatsComponent->GetAttackSpeed());
		CastingAnimInstance->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);
	}
}

void AVDKnightPlayerCharacter::DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo)
{
	if (AnimMontage != DefaultAttackAM)
	{
		return;
	}

	CurrentAttackComboCount = 0;
	bIsNextComboInputOn = false;

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AVDKnightPlayerCharacter::CheckComboInput()
{
	FName CurrentSection = *FString::Printf(TEXT("Attack%d"), CurrentAttackComboCount);
	CurrentAttackComboCount = FMath::Clamp(CurrentAttackComboCount + 1, 1, DefaultAttackAM->GetNumSections());
	FName NextSection = *FString::Printf(TEXT("Attack%d"), CurrentAttackComboCount);
	CastingAnimInstance->Montage_SetNextSection(CurrentSection, NextSection, DefaultAttackAM);

	bIsNextComboInputOn = false;

	UE_LOG(LogTemp, Log, TEXT("AVDKnightPlayerCharacter::CheckComboInput : %d, ComboIsOn: %s"), CurrentAttackComboCount, bIsNextComboInputOn ? TEXT("true") : TEXT("false"));
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

void AVDKnightPlayerCharacter::Rooting(const FInputActionValue& Value)
{
	Super::Rooting(Value);
}

void AVDKnightPlayerCharacter::Jump()
{
	// DESC :: 점프 비활성화 이캐릭터는 점프대신 구르기 가능 

	if (FowardRollingAM)
	{
		if(CastingAnimInstance->Montage_IsPlaying(FowardRollingAM))
		{
			return;
		}
		FOnMontageEnded EndDelegate;
		EndDelegate.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
			{
				// TODO :: 구르기 끝나면 처리해야할것들 EX-> 이제부터 스태미나 회복같은거
				UE_LOG(LogTemp, Log, TEXT("VDKnghtCharacter::Foward Rolling Ended"));
			});

		CastingAnimInstance->Montage_Play(FowardRollingAM);
		CastingAnimInstance->Montage_SetEndDelegate(EndDelegate, FowardRollingAM);

		bIsNextComboInputOn = false;
		CurrentAttackComboCount = 0;
	}
}

void AVDKnightPlayerCharacter::WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint)
{
	Super::WeaponColiderHit(OtherActor, ContactPoint);

	if (AVDEnemyCharacterBase* HitEnemy = Cast<AVDEnemyCharacterBase>(OtherActor))
	{

		FDamageEvent DamageEvent;
		float TakeDamage = 0.0f;

		if (HitEnemy->IsBossEnemy())
		{
			AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
			if (VDPC)
			{
				VDPC->ShowBossStateBar(HitEnemy);
			}
		}

		TakeDamage = HitEnemy->TakeDamage(BaseStatsComponent->GetAttackPower(), DamageEvent, Controller, this);
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

void AVDKnightPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	UGameInstance* GI = GetGameInstance();
	if (GI == nullptr)
	{
		return;
	}

	UVDDataTableSubSystem* DS = GI->GetSubsystem<UVDDataTableSubSystem>();
	if (DS)
	{
		FVDCharacterDefaultStats* DataTableInfo = DS->GetDataTableRow<FVDCharacterDefaultStats>(FName(TEXT("CharacterDefaultStats")), FName(TEXT("1")));
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

			UCharacterMovementComponent* Movement = GetCharacterMovement();
			Movement->MaxWalkSpeed = DataTableInfo->MaxMovementSpeed;
			Movement->MinAnalogWalkSpeed = DataTableInfo->MinMovementAnalogSpeed;
		}
	}

	if (CastingAnimInstance)
	{
		CastingAnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	}

	CurrentAttackComboCount = 0;

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
		CastPlayerController->SetCharacter(this);
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

	CastPlayerController->ShakePlayerHitCameraEffect();
	// TODO :: 방향에 따른 피격애님몽타주 재생 및 맞은 방향에 따른 넉백 처리

	return Result;
}

void AVDKnightPlayerCharacter::SetComboInputOn(bool bIsOn)
{
	bIsNextComboInputOn = bIsOn;
}

void AVDKnightPlayerCharacter::DefaultAttackHit()
{
	EquippedWeapon->SetDectedHitListReset();
	EquippedWeapon->SetColider(true);
}

void AVDKnightPlayerCharacter::ResetHitList()
{
	EquippedWeapon->SetDectedHitListReset();
}
