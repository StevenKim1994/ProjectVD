// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Public/VDPhysicInfo.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Actor/Character/VDCharacterBase.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"
// Sets default values
AVDItemPropActorBase::AVDItemPropActorBase()
{
	PrimaryActorTick.bCanEverTick = bIsHoverable;

	ColiderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ColiderComp"));
	RootComponent = ColiderComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(CPROFILE_NO_COLLISION);

	PickedEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickedEffectComp"));
	PickedEffectComp->SetupAttachment(RootComponent);
	PickedEffectComp->SetAutoActivate(false);
	PickedEffectComp->OnSystemFinished.AddDynamic(this, &AVDItemPropActorBase::OnPickedEffectFinished);
	ColiderComp->SetCollisionProfileName(CPROFILE_PLACE_TRIGGER);
	ColiderComp->OnComponentBeginOverlap.AddDynamic(this, &AVDItemPropActorBase::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AVDItemPropActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeshComp)
	{
		InitialLocation = MeshComp->GetRelativeLocation();
	}
}

// Called every frame
void AVDItemPropActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHoverable && MeshComp)
	{
		HoverTime += DeltaTime * HoverSpeed;
		float NewZ = InitialLocation.Z + (FMath::Sin(HoverTime) * HoverRange);
		HoverTime = FMath::Fmod(HoverTime, PI * 2.0f);

		FVector NewLocation = MeshComp->GetRelativeLocation();
		NewLocation.Z = NewZ;
		MeshComp->SetRelativeLocation(NewLocation);
	}
}

void AVDItemPropActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVDCharacterBase* Character = Cast<AVDCharacterBase>(OtherActor))
	{
		if (CanBePicked())
		{
			OnPicked(OtherActor);
		}
	}
}

void AVDItemPropActorBase::OnPickedEffectFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy(); // TODO :: 풀링변경 필요
}

void AVDItemPropActorBase::ResetItemProp()
{

}

bool AVDItemPropActorBase::CanBePicked()
{
	return true;
}

void AVDItemPropActorBase::OnPicked(AActor* Picker)
{
	if (nullptr == Picker)
	{
		return;
	}

	if (AVDCharacterBase* Character = Cast<AVDCharacterBase>(Picker))
	{
		UVDUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UVDUISubsystem>();
		if (Character->PickItem(this))
		{
			if (PickedEffectComp)
			{
				PickedEffectComp->Activate();
			}
			
			MeshComp->SetHiddenInGame(true);

			UISubsystem->ShowToastMessage(VDConstants::GetToastItemPickupText());
		}
		else
		{
			UISubsystem->ShowToastMessage(VDConstants::GetToastInventoryIsFullText());
		}
	}
}

