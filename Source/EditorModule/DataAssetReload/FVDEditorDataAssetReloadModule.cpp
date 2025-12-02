#include "FVDEditorDataAssetReloadModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"
#include "AssetRegistry/AssetData.h"
#include "AssetToolsModule.h"
#include "Engine/DataAsset.h"
#include "Modules/ModuleManager.h"
#include "Editor.h" 

void FVDEditorDataAssetReloadModule::StartupModule()
{
#if WITH_EDITOR
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    const FName AssetFolderPath = FName(TEXT("/Game/ProjectVD/DataAsset"));

    FARFilter Filter;
    Filter.PackagePaths.Add(AssetFolderPath);
    Filter.ClassNames.Add(UDataAsset::StaticClass()->GetFName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    IAssetTools& AssetTools = AssetToolsModule.Get();

    for (const FAssetData& AssetData : AssetDataList)
    {
        if (UObject* AssetObj = AssetData.GetAsset())
        {
            if (AssetObj->IsA<UDataAsset>())
            {
                UPackage* Package = AssetObj->GetOutermost();
                ResetLoaders(Package);
                AssetObj = StaticLoadObject(AssetObj->GetClass(), nullptr, *AssetData.GetSoftObjectPath().ToString());
                UE_LOG(LogTemp, Log, TEXT("Reloaded DataAsset: %s"), *AssetObj->GetName());
            }
        }
    }
#endif
}

void FVDEditorDataAssetReloadModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FVDEditorDataAssetReloadModule, EditorModule)
