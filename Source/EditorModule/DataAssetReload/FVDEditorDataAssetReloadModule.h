#pragma once

#include "Modules/ModuleManager.h"

class FVDEditorDataAssetReloadModule : public IModuleInterface
{
	public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};