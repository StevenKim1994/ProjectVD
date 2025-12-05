// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Character/VDCharacterBase.h"
#include "Actor/ActorComponent/VDCharacterStatsBaseComponent.h"
#include "Actor/ActorComponent/VDInventoryComponent.h"
#include "Actor/ActorComponent/VDBaseStaminaComponent.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Engine/DamageEvents.h"
#include "Animation/VDAnimInstance.h"
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
	SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -Capsule->GetScaledCapsuleHalfHeight()));

	// Movement 기본 설정
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->JumpZVelocity = 700.0f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.0f; 
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

	StaminaComponent = CreateDefaultSubobject<UVDBaseStaminaComponent>(TEXT("StaminaComponent"));
	StaminaComponent->RegisterComponent();
	StaminaComponent->SetMaxStamina(100.0f);
	StaminaComponent->SetCurrentStamina(100.0f);
	StaminaComponent->SetStaminaRecovery(true);
}

void AVDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UAnimInstance* AnimIns = GetMesh()->GetAnimInstance())
	{
		CastingAnimInstance = Cast<UVDAnimInstance>(AnimIns);
	}
}

void AVDCharacterBase::FirstOverlappingItemPickUp()
{
	if (OverlappingItemList.Num() == 0)
	{
		return;
	}

	if (InventoryComponent == nullptr)
	{
		return;
	}

	for (TWeakObjectPtr<AVDItemPropActorBase> ItemPtr : OverlappingItemList)
	{
		AVDItemPropActorBase* Item = ItemPtr.Get();
		if (Item)
		{
			if (InventoryComponent->AddItemToInventory(Item))
			{
				Item->Destroy(true); // TODO :: 풀링변경 필요
				break;
			}
		}
	}
}


void AVDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (BaseStatsComponent)
	{
		BaseStatsComponent->GetOnChangeHealth().AddWeakLambda(this, [this](float CurrentHealth, float MaxHealth)
		{
			if (CurrentHealth <= 0.0f)
			{
				// TODO :: 캐릭터 사망 처리
				if (DeathAM)
				{
					UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
					if (AnimInstance && !AnimInstance->Montage_IsPlaying(DeathAM))
					{
						AnimInstance->Montage_Play(DeathAM);
					}

					GetCharacterMovement()->DisableMovement();
				
				}

				FTimerHandle DeathTimerHandle;
				AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
				VDPC->SetGameOver();

				UE_LOG(LogTemp, Warning, TEXT("AVDCharacterBase::BeginPlay Character Dead"));
			}
		});
	}
}

void AVDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDCharacterBase::AddOverlappingItem(AVDItemPropActorBase* Item)
{
	if (Item)
	{
		if (OverlappingItemList.Contains(Item))
		{
			return;
		}

		OverlappingItemList.Add(Item);
		
		AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
		if(VDPC)
		{
			VDPC->ShowInteractionWidget(FText::FromString(TEXT("아이템줍기")), FText::FromString(TEXT("G")));
		}
	}
}

void AVDCharacterBase::RemoveOverlappingItem(AVDItemPropActorBase* Item)
{
	if (Item)
	{
		if (OverlappingItemList.Contains(Item))
		{
			OverlappingItemList.Remove(Item);
		}
	}

	if (OverlappingItemList.Num() == 0)
	{
		AVDStagePlayerController* VDPC = Cast<AVDStagePlayerController>(Controller);
		if(VDPC)
		{
			VDPC->HideInteractionWidget();
		}
	}
}

bool AVDCharacterBase::PickItem(AVDItemPropActorBase* Item)
{
	if (nullptr == InventoryComponent)
	{
		return false;
	}

	if (InventoryComponent->AddItemToInventory(Item))
	{
		Item->Destroy(true); // TODO :: 풀링변경 필요

		return true;
	}

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

void AVDCharacterBase::LockOnTarget(const FInputActionValue& Value)
{
	
}

void AVDCharacterBase::Rooting(const FInputActionValue& Value)
{
	if (RootingAM)
	{
		if (!CastingAnimInstance->Montage_IsPlaying(RootingAM))
		{
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
			CastingAnimInstance->Montage_Play(RootingAM);
			TWeakObjectPtr<AVDCharacterBase> SelfWeak = this;

			FOnMontageEnded EndDel;
			EndDel.BindWeakLambda(this, [SelfWeak](UAnimMontage* Montage, bool bInterrupted)
				{
					if (!SelfWeak.IsValid())
					{
						return;
					}

					AVDCharacterBase* Self = SelfWeak.Get();
					Self->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				});

			CastingAnimInstance->Montage_SetEndDelegate(EndDel);
		}
	}

	FirstOverlappingItemPickUp();
}

void AVDCharacterBase::WeaponColiderHit(AActor* OtherActor, const FVector& ContactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("Weapon Colider Hit Actor : %s"), *OtherActor->GetName());

	if (OtherActor && OtherActor != this)
	{
		OtherActor->TakeDamage(BaseStatsComponent->GetAttackPower(), FDamageEvent(), GetController(), this);
	}
}

