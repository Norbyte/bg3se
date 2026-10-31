#include <stdafx.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Client/LuaBindingClient.h>
#include <Lua/Shared/LuaModule.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/ExtensionStateClient.h>
#include "resource.h"
#include <Lua/Client/UIEvents.inl>
#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Objects.h>
#include <SDL_events.h>

#include <Lua/Client/ClientEntityReplicationEvents.inl>

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

LifetimeHandle GetClientLifetime()
{
    se_assert(gExtender->GetClient().IsInContext());
    return ExtensionState::Get().GetLua()->GetCurrentLifetime();
}

LifetimePool& GetClientLifetimePool()
{
    se_assert(gExtender->GetClient().IsInContext());
    return ExtensionState::Get().GetLua()->GetLifetimePool();
}

void ExtensionLibraryClient::Register(lua_State * L)
{
    ExtensionLibrary::Register(L);
}

void ExtensionLibraryClient::RegisterLib(lua_State * L)
{
    static const luaL_Reg extLib[] = {
        {0,0}
    };

    luaL_newlib(L, extLib); // stack: lib
    lua_setglobal(L, "Ext"); // stack: -

    RegisterSharedMetatables(L);
    gModuleRegistry.ConstructState(L, ModuleRole::Client);
}


ClientState* ClientState::FromLua(lua_State* L)
{
    se_assert(GetCurrentContextType() == ContextType::Client);
    return static_cast<ClientState*>(State::FromLua(L));
}

ClientState::ClientState(ExtensionState& state, uint32_t generationId)
    : State(state, generationId, false),
    replicationHooks_(*this),
    uiEvents_(*this),
    deferredUIEvents_(*this)
{}

ClientState::~ClientState()
{
    auto & sym = GetStaticSymbols();

    ecl::lua::ui::ReleasePropertyChangeHandlers(L);

    if (imgui_) {
        gExtender->IMGUI().SetObjects(nullptr);
        delete imgui_;
    }

    if (gExtender) {
        auto debugger = gExtender->GetLuaDebugger();
        if (debugger) {
            debugger->ClientStateDeleted();
        }
    }
}

void ClientState::Initialize()
{
    State::Initialize();

    StackCheck _(L, 0);
    library_.Register(L);

    gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("ClientStartup.lua");
    // Ext is not writeable after loading SandboxStartup!
    gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("SandboxStartup.lua");

    auto debugger = gExtender->GetLuaDebugger();
    if (debugger) {
        debugger->ClientStateCreated(this);
    }
}

bool ClientState::IsClient()
{
    return true;
}

void ClientState::OnUpdate(GameTime const& time)
{
    State::OnUpdate(time);
    replicationHooks_.PostUpdate();
    deferredUIEvents_.PostUpdate();
}


ecs::EntityWorld* ClientState::GetEntityWorld()
{
    return gExtender->GetClient().GetEntityHelpers().GetEntityWorld();
}


ecs::EntitySystemHelpersBase* ClientState::GetEntitySystemHelpers()
{
    return &gExtender->GetClient().GetEntityHelpers();
}

EntityReplicationEventHooks* ClientState::GetReplicationEventHooks()
{
    return &replicationHooks_;
}

extui::IMGUIObjectManager& ClientState::IMGUI()
{
    if (imgui_ == nullptr) {
        imgui_ = new extui::IMGUIObjectManager();
        gExtender->IMGUI().SetObjects(imgui_);
    }

    return *imgui_;
}

END_NS()
