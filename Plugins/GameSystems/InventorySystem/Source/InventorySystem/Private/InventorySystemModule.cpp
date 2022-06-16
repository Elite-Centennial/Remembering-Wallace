// Elite Centennial

#include "Modules/ModuleManager.h"

class FInventorySystemModule : public IModuleInterface
{
public:
	// BEGIN IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// END IModuleInterface interface
};

#define LOCTEXT_NAMESPACE "FInventorySystemModule"

void FInventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory.
	// The exact timing is specified in the .uplugin file per-module.
}

void FInventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInventorySystemModule, InventorySystem);
