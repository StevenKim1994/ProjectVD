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
    FName LabelName = FName(TEXT("PAL_Stage"));
	FPrimaryAssetId LevelLabelId = FPrimaryAssetId(LabelType, LabelName);
	TArray<FName> LoadBundles;
    LevelStreamableHandle = AssetManager.LoadPrimaryAsset(LevelLabelId, LoadBundles, FStreamableDelegate::CreateLambda([this]
    {
        OnLevelLoaded();
    }));


	if (LevelStreamableHandle.IsValid())
    {
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (ProgressTimerHandle.IsValid())
        {
			ProgressTimerHandle.Invalidate();
        }

        TimerManager.SetTimer(ProgressTimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                float Progress = LevelStreamableHandle->GetProgress();
                LevelLoadedDelegate.ExecuteIfBound(Progress);
				UE_LOG(LogTemp, Log, TEXT("VDLevelSystem::LoadChangeLevel - Loading Progress: %.2f"), Progress);
			}), 0.1f, false, true);
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
    CurrentLevelName = NextLevelName;
    NextLevelName.Empty();

    if (ProgressTimerHandle.IsValid())
    {
        ProgressTimerHandle.Invalidate();
	}

    LevelLoadedCompleteDelegate.ExecuteIfBound();

    UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));
}
