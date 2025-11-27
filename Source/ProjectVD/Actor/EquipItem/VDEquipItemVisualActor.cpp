// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Public/VDConstrants.h"
#include "Public/VDPhysicInfo.h"


// Sets default values
AVDEquipItemVisualActor::AVDEquipItemVisualActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCollisionProfileName(CPROFILE_NO_COLLISION);
	RootComponent = StaticMeshComp;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionObjectType(CCHANNEL_PROFILE_CHACRACTER_ACTION);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(CCHANNEL_PROFILE_CHACRACTER_ACTION, ECR_Overlap);
	BoxComp->IgnoreActorWhenMoving(GetOwner(), true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AVDEquipItemVisualActor::OnBoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AVDEquipItemVisualActor::OnBoxEndOverlap);

	DetectedActors.Empty();
}

void AVDEquipItemVisualActor::SetColider(bool bEnable)
{
	if (BoxComp)
	{
		BoxComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		
		if (bEnable)
		{
			DrawDebugBox(
				GetWorld(),
				BoxComp->GetComponentLocation(),
				BoxComp->GetScaledBoxExtent(),
				BoxComp->GetComponentQuat(),
				FColor::Green,
				false,
				2.0f, 
				0,
				2.0f 
			);
		}

		if (bEnable == false)
		{
			DetectedActors.Empty();
		}
	}
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
	if (DetectedActors.Contains(OtherActor) == false)
	{
		DetectedActors.Add(OtherActor);

		OnDetectedHitColiderTarget.ExecuteIfBound(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void AVDEquipItemVisualActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Overlap 종료 처리
}

