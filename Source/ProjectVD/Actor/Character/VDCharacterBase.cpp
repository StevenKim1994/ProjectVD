// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
AVDCharacterBase::AVDCharacterBase()
{
	// CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef = (TEXT("/Script/Engine.SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'"));
	if(CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
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

