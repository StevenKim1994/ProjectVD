#include "System/VDLevelSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Public/VDConstrants.h"

void UVDLevelSystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVDLevelSystem::Deinitialize()
{

}

void UVDLevelSystem::ChangeLevelByName(const FString& LevelName)
{
	if (CurrentLevelName.Equals(LevelName))
    {
        UE_LOG(LogTemp, Warning, TEXT("VDLevelSystem::ChangeLevelByName - Already in level '%s'."), *LevelName);
        return;
    }

    if (LevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("VDLevelSystem::ChangeLevelByName - LevelName is empty."));
        return;
    }

	NextLevelName = LevelName;
    UGameplayStatics::OpenLevel(GetWorld(), VDConstants::LoadingLevel);
}

void UVDLevelSystem::OnLevelLoaded()
{
    CurrentLevelName = NextLevelName;
    NextLevelName.Empty();
}
