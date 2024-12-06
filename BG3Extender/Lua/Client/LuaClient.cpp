#include <stdafx.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Client/LuaBindingClient.h>
#include <Lua/Shared/LuaModule.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/ExtensionStateClient.h>
#include "resource.h"
#if defined(ENABLE_UI)
#include <Lua/Client/UIEvents.inl>
#endif
#if defined(ENABLE_IMGUI)
#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Objects.h>
#endif
#include <SDL_events.h>

#include <Lua/Client/ClientEntityReplicationEvents.inl>

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

LifetimeHandle GetClientLifetime()
{
    assert(gExtender->GetClient().IsInClientThread());
    return ExtensionState::Get().GetLua()->GetCurrentLifetime();
}

LifetimePool& GetClientLifetimePool()
{
    assert(gExtender->GetClient().IsInClientThread());
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
    assert(gExtender->GetClient().IsInClientThread());
    return static_cast<ClientState*>(State::FromLua(L));
}

ClientState::ClientState(ExtensionState& state, uint32_t generationId)
    : State(state, generationId, false),
    replicationHooks_(*this)
#if defined(ENABLE_UI)
    , uiEvents_(*this)
#endif
{}

ClientState::~ClientState()
{
    auto & sym = GetStaticSymbols();

#if defined(ENABLE_IMGUI)
    if (imgui_) {
        gExtender->IMGUI().SetObjects(nullptr);
        delete imgui_;
    }
#endif

#if !defined(OSI_NO_DEBUGGER)
    if (gExtender) {
        auto debugger = gExtender->GetLuaDebugger();
        if (debugger) {
            debugger->ClientStateDeleted();
        }
    }
#endif
}

void ClientState::Initialize()
{
    State::Initialize();

    StackCheck _(L, 0);
    library_.Register(L);

    gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("ClientStartup.lua");
    // Ext is not writeable after loading SandboxStartup!
    gExtender->GetClient().GetExtensionState().LuaLoadBuiltinFile("SandboxStartup.lua");

#if !defined(OSI_NO_DEBUGGER)
    auto debugger = gExtender->GetLuaDebugger();
    if (debugger) {
        debugger->ClientStateCreated(this);
    }
#endif
}

bool ClientState::IsClient()
{
    return true;
}

void ClientState::OnUpdate(GameTime const& time)
{
    State::OnUpdate(time);
    replicationHooks_.PostUpdate();
}


ecs::EntityWorld* ClientState::GetEntityWorld()
{
    return GetStaticSymbols().GetClientEntityWorld();
}


ecs::EntitySystemHelpersBase* ClientState::GetEntitySystemHelpers()
{
    return &gExtender->GetClient().GetEntityHelpers();
}

EntityReplicationEventHooks* ClientState::GetReplicationEventHooks()
{
    return &replicationHooks_;
}

void ClientState::OnGameStateChanged(GameState fromState, GameState toState)
{
    GameStateChangedEvent params{
        .FromState = fromState, 
        .ToState = toState
    };
    ThrowEvent("GameStateChanged", params, false, 0);
}

bool ClientState::IsEventCancelable(SDL_Event* event)
{
    return event->type == SDL_KEYDOWN
        || event->type == SDL_KEYUP
        || event->type == SDL_MOUSEBUTTONDOWN
        || event->type == SDL_MOUSEBUTTONUP
        || event->type == SDL_MOUSEWHEEL
        || event->type == SDL_CONTROLLERBUTTONDOWN 
        || event->type == SDL_CONTROLLERBUTTONUP;
}

void ClientState::OnInputEvent(SDL_Event* event, int* result)
{
    EventResult res{ EventResult::Successful };
    bool canPrevent = (result != nullptr) && IsEventCancelable(event);

    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        KeyInputEvent params{
            .Event = (event->key.type == SDL_KEYDOWN) ? SDLKeyEvent::KeyDown : SDLKeyEvent::KeyUp,
            .Key = (SDLScanCode)event->key.keysym.scancode,
            .Modifiers = (SDLKeyModifier)event->key.keysym.mod,
            .Pressed = event->key.state == SDL_PRESSED,
            .Repeat = event->key.repeat != 0,
        };
        res = ThrowEvent("KeyInput", params, canPrevent, 0);

    } else if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        MouseButtonEvent params{
            .Button = event->button.button,
            .Pressed = event->button.state == SDL_PRESSED,
            .Clicks = event->button.clicks,
            .X = event->button.x,
            .Y = event->button.y,
        };
        res = ThrowEvent("MouseButtonInput", params, canPrevent, 0);

    } else if (event->type == SDL_MOUSEWHEEL) {
        MouseWheelEvent params{
            .ScrollX = event->wheel.preciseX,
            .ScrollY = event->wheel.preciseY,
            .X = event->wheel.x,
            .Y = event->wheel.y,
        };
        res = ThrowEvent("MouseWheelInput", params, canPrevent, 0);

    } else if (event->type == SDL_CONTROLLERAXISMOTION) {
        ControllerAxisEvent params{
            .DeviceId = event->caxis.which,
            .Axis = (SDLControllerAxis)event->caxis.axis,
            .Value = event->caxis.value / 32768.0f,
        };
        res = ThrowEvent("ControllerAxisInput", params, canPrevent, 0);

    } else if (event->type == SDL_CONTROLLERBUTTONDOWN || event->type == SDL_CONTROLLERBUTTONUP) {
        ControllerButtonEvent params{
            .DeviceId = event->cbutton.which,
            .Event = (event->type == SDL_CONTROLLERBUTTONDOWN) ? SDLKeyEvent::KeyDown : SDLKeyEvent::KeyUp,
            .Button = (SDLControllerButton)event->cbutton.button,
            .Pressed = event->cbutton.state == SDL_PRESSED,
        };
        res = ThrowEvent("ControllerButtonInput", params, canPrevent, 0);

    } else if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        ViewportResizedEvent params{
            .Width = event->window.data1,
            .Height = event->window.data2,
        };
        res = ThrowEvent("ViewportResized", params, canPrevent, 0);
    }

    if (res == EventResult::ActionPrevented) {
        *result = 0;
    }
}

#if defined(ENABLE_IMGUI)
extui::IMGUIObjectManager& ClientState::IMGUI()
{
    if (imgui_ == nullptr) {
        imgui_ = new extui::IMGUIObjectManager();
        gExtender->IMGUI().SetObjects(imgui_);
    }

    return *imgui_;
}
#endif

END_NS()
