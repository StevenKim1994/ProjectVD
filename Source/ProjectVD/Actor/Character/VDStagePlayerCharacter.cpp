// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDStagePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVDStagePlayerCharacter::AVDStagePlayerCharacter() 
{
	// Camera
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 350.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// Class
	CharacterClass = EClassType::Knight;
}

// Called when the game starts or when spawned
void AVDStagePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		EnableInput(PlayerController);
	}
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

}

