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
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->SetGenerateOverlapEvents(true);
}

void AVDEquipItemVisualActor::SetColider(bool bEnable)
{
	if (BoxComp)
	{
		BoxComp->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

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

