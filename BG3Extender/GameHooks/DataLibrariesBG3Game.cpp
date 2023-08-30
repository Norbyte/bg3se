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

		mapper.AddEngineCallback("BindECSIndex", std::bind(&LibraryManager::BindECSIndex, this, std::placeholders::_1));
		mapper.AddEngineCallback("BindECSStaticStringConstructor", std::bind(&LibraryManager::BindECSStaticStringConstructor, this, std::placeholders::_1));
		mapper.AddEngineCallback("BindECSStaticRegistrant", std::bind(&LibraryManager::BindECSStaticRegistrant, this, std::placeholders::_1));
		mapper.AddEngineCallback("BindComponentReplicationIDRef", std::bind(&LibraryManager::BindComponentReplicationIDRef, this, std::placeholders::_1));
		mapper.AddEngineCallback("BindComponentIDRef", std::bind(&LibraryManager::BindComponentIDRef, this, std::placeholders::_1));
		mapper.AddEngineCallback("BindComponentIDRef2", std::bind(&LibraryManager::BindComponentIDRef2, this, std::placeholders::_1));
	}

	SymbolMapper::MappingResult LibraryManager::BindECSIndex(uint8_t const* ptr)
	{
		auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr);
		auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 0x25);
		GetStaticSymbols().IndexSymbolToNameMaps.insert(std::make_pair(indexPtr, ecs::IndexSymbolInfo{ namePtr, ecs::IndexSymbolType::None }));
		return SymbolMapper::MappingResult::TryNext;
	}

	SymbolMapper::MappingResult LibraryManager::BindECSStaticStringConstructor(uint8_t const* ptr)
	{
		auto funcPtr = ptr - 0x3C;
		auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 30);
		GetStaticSymbols().StaticStringRegistrantMaps.insert(std::make_pair(funcPtr, namePtr));
		return SymbolMapper::MappingResult::TryNext;
	}

	SymbolMapper::MappingResult LibraryManager::BindECSStaticRegistrant(uint8_t const* ptr)
	{
		auto funcPtr = AsmResolveInstructionRef(ptr);
		auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr + 43);

		auto nameIt = GetStaticSymbols().StaticStringRegistrantMaps.find(funcPtr);
		if (nameIt != GetStaticSymbols().StaticStringRegistrantMaps.end()) {
			GetStaticSymbols().IndexSymbolToNameMaps.insert(std::make_pair(indexPtr, ecs::IndexSymbolInfo{ nameIt->second, ecs::IndexSymbolType::None }));
		}

		return SymbolMapper::MappingResult::TryNext;
	}

	SymbolMapper::MappingResult LibraryManager::BindComponentReplicationIDRef(uint8_t const* ptr)
	{
		auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr);

		auto indexIt = GetStaticSymbols().IndexSymbolToNameMaps.find(indexPtr);
		if (indexIt != GetStaticSymbols().IndexSymbolToNameMaps.end()) {
			indexIt->second.type = ecs::IndexSymbolType::Replication;
		}

		return SymbolMapper::MappingResult::TryNext;
	}

	SymbolMapper::MappingResult LibraryManager::BindComponentIDRef(uint8_t const* ptr)
	{
		auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr);

		auto indexIt = GetStaticSymbols().IndexSymbolToNameMaps.find(indexPtr);
		if (indexIt != GetStaticSymbols().IndexSymbolToNameMaps.end()) {
			indexIt->second.type = ecs::IndexSymbolType::Component;
		}

		return SymbolMapper::MappingResult::TryNext;
	}

	SymbolMapper::MappingResult LibraryManager::BindComponentIDRef2(uint8_t const* ptr)
	{
		auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr);

		auto indexIt = GetStaticSymbols().IndexSymbolToNameMaps.find(indexPtr);
		if (indexIt != GetStaticSymbols().IndexSymbolToNameMaps.end()) {
			indexIt->second.type = ecs::IndexSymbolType::Component;
		}

		return SymbolMapper::MappingResult::TryNext;
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
		SYM_OFF(CoreLibSDM__Init);
		SYM_OFF(App__UpdatePaths);

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

		/*SYM_OFF(esv__SurfaceActionFactory);
		SYM_OFF(esv__SurfaceActionFactory__CreateAction);
		SYM_OFF(esv__SurfaceManager__AddAction);*/

		SYM_OFF(eoc__SpellPrototypeManager);
		SYM_OFF(eoc__SpellPrototype__Init);

		SYM_OFF(eoc__StatusPrototypeManager);
		SYM_OFF(eoc__StatusPrototype__Init);

		SYM_OFF(eoc__PassiveManager);
		SYM_OFF(eoc__Passive__Init);

		SYM_OFF(esv__StatusMachine__CreateStatus);
		SYM_OFF(esv__StatusMachine__ApplyStatus);

		SYM_OFF(stats__DealDamageFunctor__ApplyDamage);

		SYM_OFF(stats__Functors__ExecuteType1);
		SYM_OFF(stats__Functors__ExecuteType2);
		SYM_OFF(stats__Functors__ExecuteType3);
		SYM_OFF(stats__Functors__ExecuteType4);
		SYM_OFF(stats__Functors__ExecuteType5);
		SYM_OFF(stats__Functors__ExecuteType6);
		SYM_OFF(stats__Functors__ExecuteType7);
		SYM_OFF(stats__Functors__ExecuteType8);

		SYM_OFF(gRPGStats);
		SYM_OFF(RPGStats__Load);
		SYM_OFF(RPGStats__ParseStructureFolder);

		SYM_OFF(esv__SavegameManager);
		SYM_OFF(AppInstance);

		SYM_OFF(ls__GlobalAllocator__Alloc);
		SYM_OFF(ls__GlobalAllocator__Free);

		SYM_OFF(ResourceDefns);
		SYM_OFF(ResourceMgr);
		SYM_OFF(ls__GlobalSwitches);
	}
}
