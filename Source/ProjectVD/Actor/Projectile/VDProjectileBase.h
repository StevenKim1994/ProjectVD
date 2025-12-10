// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDProjectileBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class PROJECTVD_API AVDProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AVDProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Components")
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
