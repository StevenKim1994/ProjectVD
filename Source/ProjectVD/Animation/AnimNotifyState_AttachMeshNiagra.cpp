// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AttachMeshNiagra.h"
#include "AnimNotifyState_AttachMeshNiagra.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void UAnimNotifyState_AttachMeshNiagra::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UAnimNotifyState::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	SpawnedNiagaraComponent = nullptr;
	AttachMesh = nullptr;

	if (!Template || !MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	UObject* PartObject = Owner->GetDefaultSubobjectByName(TEXT("WeaponMesh"));
	UStaticMeshComponent* StaticMeshComp = PartObject ? Cast<UStaticMeshComponent>(PartObject) : nullptr;
	if (!StaticMeshComp)
	{
		return;
	}

	AttachMesh = StaticMeshComp;

	const bool bHasValidSocket = SocketName.IsNone() || AttachMesh->DoesSocketExist(SocketName);
	if (!bHasValidSocket)
	{
		return;
	}

	SpawnedNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Template,
		AttachMesh,
		SocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		false,
		true
	);

	if (SpawnedNiagaraComponent)
	{
		SpawnedNiagaraComponent->SetAutoDestroy(false);
	}
}

void UAnimNotifyState_AttachMeshNiagra::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAnimNotifyState::NotifyEnd(MeshComp, Animation, EventReference);

	if (SpawnedNiagaraComponent)
	{
		SpawnedNiagaraComponent->Deactivate();
		SpawnedNiagaraComponent->DestroyComponent();
		SpawnedNiagaraComponent = nullptr;
	}

	AttachMesh = nullptr;
}
