// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy/Skeleton/VDEnemySkeletonMarauder.h"
#include "Components/SkeletalMeshComponent.h"

void AVDEnemySkeletonMarauder::BeginPlay()
{
	Super::BeginPlay();
}

AVDEnemySkeletonMarauder::AVDEnemySkeletonMarauder()
{
	LeftHandWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandWeapon"));
	RightHandShield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandShield"));

	if (LeftHandWeapon)
	{
		LeftHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
		LeftHandWeapon->SetRelativeLocation(FVector::ZeroVector);
		LeftHandWeapon->SetRelativeRotation(FRotator::ZeroRotator);
		LeftHandWeapon->SetRelativeScale3D(FVector::OneVector);
	}

	if (RightHandShield)
	{
		RightHandShield->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("hand_rSocket"));
		RightHandShield->SetRelativeRotation(FRotator::ZeroRotator);
		RightHandShield->SetRelativeLocation(FVector::ZeroVector);
		RightHandShield->SetRelativeScale3D(FVector::OneVector);
	}
}
