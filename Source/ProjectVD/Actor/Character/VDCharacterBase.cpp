// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Character/VDCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Actor/ActorComponent/VDInventoryComponent.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Engine/DamageEvents.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Game/StageLevel/VDStagePlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/VDDataTableSubSystem.h"
#include "System/VDUISubsystem.h"
#include "Object/VDInventoryInfo.h"
#include "Public/VDPhysicInfo.h"
#include "Public/VDItemType.h"
#include "Public/VDEquipType.h"
#include "DataTable/VDItemInfoTable.h"

AVDCharacterBase::AVDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->InitCapsuleSize(42.0f, 96.0f);
	Capsule->SetCollisionProfileName(CPROFILE_CHARACTER_CAPSULE);

	// 메시 설정
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->SetCollisionProfileName(CPROFILE_NO_COLLISION);

	// Movement 기본 설정
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->JumpZVelocity = 700.0f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.0f; // TODO :: 속도 테이블로 빼기
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	InventoryComponent = CreateDefaultSubobject<UVDInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->RegisterComponent();

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 400.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	BaseStatsComponent = CreateDefaultSubobject<UVDCharacterStatsBaseComponent>(TEXT("BaseStatsComponent"));
	BaseStatsComponent->RegisterComponent();
}

void AVDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AVDCharacterBase::PickItem(AVDItemPropActorBase* Item)
{
	if (nullptr == InventoryComponent)
	{
		return false;
	}

	if (InventoryComponent->AddItemToInventory(Item))
	{
		Item->SetMeshHidden(true); // TODO :: 풀링변경 필요

		return true;
	}
	/*
	UClass* LoadedClass = VisualActor.LoadSynchronous();
	if (LoadedClass)
	{
		AVDEquipItemVisualActor* SpawnedVisualActor = GetWorld()->SpawnActor<AVDEquipItemVisualActor>(LoadedClass);
		if (SpawnedVisualActor)
		{
			SetEquippedWeapon(SpawnedVisualActor);
		}
	}
	*/
	return false;
}

void AVDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AVDCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(BaseStatsComponent)
	{
		BaseStatsComponent->SetHealth(BaseStatsComponent->GetHealth() - DamageAmount);
	}

	UE_LOG(LogTemp, Warning, TEXT("AVDCharacterBase::TakeDamage Current Health : %f"), BaseStatsComponent->GetHealth());

	return DamageAmount;
}

bool AVDCharacterBase::UpdateEquippedItem(EVDEquipType EquipType, FName ItemID)
{
	if (EquippedArmorMap.Contains(EquipType))
	{
		EquippedArmorMap[EquipType] = ItemID;
	}
	else
	{
		EquippedArmorMap.Add(EquipType, ItemID);
	}

	// TODO :: 장비아이템 능력치 StatComponent에 반영 방어구 아이템은 능력치만 처리함.

	return true;
}

bool AVDCharacterBase::UseConsumeableItem(UVDInventoryInfo* ItemInfo)
{

	// TODO :: 소비아이템 사용 효과 StatComponent에 반영

	return true;
}

void AVDCharacterBase::SetEquippedWeapon(AVDEquipItemVisualActor* NewWeapon)
{
	if (NewWeapon)
	{
		EquippedWeapon = NewWeapon;
		EquippedWeapon->SetOwner(this);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		EquippedWeapon->GetOnDetectedHitColiderTarget().Unbind();
		EquippedWeapon->GetOnDetectedHitColiderTarget().BindUObject(this, &AVDCharacterBase::WeaponColiderHit);
		GetGameInstance()->GetSubsystem<UVDUISubsystem>()->ShowToastMessage(FText::FromString(TEXT("무기 장착 완료")));
	}
}

void AVDCharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AVDCharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(-LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AVDCharacterBase::Zoom(const FInputActionValue& Value)
{
	float ZoomAxis = Value.Get<float>();
	CameraSpringArmComponent->TargetArmLength = FMath::Clamp(CameraSpringArmComponent->TargetArmLength + ZoomAxis * -20.0f, 200.0f, 600.0f);
}

void AVDCharacterBase::Escape(const FInputActionValue& Value)
{
	AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
	if (VDPC)
	{
		VDPC->OnEscape(Value);
	}
}

void AVDCharacterBase::DefaultAttack(const FInputActionValue& Value)
{
	if (DefaultAttackAM)
	{
		GetCharacterMovement()->StopMovementImmediately();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DefaultAttackAM);
		}
	}
}

void AVDCharacterBase::Inventory(const FInputActionValue& Value)
{
	AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
	if (VDPC)
	{
		VDPC->OnInventory(Value);
	}
}

void AVDCharacterBase::WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("Weapon Colider Hit Actor : %s"), *OtherActor->GetName());

	if (OtherActor && OtherActor != this)
	{
		OtherActor->TakeDamage(10.0f, FDamageEvent(), GetController(), this);
	}
}

