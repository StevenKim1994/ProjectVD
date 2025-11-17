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
	HitBox->SetupAttachment(Root);
}

void AVDWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AVDWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AVDWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			// TODO :: 획득처리 추가
		}
	}
}

