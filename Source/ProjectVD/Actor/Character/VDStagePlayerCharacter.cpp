// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Game/StageLevel/VDStagePlayerController.h"


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
	// Mesh Override
	USkeletalMeshComponent* SkeletalMesh = GetMesh();

	// Camera Init
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 400.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// Input init
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ProjectVD/Input/IMC_Default.IMC_Default'"));
	if(InputMappingContextRef.Object != nullptr)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectVD/Input/Actions/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object != nullptr)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectVD/Input/Actions/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object != nullptr)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectVD/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionLookRef.Object != nullptr)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDefendRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectVD/Input/Actions/IA_Defend.IA_Defend'"));
	if (InputActionDefendRef.Object != nullptr)
	{
		DefendAction = InputActionDefendRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDefaultAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectVD/Input/Actions/IA_DefaultAttack.IA_DefaultAttack'"));
	if (InputActionDefaultAttackRef.Object != nullptr)
	{
		DefaultAttackAction = InputActionDefaultAttackRef.Object;
	}
}

// Called when the game starts or when spawned
void AVDStagePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AVDStagePlayerController* PlayerController = CastChecked<AVDStagePlayerController>(GetController());
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AVDStagePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AVDStagePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVDStagePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVDStagePlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVDStagePlayerCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AVDStagePlayerCharacter::JumpBegin);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVDStagePlayerCharacter::JumpEnd);
	EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Ongoing, this, &AVDStagePlayerCharacter::DefendHold);
	EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Canceled, this, &AVDStagePlayerCharacter::DefendCancel);
	EnhancedInputComponent->BindAction(DefaultAttackAction, ETriggerEvent::Triggered, this, &AVDStagePlayerCharacter::DefaultAttack);
}

