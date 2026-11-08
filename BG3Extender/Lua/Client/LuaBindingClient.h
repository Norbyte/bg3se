#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <Lua/Client/CameraEvents.h>
#include <Lua/Client/ClientEvents.h>
#include <Lua/Client/ClientEntityReplicationEvents.h>
#include <Lua/Client/UIEvents.h>

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

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

    EntityReplicationEventHooks* GetReplicationEventHooks() override;

    inline ClientEntityReplicationEventHooks& GetClientReplicationEventHooks()
    {
        return replicationHooks_;
    }

    UIEventHooks& GetUIEvents()
    {
        return uiEvents_;
    }

    DeferredUIEvents& GetDeferredUIEvents()
    {
        return deferredUIEvents_;
    }

    extui::IMGUIObjectManager& IMGUI()
    {
        return *imgui_;
    }

    // Values boxed by Ext.UI.Box(), kept alive until the end of the frame
    inline Array<Noesis::Ptr<Noesis::BaseComponent>>& GetBoxedValues()
    {
        return boxedValues_;
    }

private:
    ExtensionLibraryClient library_;
    CameraEventHooks cameraHooks_;
    ClientEntityReplicationEventHooks replicationHooks_;
    UIEventHooks uiEvents_;
    DeferredUIEvents deferredUIEvents_;
    GameUniquePtr<extui::IMGUIObjectManager> imgui_;
    Array<Noesis::Ptr<Noesis::BaseComponent>> boxedValues_;
};

END_NS()
