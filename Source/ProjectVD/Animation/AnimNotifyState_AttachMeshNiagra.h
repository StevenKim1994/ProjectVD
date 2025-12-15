// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttachMeshNiagra.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PROJECTVD_API UAnimNotifyState_AttachMeshNiagra : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> Template;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMeshComponent> AttachMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", Meta = (AllowPrivateAccess = true))
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", Meta = (AllowPrivateAccess = true))
	FVector Scale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachStaticMesh", Meta = (AllowPrivateAccess = true))
	FName SocketName;

	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> SpawnedNiagaraComponent;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
