#pragma once

#include <Osiris/Shared/CustomFunctions.h>
#include <Extender/Shared/ExtensionState.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

class Hooks
{
public:
	void Startup();

	void OnParseStructureFolder(stats::RPGStats* self, Array<STDString>* paths);

private:
	bool loaded_{ false };
};

END_SE()
