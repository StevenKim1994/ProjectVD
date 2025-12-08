// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDHitStopComponent.h"

UVDHitStopComponent::UVDHitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVDHitStopComponent::SetHitStop(float InHitStopTime, float TimeDilation)
{
	if (HitStopTimerHandle.IsValid())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(HitStopTimerHandle);
	}

	if (AActor* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = TimeDilation;
		FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UVDHitStopComponent::EndHitStop);
		Owner->GetWorldTimerManager().SetTimer(HitStopTimerHandle,TimerDel, InHitStopTime, false);
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
