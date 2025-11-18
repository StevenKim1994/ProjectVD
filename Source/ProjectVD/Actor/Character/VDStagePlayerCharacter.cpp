// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Game/StageLevel/VDStagePlayerController.h"

AVDStagePlayerCharacter::AVDStagePlayerCharacter() 
{
	// 플레이어 전용 회전 설정
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 플레이어 전용 이동 설정
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// 플레이어 전용 카메라 설정
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 400.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;
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
	//Super::DefaultAttack(Value);
	UE_LOG(LogTemp, Log, TEXT("Attack Combo Count: %d"), CurrentAttackComboCount);

	if (DefaultAttackAM)
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if (Movement)
		{
			Movement->SetMovementMode(EMovementMode::MOVE_None);
			Movement->StopMovementImmediately();
		}

		if (CurrentAttackComboCount == 0)
		{
			DefaultAttackCombo();
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
	UAnimInstance* UAI = GetMesh()->GetAnimInstance();
	if (UAI && DefaultAttackAM)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AVDStagePlayerCharacter::DefaultAttackComboEnded);
		UAI->Montage_SetEndDelegate(EndDelegate, DefaultAttackAM);
		UAI->Montage_Play(DefaultAttackAM, AttackSpeedRate);
	}
}

void AVDStagePlayerCharacter::DefaultAttackComboEnded(UAnimMontage* AnimMontage, bool IsEndedCombo)
{
	if (AnimMontage)
	{
		CurrentAttackComboCount = 0;

		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if (Movement)
		{
			Movement->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

