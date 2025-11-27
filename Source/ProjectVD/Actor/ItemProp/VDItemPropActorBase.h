// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/VDPickable.h"
#include "VDItemPropActorBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class AVDEquipItemVisualActor;

UCLASS(Abstract)
class PROJECTVD_API AVDItemPropActorBase : public AActor , public IVDPickable
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemProp", Meta = (AllowPrivateAccess = "true"))
	bool bIsHoverable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="ItemProp", Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> PickedEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "ItemProp" , Meta = (AllowPrivateAccess ="true"))
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemProp", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> ColiderComp;

	float HoverTime = 0.0f;
	float HoverSpeed = 1.0f;
	float HoverRange = 10.0f;
	FVector InitialLocation = FVector::ZeroVector;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemProp", Meta = (AllowPrivateAccess = "true" , RowType = "VDItemInfoTable" ))
	FDataTableRowHandle  ItemInfoTableRowName;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPickedEffectFinished(UNiagaraComponent* FinishedComponent);
public:
	AVDItemPropActorBase();

	FName GetItemInfoName();

	virtual void ResetItemProp();
	virtual void SetMeshHidden(bool bIsHidden);
	// IVDPickable을(를) 통해 상속됨
	bool CanBePicked() override;

	void OnPicked(AActor* Picker) override;

};
