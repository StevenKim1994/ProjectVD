// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Public/VDConstrants.h"
#include "Public/VDPhysicInfo.h"
#include "NiagaraComponent.h"

// Sets default values
AVDEquipItemVisualActor::AVDEquipItemVisualActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCollisionProfileName(CPROFILE_NO_COLLISION);
	RootComponent = StaticMeshComp;

	EquipEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EquipEffectComp"));
	EquipEffectComp->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(CPROFILE_WEAPON_OVERLAP_TRIGGER);
	BoxComp->IgnoreActorWhenMoving(GetOwner(), true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AVDEquipItemVisualActor::OnBoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AVDEquipItemVisualActor::OnBoxEndOverlap);

	DetectedActors.Empty();
}

void AVDEquipItemVisualActor::SetColider(bool bEnable)
{
	if (BoxComp)
	{
		BoxComp->SetGenerateOverlapEvents(bEnable);
		BoxComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void AVDEquipItemVisualActor::SetDectedHitListReset()
{
	DetectedActors.Empty();
}

void AVDEquipItemVisualActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVDEquipItemVisualActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVDEquipItemVisualActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (DetectedActors.Contains(OtherActor) == false)
	{
		DetectedActors.Add(OtherActor);

		FVector ContactPoint = FVector::ZeroVector; // DESC :: 오버랩 이벤트에선 접촉지점 정보를 제공하지 않으므로, 대략적인 접촉지점을 계산함.
		FVector ClosestOnOther;
		float DistOnOther;
		DistOnOther = OtherComp->GetClosestPointOnCollision(BoxComp->GetComponentLocation(), ClosestOnOther);
		ContactPoint = (ClosestOnOther + BoxComp->GetComponentLocation()) * 0.5f;

		OnDetectedHitColiderTarget.ExecuteIfBound(OtherActor, ContactPoint);
	}
}

void AVDEquipItemVisualActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

