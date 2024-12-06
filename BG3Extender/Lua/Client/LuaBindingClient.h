#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <Lua/Client/ClientEvents.h>
#include <Lua/Client/ClientEntityReplicationEvents.h>
#if defined(ENABLE_UI)
#include <Lua/Client/UIEvents.h>
#endif

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

LifetimeHandle GetClientLifetime();
LifetimePool& GetClientLifetimePool();

class ExtensionLibraryClient : public ExtensionLibrary
{
public:
    void Register(lua_State * L) override;
    void RegisterLib(lua_State * L) override;
};


class ClientState : public State
{
public:
    static ClientState* FromLua(lua_State* L);

    ClientState(ExtensionState& state, uint32_t generationId);
    ~ClientState();

    void Initialize() override;
    bool IsClient() override;
    void OnUpdate(GameTime const& time) override;

    ecs::EntityWorld* GetEntityWorld() override;
    ecs::EntitySystemHelpersBase* GetEntitySystemHelpers() override;
    EntityReplicationEventHooks* GetReplicationEventHooks() override;

    inline ClientEntityReplicationEventHooks& GetClientReplicationEventHooks()
    {
        return replicationHooks_;
    }

    void OnGameStateChanged(GameState fromState, GameState toState);
    bool IsEventCancelable(SDL_Event* event);
    void OnInputEvent(SDL_Event* event, int* result);

#if defined(ENABLE_UI)
    UIEventHooks& GetUIEvents()
    {
        return uiEvents_;
    }
#endif

#if defined(ENABLE_IMGUI)
    extui::IMGUIObjectManager& IMGUI();
#endif

private:
    ExtensionLibraryClient library_;
    ClientEntityReplicationEventHooks replicationHooks_;
#if defined(ENABLE_UI)
    UIEventHooks uiEvents_;
#endif
#if defined(ENABLE_IMGUI)
    extui::IMGUIObjectManager* imgui_{ nullptr };
#endif
};

END_NS()
