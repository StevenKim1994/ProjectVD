// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectVD/Actor/Character/VDStagePlayerCharacter.h"
#include "ProjectVD/Actor/Enemy/VDEnemyCharacterBase.h"
#include "ProjectVD/Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "ProjectVD/Actor/ActorComponent/VDBaseStaminaComponent.h"
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
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Animation/AnimInstance.h"          
#include "TimerManager.h"                    
#include "Public/VDPhysicInfo.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

AVDStagePlayerCharacter::AVDStagePlayerCharacter() 
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 400.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	StaminaComponent = CreateDefaultSubobject<UVDBaseStaminaComponent>(TEXT("StaminaComponent"));
	StaminaComponent->RegisterComponent();
	StaminaComponent->SetMaxStamina(100.0f);
	StaminaComponent->SetCurrentStamina(100.0f);
	StaminaComponent->SetStaminaRecovery(true);

	BaseStatsComponent = CreateDefaultSubobject<UVDCharacterStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();
	BaseStatsComponent
		->SetAttackRange(70.0f)
		->SetAttackSpeed(1.0f)
		->SetAttackPower(10.0f)
		->SetMaxHealth(100.0f)
		->SetMaxMana(100.0f)
		->SetHealth(100.0f)
		->SetMana(100.0f);
}

void AVDStagePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentAttackComboCount = 0;
}

void AVDStagePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AVDStagePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetCharacterMovement()->IsFalling())
	{
		bHasAirAttacked = false;
	}
}

void AVDStagePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CastPlayerController = Cast<AVDStagePlayerController>(GetController());

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (CastPlayerController)
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

float AVDStagePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void AVDStagePlayerCharacter::SetComboInputOn(bool bIsOn)
{
	bIsNextComboInputOn = bIsOn;
}

void AVDStagePlayerCharacter::DefaultAttackHit()
{
	if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
	{
		UWorld* World = GetWorld();
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(DefaultAttack), false, this);

		// TODO :: 추후 별도로 스탯정보로 뻄
		const float AttackRange = BaseStatsComponent->GetAttackRange();
		const float AttackRadius = 50.0f;
		const float AttackDamage = BaseStatsComponent->GetAttackPower();
		const FVector Start = GetActorLocation() + (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius() );
		const FVector End = Start + (GetActorForwardVector() * AttackRange);

		if (World)
		{
			bool bIsHitCheck = World->SweepSingleByChannel(HitResult,
				Start,
				End,
				FQuat::Identity,
				CCHANNEL_PROFILE_CHACRACTER_ACTION,
				FCollisionShape::MakeSphere(50.0f), CollisionParams);

			FVector DebugDrawLocation = Start + (End - Start) * 0.5f;
			float DebugCapsuleHeight = AttackRange * 0.5f;
			FColor DebugColor = bIsHitCheck ? FColor::Red : FColor::Green;

			DrawDebugCapsule(World, DebugDrawLocation, DebugCapsuleHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DebugColor, false, 2.0f);

			if (bIsHitCheck)
			{
				if (HitResult.GetActor())
				{
					if(AVDEnemyCharacterBase* HitEnemy = Cast<AVDEnemyCharacterBase>(HitResult.GetActor()))
					{ 
						UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

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
						
						TakeDamage = HitEnemy->TakeDamage(AttackDamage, DamageEvent, Controller, this);

						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),  AttackHitEffect, HitResult.ImpactPoint, FRotator::ZeroRotator,FVector::OneVector, true, true,
							ENCPoolMethod::AutoRelease, true);
					}
				}
			}
		}
	}
}

void AVDStagePlayerCharacter::Escape(const FInputActionValue& Value)
{
	AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
	if (VDPC)
	{
		VDPC->OnEscape(Value);
	}
}

