#if WITH_EDITOR
#include "EditorAssetManagerModule.h"
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Modules/ModuleManager.h"

void FEditorAssetManagerModule::StartupModule()
{
	//FEditorDelegates::OnEditorInitialized.AddRaw(this, &FEditorAssetManagerModule::OnEditorInitialized);
}

void FEditorAssetManagerModule::ShutdownModule()
{
	FEditorDelegates::OnEditorInitialized.RemoveAll(this);
}

void FEditorAssetManagerModule::OnEditorInitialized()
{
	if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		AssetManager->RefreshPrimaryAssetDirectory(true);

		UE_LOG(LogTemp, Log, TEXT("EditorAssetManagerModule: Refreshed Primary Asset Directory on editor initialization."));
	}
}

//IMPLEMENT_MODULE(FEditorAssetManagerModule, EditorAssetManager)
#endif