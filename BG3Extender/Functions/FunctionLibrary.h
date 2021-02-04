#pragma once

#include "../CustomFunctions.h"
#include "../ExtensionState.h"
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Misc.h>

namespace bg3se
{
	class OsirisProxy;
	struct UIObjectManager;
}

namespace bg3se::esv
{
	class CustomFunctionLibrary
	{
	public:
		CustomFunctionLibrary(OsirisProxy & osiris);

		void Register();
		void RegisterHelperFunctions();
		void RegisterMathFunctions();
		void RegisterStatFunctions();
		void RegisterLuaFunctions();

		void PostStartup();
		void OnBaseModuleLoadedServer();
		void OnBaseModuleLoadedClient();

	private:
		OsirisProxy & osiris_;
		bool PostLoaded{ false };
	};

}