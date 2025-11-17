// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Character/VDCharacterBase.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Weapon/VDWeapon.h"

AVDCharacterBase::AVDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->InitCapsuleSize(42.0f, 96.0f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));

	// 메시 설정
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->SetCollisionProfileName(TEXT("CharacterMesh"));

	// Movement 기본 설정
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->JumpZVelocity = 700.0f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.0f;
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;
}

void AVDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVDCharacterBase::EquipWeapon(AVDWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		NewWeapon->Destroy();
		// TODO :: 풀링처리로 변경하기
		
		UE_LOG(LogTemp, Warning, TEXT("Equip Weapon: %s"), *NewWeapon->GetName());
	}
}

void AVDCharacterBase::Move(const FInputActionValue& Value)
{
	// 기본 구현 없음 - 자식 클래스에서 구현
}

void AVDCharacterBase::DefaultAttack(const FInputActionValue& Value)
{
	// 기본 구현 없음 - 자식 클래스에서 구현
}

