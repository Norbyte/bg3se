#include "stdafx.h"
#include "resource.h"
#include <GameHooks/DataLibraries.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Symbols.h>
#include <functional>
#include <psapi.h>
#include <Extender/Shared/tinyxml2.h>

namespace bg3se
{
	void LibraryManager::PreRegisterLibraries(SymbolMappingLoader & loader)
	{
		loader.AddKnownModule("Main");
	}

	void LibraryManager::RegisterLibraries(SymbolMapper & mapper)
	{
		if (GetModuleHandleW(L"bg3.exe") != NULL)
		{
			mapper.AddModule("Main", L"bg3.exe");
		}
		else
		{
			mapper.AddModule("Main", L"bg3_dx11.exe");
		}

		// mapper.AddEngineCallback("FindLibraries", &FindLibrariesEoCApp);
	}

	bool LibraryManager::BindApp()
	{
		if (GetModuleHandleW(L"bg3.exe") != NULL)
		{
			symbolMapper_.AddModule("Main", L"bg3.exe");
		}
		else if (GetModuleHandleW(L"bg3_dx11.exe") != NULL)
		{
			symbolMapper_.AddModule("Main", L"bg3_dx11.exe");
		}
		else
		{
			return false;
		}

		appModule_ = symbolMapper_.Modules().find("Main")->second;
		return true;
	}

	HMODULE LibraryManager::GetAppHandle()
	{
		HMODULE hApp = GetModuleHandleW(L"bg3.exe");
		if (hApp == NULL) {
			hApp = GetModuleHandleW(L"bg3_dx11.exe");
		}

		return hApp;
	}


#define SYM_OFF(name) mappings_.StaticSymbols.insert(std::make_pair(#name, SymbolMappings::StaticSymbol{ (int)offsetof(StaticSymbols, name) }))

	void LibraryManager::RegisterSymbols()
	{
		SYM_OFF(ls__FixedString__CreateFromString);
		SYM_OFF(ls__FixedString__GetString);
		SYM_OFF(ls__FixedString__IncRef);
		SYM_OFF(ls__FixedString__DecRef);
		SYM_OFF(ls__gGlobalStringTable);

		SYM_OFF(ls__FileReader__ctor);
		SYM_OFF(ls__FileReader__dtor);
		SYM_OFF(ls__PathRoots);

		SYM_OFF(ecl__EoCClient);
		SYM_OFF(esv__EoCServer);
		SYM_OFF(ecl__EoCClient__HandleError);
		SYM_OFF(ls__gTranslatedStringRepository);

		SYM_OFF(ecl__gGameStateEventManager);
		SYM_OFF(ecl__GameStateEventManager__ExecuteGameStateChangedEvent);
		SYM_OFF(ecl__GameStateThreaded__GameStateWorker__DoWork);
		SYM_OFF(esv__GameStateThreaded__GameStateWorker__DoWork);
		SYM_OFF(ecl__GameStateMachine__Update);
		SYM_OFF(esv__GameStateMachine__Update);

		SYM_OFF(esv__SurfaceActionFactory);
		SYM_OFF(esv__SurfaceActionFactory__CreateAction);
		SYM_OFF(esv__SurfaceManager__AddAction);

		SYM_OFF(eoc__SpellPrototypeManager);
		SYM_OFF(eoc__SpellPrototype__Init);

		SYM_OFF(eoc__StatusPrototypeManager);
		SYM_OFF(eoc__StatusPrototype__Init);

		SYM_OFF(eoc__PassiveManager);
		SYM_OFF(eoc__Passive__Init);

		SYM_OFF(esv__StatusMachine__CreateStatus);
		SYM_OFF(esv__StatusMachine__ApplyStatus);

		SYM_OFF(eoc__DealDamageFunctor__ApplyDamage);

		SYM_OFF(eoc__StatsFunctorSet__ExecuteType1);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType2);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType3);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType4);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType5);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType6);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType7);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType8);

		SYM_OFF(Stats);
		SYM_OFF(esv__SavegameManager);
		SYM_OFF(AppInstance);

		SYM_OFF(ls__GlobalAllocator__Alloc);
		SYM_OFF(ls__GlobalAllocator__Free);

		SYM_OFF(ResourceDefns);
		SYM_OFF(ResourceMgr);
		SYM_OFF(GlobalSwitches);
	}

	void LibraryManager::FindSymbolNameRegistrations()
	{
		static uint8_t const sig1[] = {
			0x48, 0x8B, 0x0C, 0xC8,
			0x8B, 0x04, 0x0A,
			0x39, 0x05
		};

		static uint8_t const sig2[] = {
			0xC7, 0x44, 0x24, 0x28
		};

		static uint8_t const sig3[] = {
			0x48, 0x89, 0x44, 0x24, 0x20
		};

		uint8_t const * p = (uint8_t const *)appModule_.ModuleTextStart;
		uint8_t const * moduleEnd = p + appModule_.ModuleTextSize;

		auto& maps = GetStaticSymbols().SymbolIdToNameMaps;
		for (; p < moduleEnd - 100; p++) {
			if (*(uint64_t*)p == *(uint64_t*)&sig1[0]
				&& *(uint32_t*)(p + 22) == *(uint32_t*)&sig2[0]
				&& *(uint32_t*)(p + 44) == *(uint32_t*)&sig3[0]
				&& memcmp(p, sig1, sizeof(sig1)) == 0) {

				int32_t rel = *(int32_t*)(p + 23 - 5);
				uint8_t const* registrantObj = p + rel + 20 + 7 - 5;

				int32_t relName = *(int32_t*)(p + 45 - 5);
				uint8_t const* relNamePtr = p + relName + 42 + 7 - 5;

				maps.insert(std::make_pair((int32_t*)registrantObj, (char*)relNamePtr));
			}
		}
	}
}
