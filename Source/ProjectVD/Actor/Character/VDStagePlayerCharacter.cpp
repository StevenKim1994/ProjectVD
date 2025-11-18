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

void AVDStagePlayerCharacter::EquipWeapon(AVDWeapon* NewWeapon)
{
	Super::EquipWeapon(NewWeapon);

	AVDStagePlayerController* VDPC = CastPlayerController.Get();
	if (VDPC)
	{
		VDPC->ShowToast(TEXT("알림"), TEXT("무기 획득함"));
	}
}

void AVDStagePlayerCharacter::SetComboInputOn_Implementation(bool bIsOn)
{
	bIsNextComboInputOn = bIsOn;
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
	CurrentAttackComboCount = 0;
	ACharacter::Jump();
}

void AVDStagePlayerCharacter::JumpEnd(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

void AVDStagePlayerCharacter::DefaultAttack(const FInputActionValue& Value)
{

	if (IsAirAttack())
	{
		bIsNextComboInputOn = false;
		CurrentAttackComboCount = 0;
		UE_LOG(LogTemp, Log, TEXT("Air Attack"));
		if (AirAttackAM)
		{
			if (UAnimInstance* UAI = GetMesh()->GetAnimInstance())
			{
				UAI->Montage_Play(AirAttackAM, AttackSpeedRate);
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

			UAI->Montage_Play(DefaultAttackAM, AttackSpeedRate);
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

