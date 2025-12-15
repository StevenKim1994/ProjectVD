// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Public/VDPhysicInfo.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Actor/Character/VDCharacterBase.h"
#include "System/VDUISubsystem.h"
#include "Public/VDConstrants.h"
#include "Blueprint/UserWidget.h"
#include "UI/InGame/VDItemPropNamePlateWidget.h"

// Sets default values
AVDItemPropActorBase::AVDItemPropActorBase()
{

	ColiderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ColiderComp"));
	RootComponent = ColiderComp;

	NamePlateWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlateWidgetComp"));
	NamePlateWidgetComp->SetupAttachment(RootComponent);
	NamePlateWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	NamePlateWidgetComp->SetDrawAtDesiredSize(true);
	NamePlateWidgetComp->SetTwoSided(true);
	NamePlateWidgetComp->SetPivot(FVector2D(0.5f, 1.0f));
	NamePlateWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(CPROFILE_NO_COLLISION);

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(RootComponent);
	EffectComp->SetAutoActivate(false);
	EffectComp->OnSystemFinished.AddDynamic(this, &AVDItemPropActorBase::OnPickedEffectFinished);
	ColiderComp->SetCollisionProfileName(CPROFILE_PLACE_TRIGGER);
	ColiderComp->OnComponentBeginOverlap.AddDynamic(this, &AVDItemPropActorBase::OnBeginOverlap);
	ColiderComp->OnComponentEndOverlap.AddDynamic(this, &AVDItemPropActorBase::OnEndOverlap);

	PrimaryActorTick.bCanEverTick = bIsHoverable;
}

FName AVDItemPropActorBase::GetItemInfoName()
{
	return ItemInfoTableRowName.RowName;
}

// Called when the game starts or when spawned
void AVDItemPropActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (MeshComp)
	{
		InitialLocation = MeshComp->GetRelativeLocation();
	}

	if (NamePlateWidgetComp)
	{
		NamePlateWidget = Cast<UVDItemPropNamePlateWidget>(NamePlateWidgetComp->GetUserWidgetObject());
		if (NamePlateWidget == nullptr)
		{
			return;
		}

		NamePlateWidget->SetItemNameText(FText::FromName(ItemInfoTableRowName.RowName));
		NamePlateWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(GroundEffect && EffectComp)
	{
		EffectComp->SetAsset(GroundEffect);
		EffectComp->Activate();
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
		PickerCharacter = Character;
		if (PickerCharacter.IsValid() == false)
		{
			return;
		}

		if (CanBePicked())
		{
			PickerCharacter->AddOverlappingItem(this);
			ShowNamePlateWidget(true);
		}
	}
}

void AVDItemPropActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AVDCharacterBase* Character = Cast<AVDCharacterBase>(OtherActor))
	{
		if (PickerCharacter.IsValid())
		{
			PickerCharacter->RemoveOverlappingItem(this);
			PickerCharacter = nullptr;
			ShowNamePlateWidget(false);
		}
	}
}

void AVDItemPropActorBase::ShowNamePlateWidget(bool bIsShow)
{
	if(NamePlateWidget)
	{
		if (!NamePlateWidget->IsVisible())
		{
			NamePlateWidget->SetVisibility(ESlateVisibility::Visible);
		}

		NamePlateWidget->ShowNamePlate(bIsShow);
	}
}



void AVDItemPropActorBase::OnPickedEffectFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy(); // TODO :: 풀링변경 필요
}

void AVDItemPropActorBase::ResetItemProp()
{

}

void AVDItemPropActorBase::SetMeshHidden(bool bIsHidden)
{
	if (MeshComp)
	{
		MeshComp->SetHiddenInGame(bIsHidden);
	}
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
			if (EffectComp && PickedEffect)
			{
				EffectComp->DeactivateImmediate();
				EffectComp->SetAsset(PickedEffect);
				EffectComp->Activate();
				EffectComp->ResetSystem();
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

