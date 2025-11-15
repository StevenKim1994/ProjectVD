#include "System/VDLevelSystem.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Public/VDConstrants.h"


void UVDLevelSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UVDLevelSystem::Deinitialize()
{
    Super::Deinitialize();
}

void UVDLevelSystem::LoadPrepareNextLevelAssets()
{
    // TODO :: NextLevel에서 사용되는 에셋리스트를 모아둔 DataAsset에서 필요한 정보를 ResourceSystem에 StreamableManager로 로드하기 
}

void UVDLevelSystem::LoadChangeLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
    CurrentLevelName = NextLevelName;
    // TODO :: LoadPrepareNextLevelAssets가 끝나면 OpenLevel(NextLevelName);
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

    if (LevelLoadedCompleteDelegate.IsBound())
    {
        LevelLoadedCompleteDelegate.Execute();
    }
}
