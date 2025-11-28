#include "System/VDLevelSystem.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "Engine/PrimaryAssetLabel.h"
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
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType LabelType = FName(TEXT("Stage"));
    TArray<FPrimaryAssetId> LabelIds;
	bool Result = AssetManager.GetPrimaryAssetIdList(LabelType, LabelIds);

    if (Result)
    {
        for (const FPrimaryAssetId& Id : LabelIds)
        {
            UE_LOG(LogTemp, Log, TEXT("VDLevelSystem::LoadChangeLevel - Found Primary Asset Id: %s"), *Id.ToString());
            UPrimaryAssetLabel* Label = Cast<UPrimaryAssetLabel>(AssetManager.GetPrimaryAssetObject(Id));
            if (Label)
            {

            }
        }
    }
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
    if (ProgressTimerHandle.IsValid())
    {
        ProgressTimerHandle.Invalidate();
	}

    LevelLoadedCompleteDelegate.ExecuteIfBound();

}

void UVDLevelSystem::ChangeToNextLevel()
{
    if (!NextLevelName.IsEmpty())
    {
        CurrentLevelName = NextLevelName;
        NextLevelName.Empty();
        UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));
    }
}
