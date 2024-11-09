// - BY DreamMoon 2024 -
// https://dmstudio.top

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDreamToolsAsync, Log, All);

class FDreamToolsAsyncModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
