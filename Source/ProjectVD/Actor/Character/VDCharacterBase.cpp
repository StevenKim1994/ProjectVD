// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/VDCharacterBase.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Weapon/VDWeapon.h"
// Sets default values
AVDCharacterBase::AVDCharacterBase()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	UCapsuleComponent *Capsule = GetCapsuleComponent();
	Capsule->InitCapsuleSize(42.0f, 96.0f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));

	UCharacterMovementComponent *Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	Movement->JumpZVelocity = 700.0f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.0f;
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;

	USkeletalMeshComponent *SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
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
	CastPlayerController = Cast<AVDStagePlayerController>(GetController());
}

void AVDCharacterBase::EquipWeapon(AVDWeapon* NewWeapon)
{
	if(NewWeapon)
	{
		NewWeapon->Destroy();
		// TODO :: 풀링처리로 변경하기
		
		
		UE_LOG(LogTemp, Warning, TEXT("Equip Weapon: %s"), *NewWeapon->GetName());
		

	}
}

