#include "stdafx.h"
#include <GameHooks/EngineHooks.h>
#include <GameDefinitions/Symbols.h>

namespace bg3se
{

#define HOOK_DEFN(name, sym, defn) decltype(EngineHooks::name) * decltype(EngineHooks::name)::gHook;
#include <GameHooks/EngineHooks.inl>
#undef HOOK_DEFN

void EngineHooks::HookAll()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    auto& sym = GetStaticSymbols();

    /*if (sym.AppInstance != nullptr && *sym.AppInstance != nullptr) {
        sym.App__OnInputEvent = (*sym.AppInstance)->__vftable->OnInputEvent;
    }*/

#define HOOK_DEFN(name, sym, defn) if (GetStaticSymbols().sym != nullptr) { name.Wrap(GetStaticSymbols().sym); }
#include <GameHooks/EngineHooks.inl>
#undef HOOK_DEFN

    //sym.CharStatsGetters.WrapAll();

    DetourTransactionCommit();
}

void EngineHooks::UnhookAll()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

#define HOOK_DEFN(name, sym, defn) name.Unwrap();
#include <GameHooks/EngineHooks.inl>
#undef HOOK_DEFN

    DetourTransactionCommit();
}

}
