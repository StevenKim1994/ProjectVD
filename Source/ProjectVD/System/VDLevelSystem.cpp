#include "System/VDLevelSystem.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/PrimaryAssetLabel.h"
#include "Public/VDConstrants.h"
#include "System/VDResourceSystem.h"


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
    FName LoadPALAssetName = FName(*FString::Printf(TEXT("PAL_%s"), *NextLevelName));
    const FPrimaryAssetId LevelAssetId(FName(TEXT("PrimaryAssetLabel")), LoadPALAssetName);
    if (!LevelPrepareAssetsMap.Contains(LevelAssetId))
    {
        UVDResourceSystem* ResourceSystem = GetGameInstance()->GetSubsystem<UVDResourceSystem>();
        UPrimaryAssetLabel* LoadMapInfo = ResourceSystem->GetLoadedPrimaryAsset<UPrimaryAssetLabel>(LevelAssetId);

        if(LoadMapInfo)
        {
            LevelPrepareAssetsMap.Add(LevelAssetId, LoadMapInfo);
		}
    }

    UPrimaryAssetLabel* PrepareLevelAsset = LevelPrepareAssetsMap[LevelAssetId].Get();
    if (PrepareLevelAsset)
    {
        UAssetManager& AssetManager = UAssetManager::Get();
		FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();

        TArray<FSoftObjectPath> AssetPaths;
        for (const TSoftObjectPtr<UObject> Ptr : PrepareLevelAsset->ExplicitAssets)
        {
            AssetPaths.Add(Ptr.ToSoftObjectPath());
        }
        for (const TSoftClassPtr<UObject> Ptr : PrepareLevelAsset->ExplicitBlueprints)
        {
            AssetPaths.Add(Ptr.ToSoftObjectPath());
        }
        CurrentCount = 0;
        AssetNum = AssetPaths.Num();
        FStreamableDelegate ProgressDelegate = FStreamableDelegate::CreateUObject(
            this,
            &UVDLevelSystem::OnLevelLoaded
		);
        for (const FSoftObjectPath& Path : AssetPaths)
        {
            UAssetManager::GetStreamableManager().RequestAsyncLoad(
                Path,
                ProgressDelegate
            );
        }
    }
    else
    {
        ChangeToNextLevel();
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

void UVDLevelSystem::OnLoadSingleAsset()
{
    CurrentCount++;

    UE_LOG(LogTemp, Warning, TEXT("VDLevelSystem::OnLoadSingleAsset Count : '%d' Max : '%d'"), CurrentCount, AssetNum);
}
