// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDCharacterBase.h"

#include "VDCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AVDCharacterBase::AVDCharacterBase()
{

}

void AVDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AVDCharacterBase::SetCharacterControlData(const UVDCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

// Called when the game starts or when spawned
void AVDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

