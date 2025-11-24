// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDEquipItemVisualActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class PROJECTVD_API AVDEquipItemVisualActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="EquipItem", Meta = (AllowPrivateAccess ="true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipItem", Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AVDEquipItemVisualActor();
	
	//void SetEquipInfo(); // TODO :: 해당아이템의 정보처리

	void SetColider(bool bEnable);
};