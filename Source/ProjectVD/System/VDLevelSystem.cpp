#include "System/VDLevelSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UVDLevelSystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVDLevelSystem::Deinitialize()
{

}

void UVDLevelSystem::ChangeLevelByName(const FString& LevelName)
{
    if (LevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("VDLevelSystem::ChangeLevelByName - LevelName is empty."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("VDLevelSystem::ChangeLevelByName - World is null."));
        return;
    }

    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, /*bRemovePrefix=*/true);
    if (CurrentLevelName.Equals(LevelName, ESearchCase::IgnoreCase))
    {
        UE_LOG(LogTemp, Verbose, TEXT("VDLevelSystem::ChangeLevelByName - Already in level '%s'."), *LevelName);
        return;
    }

    const FName TargetLevelFName(*LevelName);
    UGameplayStatics::OpenLevel(World, TargetLevelFName);
}
