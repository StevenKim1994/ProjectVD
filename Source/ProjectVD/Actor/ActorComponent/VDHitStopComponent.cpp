// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDHitStopComponent.h"

UVDHitStopComponent::UVDHitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVDHitStopComponent::SetHitStop( float TimeDilation, float InHitStopTime)
{
    // 이미 히트스톱 중이면 중복 적용 방지 (선택사항)
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
