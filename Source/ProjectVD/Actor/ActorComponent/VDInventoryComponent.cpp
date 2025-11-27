// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorComponent/VDInventoryComponent.h"
#include "System/VDInventorySubSystem.h"
#include "System/VDDataTableSubSystem.h"
#include "Actor/EquipItem/VDEquipItemVisualActor.h"
#include "Actor/ItemProp/VDItemPropActorBase.h"
#include "Object/VDInventoryInfo.h"
#include "DataTable/VDItemInfoTable.h"

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
	UGameInstance* GameInstance = GetOwner()->GetGameInstance();
	UVDInventorySubSystem* InventorySubSystem = GameInstance->GetSubsystem<UVDInventorySubSystem>();
	UVDDataTableSubSystem* DataTableSubSystem = GameInstance->GetSubsystem<UVDDataTableSubSystem>();

	FName InfoName = Item->GetItemInfoName();
	FVDItemInfoTable* Table = DataTableSubSystem->GetDataTableRow<FVDItemInfoTable>(FName(TEXT("ItemInfo")),Item->GetItemInfoName());
	
	if(Table) 
	{ 
		UVDInventoryInfo* NewItemInfo = NewObject<UVDInventoryInfo>();
		NewItemInfo->SetQuantity(1);
		NewItemInfo->SetItemID(Item->GetItemInfoName());
		NewItemInfo->SetItemType(Table->ItemType);
		NewItemInfo->SetMaxQuantity(Table->MaxQuantity);
		NewItemInfo->SetIsEmpty(false);

		InventorySubSystem->AddInventoryItem(NewItemInfo);

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UVDInventoryComponent::AddItemToInventory() Failed to find item info: %s"), *InfoName.ToString());
		return false;
	}
}

