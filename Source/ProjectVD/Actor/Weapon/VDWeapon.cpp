// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon/VDWeapon.h"
#include "Actor/Character/VDCharacterBase.h"
#include "Components/BoxComponent.h"

// Sets default values
AVDWeapon::AVDWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	
	// 충돌 설정: 오버랩 전용, Pawn과만 오버랩
	HitBox->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBox->SetCollisionObjectType(ECC_WorldDynamic);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->SetGenerateOverlapEvents(true);
	HitBox->SetNotifyRigidBodyCollision(true);
	HitBox->SetupAttachment(Root);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AVDWeapon::OnOverlapBegin);
}

void AVDWeapon::BeginPlay()
{
	Super::BeginPlay();

	HitBox->SetGenerateOverlapEvents(true);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AVDWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 유효성 체크
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 캐릭터만 처리
	if (AVDCharacterBase* Character = Cast<AVDCharacterBase>(OtherActor))
	{
		// 습득 가능 여부 확인
		if (!CanBePicked())
		{
			return;
		}

		UE_LOG(LogTemp, Verbose, TEXT("HitBox Overlap: %s with %s"), *GetName(), *Character->GetName());

		// 습득 처리
		OnPicked(Character);

	
	}
}

void AVDWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sin 기반 수직 바운싱(상하 움직임)
	constexpr float BobAmplitude = 20.f;   // 이동 폭(cm)
	constexpr float BobFrequency = 1.0f;   // 초당 사이클(Hz)
	const float Time = GetGameTimeSinceCreation();
	const float PrevTime = Time - DeltaTime;
	const float Angular = 2.f * PI * BobFrequency;

	const float DeltaZ = BobAmplitude * (FMath::Sin(Angular * Time) - FMath::Sin(Angular * PrevTime));
	if (!FMath::IsNearlyZero(DeltaZ))
	{
		if (Mesh)
		{
			Mesh->AddLocalOffset(FVector(0.f, 0.f, DeltaZ));
		}
	}
}

bool AVDWeapon::CanBePicked()
{
	return true;
}

void AVDWeapon::OnPicked(AActor* Picker)
{
	if (AVDCharacterBase* Character = Cast<AVDCharacterBase>(Picker))
	{
		if (CanBePicked())
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon picked by %s"), *Character->GetName());

			HitBox->SetGenerateOverlapEvents(false);
			HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Character->EquipWeapon(this);
		}
	}
}

void AVDWeapon::OnPooledActivate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVDWeapon::OnPooledDeactivate()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AVDWeapon::ResetForReuse()
{
	PrimaryActorTick.bCanEverTick = true;
}

