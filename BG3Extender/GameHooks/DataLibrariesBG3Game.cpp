#include "stdafx.h"
#include "resource.h"
#include <GameHooks/DataLibraries.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Symbols.h>
#include <functional>
#include <psapi.h>
#include <Extender/Shared/tinyxml2.h>

BEGIN_NS(ecs)

STDString SimplifyComponentName(StringView name);

END_NS()

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
            DX11 = false;
            mapper.AddModule("Main", L"bg3.exe");
        }
        else
        {
            DX11 = true;
            mapper.AddModule("Main", L"bg3_dx11.exe");
        }

        mapper.AddEngineCallback("BindECSContext", std::bind(&LibraryManager::BindECSContext, this, std::placeholders::_1));
        mapper.AddEngineCallback("BindECSIndex", std::bind(&LibraryManager::BindECSIndex, this, std::placeholders::_1));
        mapper.AddEngineCallback("BindECSStaticStringConstructor", std::bind(&LibraryManager::BindECSStaticStringConstructor, this, std::placeholders::_1));
        mapper.AddEngineCallback("BindECSStaticRegistrant", std::bind(&LibraryManager::BindECSStaticRegistrant, this, std::placeholders::_1));
        mapper.AddEngineCallback("BindComponentReplicationIDRef", std::bind(&LibraryManager::BindComponentReplicationIDRef, this, std::placeholders::_1));
    }

    SymbolMapper::MappingResult LibraryManager::BindECSContext(uint8_t const* ptr)
    {
        auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr + 7);
        auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 0x63);
        GetStaticSymbols().IndexSymbolToContextMaps.insert(std::make_pair(indexPtr, namePtr));
        return SymbolMapper::MappingResult::TryNext;
    }

    SymbolMapper::MappingResult LibraryManager::BindECSIndex(uint8_t const* ptr)
    {
        auto contextPtr = (int32_t*)AsmResolveInstructionRef(ptr);
        auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr + 0x1C);
        auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 0x46);
        GetStaticSymbols().IndexSymbolToNameMaps.insert(std::make_pair(indexPtr, ecs::IndexSymbolInfo{ namePtr, contextPtr }));
        return SymbolMapper::MappingResult::TryNext;
    }

    SymbolMapper::MappingResult LibraryManager::BindECSStaticStringConstructor(uint8_t const* ptr)
    {
        auto funcPtr = ptr - 0x65;
        auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 9);
        GetStaticSymbols().StaticStringRegistrantMaps.insert(std::make_pair(funcPtr, namePtr));
        return SymbolMapper::MappingResult::TryNext;
    }

    SymbolMapper::MappingResult LibraryManager::BindECSStaticRegistrant(uint8_t const* ptr)
    {
        auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr);
        auto funcPtr = AsmResolveInstructionRef(ptr + 0x13);
        auto contextPtr = (int32_t*)AsmResolveInstructionRef(ptr + 0x1B);

        auto nameIt = GetStaticSymbols().StaticStringRegistrantMaps.find(funcPtr);
        if (nameIt != GetStaticSymbols().StaticStringRegistrantMaps.end()) {
            GetStaticSymbols().IndexSymbolToNameMaps.insert(std::make_pair(indexPtr, ecs::IndexSymbolInfo{ nameIt->second, contextPtr }));
        }

        return SymbolMapper::MappingResult::TryNext;
    }

    SymbolMapper::MappingResult LibraryManager::BindComponentReplicationIDRef(uint8_t const* ptr)
    {
        auto contextPtr = (int32_t*)AsmResolveInstructionRef(ptr);
        auto indexPtr = (int32_t*)AsmResolveInstructionRef(ptr + 0x7);
        auto namePtr = (char const*)AsmResolveInstructionRef(ptr + 0x31);
        GetStaticSymbols().IndexSymbolToNameMaps.insert(std::make_pair(indexPtr, ecs::IndexSymbolInfo{ namePtr, contextPtr }));
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
        SYM_OFF(ls__GlobalStringTable__MainTable__CreateFromString);
        SYM_OFF(ls__FixedString__GetString);
        SYM_OFF(ls__FixedString__IncRef);
        SYM_OFF(ls__GlobalStringTable__MainTable__DecRef);
        SYM_OFF(ls__gGlobalStringTable);

        SYM_OFF(ls__ThreadRegistry__RequestThreadIndex);
        SYM_OFF(ls__FileReader__ctor);
        SYM_OFF(ls__FileReader__dtor);
        SYM_OFF(ls__PathRoots);
        SYM_OFF(App__Ctor);
        SYM_OFF(App__UpdatePaths);

        SYM_OFF(ecl__EoCClient);
        SYM_OFF(esv__EoCServer);
        SYM_OFF(ecl__EoCClient__HandleError);
        SYM_OFF(ecl__gCursorControl);
        SYM_OFF(ls__gDragDropManager);
        SYM_OFF(ls__gTranslatedStringRepository);

        SYM_OFF(ecl__gGameStateEventManager);
        SYM_OFF(esv__gGameStateEventManager);
        SYM_OFF(ecl__GameStateThreaded__GameStateWorker__DoWork);
        SYM_OFF(esv__GameStateThreaded__GameStateWorker__DoWork);
        SYM_OFF(ecl__GameStateMachine__Update);
        SYM_OFF(esv__GameStateMachine__Update);
        SYM_OFF(App__LoadGraphicSettings);
        SYM_OFF(ls__gInputManager);

        SYM_OFF(ecs__EntityWorld__Update);
        SYM_OFF(ecs__EntityWorld__FlushECBs);

        /*SYM_OFF(esv__SurfaceActionFactory);
        SYM_OFF(esv__SurfaceActionFactory__CreateAction);
        SYM_OFF(esv__SurfaceManager__AddAction);*/

        SYM_OFF(eoc__SpellPrototypeManager);
        SYM_OFF(eoc__SpellPrototype__Init);

        SYM_OFF(eoc__BoostPrototypeManager);

        SYM_OFF(eoc__StatusPrototypeManager);
        SYM_OFF(eoc__StatusPrototype__Init);

        SYM_OFF(eoc__PassivePrototypeManager);
        SYM_OFF(eoc__PassivePrototype__Init);

        SYM_OFF(eoc__InterruptPrototypeManager);
        SYM_OFF(eoc__InterruptPrototype__Init);

        SYM_OFF(eoc__ParseStaticBoosts);

        SYM_OFF(esv__OsirisVariableHelper__SavegameVisit);

        SYM_OFF(esv__StatusMachine__CreateStatus);
        SYM_OFF(esv__StatusMachine__ApplyStatus);

        SYM_OFF(stats__DealDamageFunctor__ApplyDamage);
        SYM_OFF(esv__StatsSystem__ThrowDamageEvent);

        SYM_OFF(esv__ExecuteStatsFunctor_AttackTargetContext);
        SYM_OFF(esv__ExecuteStatsFunctor_AttackPositionContext);
        SYM_OFF(esv__ExecuteStatsFunctor_MoveContext);
        SYM_OFF(esv__ExecuteStatsFunctor_TargetContext);
        SYM_OFF(esv__ExecuteStatsFunctor_NearbyAttackedContext);
        SYM_OFF(esv__ExecuteStatsFunctor_NearbyAttackingContext);
        SYM_OFF(esv__ExecuteStatsFunctor_EquipContext);
        SYM_OFF(esv__ExecuteStatsFunctor_SourceContext);
        SYM_OFF(esv__ExecuteStatsFunctor_InterruptContext);

        SYM_OFF(gRPGStats);
        SYM_OFF(RPGStats__Load);
        SYM_OFF(RPGStats__PreParseDataFolder);
        SYM_OFF(stats__Object__SetPropertyString);

        SYM_OFF(esv__LevelManager);
        SYM_OFF(ecl__LevelManager);
        SYM_OFF(ls__GlobalTemplateManager);
        SYM_OFF(esv__CacheTemplateManager);

        SYM_OFF(eoc__AiGrid__FindPath);
        SYM_OFF(eoc__AiGrid__FindPathImmediate);

        SYM_OFF(esv__SavegameManager);

        SYM_OFF(Noesis__Reflection__LockedByThreadId);
        SYM_OFF(Noesis__Reflection__Lock);
        SYM_OFF(Noesis__Reflection__NumLocks);
        SYM_OFF(Noesis__Reflection__Data);

        SYM_OFF(Noesis__SymbolManager__Data);
        SYM_OFF(Noesis__SymbolManager__Lock);

        SYM_OFF(Noesis__GUI__LoadXaml);
        SYM_OFF(Noesis__Visual__AddVisualChild);
        SYM_OFF(Noesis__Visual__RemoveVisualChild);
        SYM_OFF(ls__UIStateMachine__FireStateEvent2);

        SYM_OFF(AppInstance);

        SYM_OFF(ls__gGlobalAllocator);
        SYM_OFF(ls__GlobalAllocator__Get);
        SYM_OFF(ls__GlobalAllocator__Alloc);
        SYM_OFF(ls__GlobalAllocator__Free);

        SYM_OFF(eoc__gGuidResourceManager);
        SYM_OFF(ls__gGlobalResourceManager);

        SYM_OFF(ls__gTextureAtlasMap);
        SYM_OFF(ls__AppliedMaterial__LoadTexture);
        SYM_OFF(ls__TextureManager__UnloadTexture);

        SYM_OFF(ls__Visual__UpdateBlendshapeWeightsFromSkeleton);

        SYM_OFF(ls__GlobalSwitches);

        SYM_OFF(Kernel_FindFirstFileW);
        SYM_OFF(Kernel_FindNextFileW);
        SYM_OFF(Kernel_FindClose);
    }
}
