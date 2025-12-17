// Fill out your copyright notice in the Description page of Project Settings.

#include "VDKnightPlayerCharacter.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"
#include "Actor/ActorComponent/VDHitStopComponent.h"
#include "Actor/ActorComponent/VDTargetLockOnComponent.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Actor/Enemy/VDEnemyCharacterBase.h"
#include "Animation/VDAnimInstance.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "System/VDDataTableSubsystem.h"
#include "System/VDUISubsystem.h"
#include "DataTable/VDCharacterDefaultStats.h"

AVDKnightPlayerCharacter::AVDKnightPlayerCharacter()
{
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
	if (!EquippedWeapon)
	{
		return;
	}



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
		const float StaminaCost = 10.0f;
		if (StaminaComponent->ConsumeStamina(StaminaCost))
		{
			CastingAnimInstance->Montage_Play(DefaultAttackAM, BaseStatsComponent->GetAttackSpeed());
			CastingAnimInstance->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);
		}
	}
}

void AVDKnightPlayerCharacter::DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo)
{
	if (AnimMontage != DefaultAttackAM)
	{
		return;
	}

	if (EquippedWeapon)
	{
		EquippedWeapon->SetColider(false);
		EquippedWeapon->SetDectedHitListReset();
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

	const float StaminaCost = 10.0f;
	//if (StaminaComponent->ConsumeStamina(StaminaCost))
	{
		FOnMontageSectionChanged SectionChangedDelegate;

		SectionChangedDelegate.BindWeakLambda(this, [this](UAnimMontage* Montage, FName SectionName, bool bLoop)
			{
				UE_LOG(LogTemp, Log, TEXT("AVDKnightPlayerCharacter::CheckComboInput - Section Changed : %s"), *SectionName.ToString());
			});

		CastingAnimInstance->Montage_SetNextSection(CurrentSection, NextSection, DefaultAttackAM);
		CastingAnimInstance->Montage_SetSectionChangedDelegate(SectionChangedDelegate, DefaultAttackAM);
		bIsNextComboInputOn = false;

		UE_LOG(LogTemp, Log, TEXT("AVDKnightPlayerCharacter::CheckComboInput : %d, ComboIsOn: %s"), CurrentAttackComboCount, bIsNextComboInputOn ? TEXT("true") : TEXT("false"));
	}
}

void AVDKnightPlayerCharacter::Zoom(const FInputActionValue& Value)
{
	Super::Zoom(Value);
}

void AVDKnightPlayerCharacter::LockOnTarget(const FInputActionValue& Value)
{
	Super::LockOnTarget(Value);
}

void AVDKnightPlayerCharacter::Rooting(const FInputActionValue& Value)
{
	Super::Rooting(Value);
}

void AVDKnightPlayerCharacter::Defence(const FInputActionValue& Value)
{
	if (!EquippedWeapon)
	{
		return;
	}

	Super::Defence(Value);
}

void AVDKnightPlayerCharacter::Jump()
{
	// DESC :: 점프 비활성화 이캐릭터는 점프대신 구르기 가능 

	if (FowardRollingAM)
	{
		if (CastingAnimInstance->Montage_IsPlaying(FowardRollingAM))
		{
			return;
		}
		const float StaminaCost = 20.0f;
		if (StaminaComponent->ConsumeStamina(StaminaCost))
		{
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
}

void AVDKnightPlayerCharacter::RollLeft(const FInputActionValue& Value)
{
	Super::RollLeft(Value);
	UE_LOG(LogTemp, Warning, TEXT("AVDKnightPlayerCharacter::RollLeft"));

	if (FowardRollingAM == nullptr || CastingAnimInstance == nullptr)
	{
		return;
	}

	if (CastingAnimInstance->Montage_IsPlaying(FowardRollingAM))
	{
		return;
	}

	const float StaminaCost = 20.0f;
	if (StaminaComponent->ConsumeStamina(StaminaCost))
	{
		TargetLockOnComponent->Deactivate();

		const FRotator CurrentRot = GetActorRotation();
		const FRotator LeftFacingRot = FRotator(0.0f, CurrentRot.Yaw - 90.0f, 0.0f);
		SetActorRotation(LeftFacingRot);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindWeakLambda(this, [this, CurrentRot](UAnimMontage* Montage, bool bInterrupted)
			{
				UE_LOG(LogTemp, Log, TEXT("VDKnightCharacter::Left Rolling Ended"));
				this->TargetLockOnComponent->Activate();
				this->SetActorRotation(FRotator(0.0f, CurrentRot.Yaw, 0.0f));
			});

		CastingAnimInstance->Montage_Play(FowardRollingAM);
		CastingAnimInstance->Montage_SetEndDelegate(EndDelegate, FowardRollingAM);

		// 콤보 입력 초기화
		bIsNextComboInputOn = false;
		CurrentAttackComboCount = 0;
	}
}

void AVDKnightPlayerCharacter::RollRight(const FInputActionValue& Value)
{
	Super::RollRight(Value);
	UE_LOG(LogTemp, Warning, TEXT("AVDKnightPlayerCharacter::RollRight"));

	if (FowardRollingAM == nullptr || CastingAnimInstance == nullptr)
	{
		return;
	}

	// 이미 롤링 중이면 중복 재생 방지
	if (CastingAnimInstance->Montage_IsPlaying(FowardRollingAM))
	{
		return;
	}

	const float StaminaCost = 20.0f;
	if(StaminaComponent->ConsumeStamina(StaminaCost))
	{ 
		TargetLockOnComponent->Deactivate();
		const FRotator CurrentRot = GetActorRotation();
		const FRotator LeftFacingRot = FRotator(0.0f, CurrentRot.Yaw + 90.0f, 0.0f);
		SetActorRotation(LeftFacingRot);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindWeakLambda(this, [this, CurrentRot](UAnimMontage* Montage, bool bInterrupted)
			{
				UE_LOG(LogTemp, Log, TEXT("VDKnightCharacter::Left Rolling Ended"));
				this->SetActorRotation(FRotator(0.0f, CurrentRot.Yaw, 0.0f));
				this->TargetLockOnComponent->Activate();
			});

		CastingAnimInstance->Montage_Play(FowardRollingAM);
		CastingAnimInstance->Montage_SetEndDelegate(EndDelegate, FowardRollingAM);

		// 콤보 입력 초기화
		bIsNextComboInputOn = false;
		CurrentAttackComboCount = 0;
	}
}

void AVDKnightPlayerCharacter::WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint)
{
	Super::WeaponColiderHit(OtherActor, ContactPoint);
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
}

void AVDKnightPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AVDKnightPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Result;
}

void AVDKnightPlayerCharacter::SetComboInputOn(bool bIsOn)
{
	bIsNextComboInputOn = bIsOn;
	if (!bIsNextComboInputOn)
	{
		EquippedWeapon->SetColider(false);
		EquippedWeapon->SetDectedHitListReset();
	}
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
