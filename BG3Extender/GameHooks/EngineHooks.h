#pragma once

#include <CoreLib/Wrappers.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Ai.h>

namespace bg3se
{
    class EngineHooks
    {
    public:
#define HOOK_DEFN(name, sym, defn) enum class name##Tag {}; \
    WrappableFunction<name##Tag, defn> name;
#include <GameHooks/EngineHooks.inl>
#undef HOOK_DEFN

        void HookAll();
        void UnhookAll();
    };
}
