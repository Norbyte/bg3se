#pragma once

#include <Extender/Shared/ExtensionState.h>
#include <Lua/Client/LuaBindingClient.h>

BEGIN_NS(ecl)

using LuaClientPin = LuaStatePin<ExtensionState, lua::ClientState>;

class ExtensionState : public ExtensionStateBase
{
public:
    static ExtensionState & Get();

    ExtensionState();
    ~ExtensionState() override;

    lua::State * GetLua() override;
    ModManager * GetModManager() override;
    bg3se::LevelManager * GetLevelManager() override;
    void OnUpdate(GameTime const& time) override;

    lua::ClientState* GetClientLua()
    {
        if (Lua) {
            return Lua.get();
        } else {
            return nullptr;
        }
    }

    inline char const * GetBootstrapFileName() override
    {
        return "BootstrapClient.lua";
    }

    void OnInputEvent(SDL_Event* event, int& result);

    inline char const * GetUnconditionalFileName() override
    {
        return "OverrideClient.lua";
    }

protected:
    friend LuaStatePin<ExtensionState, lua::ClientState>;
    std::unique_ptr<lua::ClientState> Lua;
    uint32_t nextGenerationId_{ 1 };

    Array<SDL_Event> deferredInputEvents_;

    void DoLuaReset() override;
    void LuaStartup() override;
    void FireInputEvents();
};

END_NS()