void AVDStagePlayerCharacter::Move(const FInputActionValue& Value)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->IsAnyMontagePlaying())
		{
			return;
		}
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AVDStagePlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(-LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AVDStagePlayerCharacter::Zoom(const FInputActionValue& Value)
{
	float ZoomAxis = Value.Get<float>();
	CameraSpringArmComponent->TargetArmLength = FMath::Clamp(CameraSpringArmComponent->TargetArmLength + ZoomAxis * -20.0f, 200.0f, 600.0f);
}

void AVDStagePlayerCharacter::JumpBegin(const FInputActionValue& Value)
{
	const float StaminaCost = 20.0f;
	if (!StaminaComponent->HasStamina(StaminaCost))
	{
		return;
	}

	StaminaComponent->ConsumeStamina(StaminaCost);
	CurrentAttackComboCount = 0;
	Super::Jump();
}

void AVDStagePlayerCharacter::JumpEnd(const FInputActionValue& Value)
{
	Super::StopJumping();
}

void AVDStagePlayerCharacter::RollLeft(const FInputActionValue& Value)
{
	if (RollLeftAM)
	{
		const float StaminaCost = 20.0f;

		if (!StaminaComponent->HasStamina(StaminaCost))
		{
			return;
		}

		StaminaComponent->ConsumeStamina(StaminaCost);
		if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
		{
			UAI->Montage_Play(RollLeftAM, 1.0f);
		}

		FVector DodgeVelocity = -GetActorRightVector() * 1000.f;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		GetCharacterMovement()->Velocity = DodgeVelocity;
	}
}

void AVDStagePlayerCharacter::RollRight(const FInputActionValue& Value)
{
	if (RollRightAM)
	{
		const float StaminaCost = 20.0f;
		if (!StaminaComponent->HasStamina(StaminaCost))
		{
			return;
		}

		StaminaComponent->ConsumeStamina(StaminaCost);
		if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
		{
			UAI->Montage_Play(RollRightAM, 1.0f);
		}

		FVector DodgeVelocity = GetActorRightVector() * 1000.f;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		GetCharacterMovement()->Velocity = DodgeVelocity;
	}
}

void AVDStagePlayerCharacter::DefaultAttack(const FInputActionValue& Value)
{
	//Super::DefaultAttack(Value); DESC :: 부모함수 호출하지 않음.
	if (IsAirAttack())
	{
		const float StaminaCost = 15.0f;
		if (!StaminaComponent->HasStamina(StaminaCost))
		{
			return;
		}

		StaminaComponent->ConsumeStamina(StaminaCost);
		bIsNextComboInputOn = false;
		CurrentAttackComboCount = 0;
		UE_LOG(LogTemp, Log, TEXT("Air Attack"));
		if (AirAttackAM)
		{
			if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
			{
				UAI->Montage_Play(AirAttackAM, BaseStatsComponent->GetAttackSpeed());
				bHasAirAttacked = true; // 공중공격 플래그 설정
			}
		}
	}
	else if(!GetCharacterMovement()->IsFalling())
	{	
		if (DefaultAttackAM)
		{
			if (UCharacterMovementComponent* Movement = GetCharacterMovement())
			{
				Movement->StopMovementImmediately();
				Movement->SetMovementMode(EMovementMode::MOVE_None);
			}

			// 첫 입력: 콤보 시작
			if (CurrentAttackComboCount == 0)
			{
				DefaultAttackCombo();
				return;
			}

			if (bIsNextComboInputOn)
			{
				CheckComboInput();
			}
		}
	}
}

void AVDStagePlayerCharacter::Jump()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->IsAnyMontagePlaying())
		{
			return;
		}
	}
	
	bIsNextComboInputOn = false;
	CurrentAttackComboCount = 0;

	Super::Jump();
}

void AVDStagePlayerCharacter::DefendHold(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Hold DefendHold"));
}

void AVDStagePlayerCharacter::DefendCancel(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Log, TEXT("Cancel Defend"));
}

bool AVDStagePlayerCharacter::IsAirAttack()
{
	bool bIsFalling = GetCharacterMovement()->IsFalling();
	bool bHasAirAttackAM = (AirAttackAM != nullptr);
	bool bIsNotPlayingMontage = (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AirAttackAM) == false);
	bool bIsNearGround = false;
	if (bIsFalling)
	{
		FVector ActorLocation = GetActorLocation();
		FHitResult HitResult;
		FVector Start = ActorLocation;
		FVector End = ActorLocation - FVector(0, 0, 30.0f); // 30cm 아래로 레이캐스트

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bIsNearGround = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	}

	// 공중에서 한번만 공격 가능
	return bIsFalling && bHasAirAttackAM && bIsNotPlayingMontage && !bIsNearGround && !bHasAirAttacked;
}

void AVDStagePlayerCharacter::DefaultAttackCombo()
{
	CurrentAttackComboCount = 1;

	if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
	{
		if (DefaultAttackAM)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AVDStagePlayerCharacter::DefaultAttackComboEnded);

			UAI->Montage_Play(DefaultAttackAM, BaseStatsComponent->GetAttackSpeed());
			UAI->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);
		}
	}
}

void AVDStagePlayerCharacter::DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo)
{
	if (AnimMontage != DefaultAttackAM) return;

	CurrentAttackComboCount = 0;
	bIsNextComboInputOn = false;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AVDStagePlayerCharacter::CheckComboInput()
{
	// DESC :: 콤보가능 여부를 체크하는 bIsNextComboInputOn 변수가 true일 때만 콤보 공격을 이어감 이값은 애님노티파이스테이트에서 설정함.
	if (bIsNextComboInputOn)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentAttackComboCount = FMath::Clamp(CurrentAttackComboCount + 1, 1, DefaultAttackAM->GetNumSections() );
		FName NextSection = *FString::Printf(TEXT("Combo%d"), CurrentAttackComboCount);
		AnimInstance->Montage_JumpToSection(NextSection, DefaultAttackAM);
		bIsNextComboInputOn = false;
	}
}

