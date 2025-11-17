// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Game/StageLevel/VDStagePlayerController.h"


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
	UE_LOG(LogTemp, Log, TEXT("Press Default Attack"));
}

void AVDStagePlayerCharacter::DefendHold(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Hold DefendHold"));
}

void AVDStagePlayerCharacter::DefendCancel(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Log, TEXT("Cancel Defend"));
}

AVDStagePlayerCharacter::AVDStagePlayerCharacter() 
{
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 400.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AVDStagePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (CastPlayerController)
	{
		UInputMappingContext* DefaultMappingContext = CastPlayerController->GetCharacterControllerIMC();
		const TArray<FEnhancedActionKeyMapping>&Mappings = DefaultMappingContext->GetMappings();
		for (const FEnhancedActionKeyMapping& Mapping : Mappings)
		{
			const UInputAction* Action = Mapping.Action;
			if (Action)
			{
				FString ActionName = Action->GetName();
				if (ActionName.StartsWith(TEXT("IA_")))
				{
					ActionName = ActionName.RightChop(3); // "IA_" 길이만큼 잘라냄
					EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, FName(ActionName));
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
		VDPC->ShowToast("알림","무기 획득함");
	}
}

