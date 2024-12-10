#include <stdafx.h>
#include <Lua/Osiris/Binding.h>
#include <Extender/ScriptExtender.h>

#include <Lua/Osiris/ValueHelpers.inl>
#include <Lua/Osiris/Function.inl>
#include <Lua/Osiris/CustomFunction.inl>
#include <Lua/Osiris/NameCache.inl>
#include <Lua/Osiris/LuaNameResolver.inl>
#include <Lua/Osiris/FunctionProxy.inl>
#include <Lua/Osiris/CallbackManager.inl>

BEGIN_NS(esv::lua)

OsirisBinding::OsirisBinding(ExtensionState& state)
    : identityAdapters_(gExtender->GetServer().Osiris().GetGlobals()),
    resolver_(*this, gExtender->GetServer().Osiris().GetGlobals()),
    osirisCallbacks_(state)
{
    identityAdapters_.UpdateAdapters();
    resolver_.InvalidateCache();
}

OsirisBinding::~OsirisBinding()
{
    if (gExtender) {
        // FIXME - HANDLE IN SERVER LOGIC!
        gExtender->GetServer().Osiris().GetCustomFunctionManager().ClearDynamicEntries();
    }
}

void OsirisBinding::StoryLoaded()
{
    generationId_++;
    identityAdapters_.UpdateAdapters();
    if (!identityAdapters_.HasAllAdapters()) {
        OsiWarn("Not all identity adapters are available - some queries may not work!");
    }

    resolver_.InvalidateCache();
    osirisCallbacks_.StoryLoaded();
}

void OsirisBinding::StorySetMerging(bool isMerging)
{
    osirisCallbacks_.StorySetMerging(isMerging);
}

}
