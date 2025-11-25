// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDInventoryComponent.h"
#include "System/VDInventorySubSystem.h"

// Sets default values for this component's properties
UVDInventoryComponent::UVDInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVDInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UVDInventoryComponent::IsInventoryFull() const
{
	UVDInventorySubSystem* InventorySubSystem =	GetOwner()->GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();

	if (InventorySubSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UVDInventoryComponent::IsInventoryFull() InventorySubSystem is nullptr"));
		return false;
	}

	bool bResult = InventorySubSystem->IsInventoryFull();

	return bResult;
}

bool UVDInventoryComponent::AddItemToInventory(AVDItemPropActorBase* Item)
{
	UVDInventorySubSystem* InventorySubSystem = GetOwner()->GetGameInstance()->GetSubsystem<UVDInventorySubSystem>();

	if (InventorySubSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UVDInventoryComponent::IsInventoryFull() InventorySubSystem is nullptr"));
		return false;
	}

	// TODO :: 인벤토리 추가할 수 있는 아이템은 추가하고 결과를 return 하도록 수정 필요
	return true;
}

