// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDHitStopComponent.h"

UVDHitStopComponent::UVDHitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVDHitStopComponent::SetHitStop( float TimeDilation, float InHitStopTime)
{
	AActor* OwnerActor = GetOwner();
    if (OwnerActor == nullptr)
    {
        return;
    }

    if (OwnerActor->CustomTimeDilation != 1.0f)
    {
        return;
    }

    OwnerActor->CustomTimeDilation = TimeDilation; 

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            HitStopTimerHandle,
            this,
            &UVDHitStopComponent::EndHitStop,
            InHitStopTime,
            false
        );
    }
}

void UVDHitStopComponent::StopHitStop()
{
}

void UVDHitStopComponent::EndHitStop()
{
	AActor* Owner = GetOwner();
	if(Owner)
	{
		Owner->CustomTimeDilation = 1.0f;
	}
}
