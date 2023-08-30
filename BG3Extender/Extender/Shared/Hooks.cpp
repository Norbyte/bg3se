#include <stdafx.h>
#include <Extender/Shared/Hooks.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

void Hooks::Startup()
{
	if (loaded_) {
		return;
	}

	auto& lib = gExtender->GetEngineHooks();
	lib.RPGStats__ParseStructureFolder.SetPostHook(&Hooks::OnParseStructureFolder, this);
	
	loaded_ = true;
}

void Hooks::OnParseStructureFolder(stats::RPGStats* self, Array<STDString>* paths)
{
	LuaVirtualPin lua(gExtender->GetCurrentExtensionState());
	if (lua) {
		lua->OnStatsStructureLoaded();
	}
}

END_SE()
