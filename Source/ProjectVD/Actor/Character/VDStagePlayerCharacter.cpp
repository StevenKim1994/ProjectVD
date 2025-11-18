// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Animation/AnimInstance.h"          
#include "TimerManager.h"                    

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

	BaseStatsComponent = CreateDefaultSubobject<UVDCharacterStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();
	BaseStatsComponent
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

void AVDStagePlayerCharacter::EquipWeapon(AVDWeapon* NewWeapon)
{
	Super::EquipWeapon(NewWeapon);

	AVDStagePlayerController* VDPC = CastPlayerController.Get();
	if (VDPC)
	{
		VDPC->ShowToast(TEXT("알림"), TEXT("무기 획득함"));
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

	ACharacter::Jump();
}

void AVDStagePlayerCharacter::JumpEnd(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

void AVDStagePlayerCharacter::DefaultAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Attack Combo Count: %d"), CurrentAttackComboCount);

	if (!DefaultAttackAM) return;

	// 이동 잠금
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(EMovementMode::MOVE_None);
		Movement->StopMovementImmediately();
	}

	// 첫 입력: 콤보 시작
	if (CurrentAttackComboCount == 0)
	{
		DefaultAttackCombo();
		return;
	}

	// 콤보 윈도우 내 추가 입력 여부 표시
	bIsNextComboInputOn = AttackComboResetTimerHandle.IsValid();
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

void AVDStagePlayerCharacter::DefaultAttackCombo()
{
	CurrentAttackComboCount = 1;

	if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
	{
		if (DefaultAttackAM)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AVDStagePlayerCharacter::DefaultAttackComboEnded);

			// 안전한 속도
			const float SafeRate = FMath::Max(AttackSpeedRate, 0.1f);
			UAI->Montage_Play(DefaultAttackAM, SafeRate);
			UAI->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);

			AttackComboResetTimerHandle.Invalidate();
			SetComboCheckTimer();
		}
	}
}

void AVDStagePlayerCharacter::DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo)
{
	if (AnimMontage != DefaultAttackAM) return;

	CurrentAttackComboCount = 0;
	bIsNextComboInputOn = false;
	AttackComboResetTimerHandle.Invalidate();

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AVDStagePlayerCharacter::SetComboCheckTimer()
{
	if (!DefaultAttackAM) return;

	const int32 CurrentComboIndex = FMath::Clamp(CurrentAttackComboCount - 1, 0, DefaultAttackAM->GetNumSections() - 1);

	// 현재 섹션 길이 가져오기
	const float SectionLen = DefaultAttackAM->GetSectionLength(CurrentComboIndex);
	if (SectionLen <= 0.f) return;

	const float SafeRate = FMath::Max(AttackSpeedRate, 0.1f);
	const float EffectiveTime = SectionLen / SafeRate;

	// 너무 큰 시간 방지
	if (EffectiveTime > 0.f && EffectiveTime < 60.f)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackComboResetTimerHandle, this, &AVDStagePlayerCharacter::CheckComboInput, EffectiveTime, false);
	}
}

void AVDStagePlayerCharacter::CheckComboInput()
{
	AttackComboResetTimerHandle.Invalidate();

	if (!DefaultAttackAM) return;
	if (CurrentAttackComboCount <= 0) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 입력이 없으면 콤보 종료 대기
	if (!bIsNextComboInputOn)
	{
		// 입력 없을 시 다음 섹션으로 강제 점프하지 않음. 몽타주 종료 시 EndDelegate에서 상태 복구.
		UE_LOG(LogTemp, Verbose, TEXT("No next combo input. Waiting montage to end."));
		return;
	}

	// 다음 콤보로 증가(클램프)
	CurrentAttackComboCount = FMath::Clamp(CurrentAttackComboCount + 1, 1, MaxAttackComboCount);

	// 다음 섹션 인덱스
	const int32 NextSectionIndex = FMath::Clamp(CurrentAttackComboCount - 1, 0, DefaultAttackAM->GetNumSections() - 1);

	// 섹션 이름을 안전하게 가져오기(이름 포맷 실수 방지)
	const FName NextSectionName = DefaultAttackAM->GetSectionName(NextSectionIndex);
	if (NextSectionName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid section index %d for montage %s"), NextSectionIndex, *DefaultAttackAM->GetName());
		return;
	}

	// 명시적으로 해당 몽타주에 대해 섹션 점프
	AnimInstance->Montage_JumpToSection(NextSectionName, DefaultAttackAM);

	// 다음 입력 대기 상태 초기화
	bIsNextComboInputOn = false;

	// 다음 섹션 타이머 설정
	SetComboCheckTimer();
}

