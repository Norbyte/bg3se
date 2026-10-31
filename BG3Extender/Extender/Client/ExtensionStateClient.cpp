#include <stdafx.h>
#include <Extender/Client/ExtensionStateClient.h>
#include <GameDefinitions/Level.h>
#include <GameDefinitions/Symbols.h>
#include <Extender/ScriptExtender.h>
#include <Lua/Client/ClientEvents.h>

BEGIN_NS(ecl)

ExtensionState & ExtensionState::Get()
{
    return gExtender->GetClient().GetExtensionState();
}

ExtensionState::ExtensionState()
    : ExtensionStateBase(false)
{}

ExtensionState::~ExtensionState()
{
    if (Lua) {
        IncLuaRefs();
        ShutdownLuaState();
        DecLuaRefs(false);
    }
}

lua::State * ExtensionState::GetLua()
{
    if (Lua) {
        return Lua.get();
    } else {
        return nullptr;
    }
}

ModManager* ExtensionState::GetModManager()
{
    return GetStaticSymbols().GetModManagerClient();
}

bg3se::LevelManager* ExtensionState::GetLevelManager()
{
    return GetStaticSymbols().GetClientLevelManager();
}

void ExtensionState::OnUpdate(GameTime const& time)
{
    OPTICK_EVENT("Client update");
    FireInputEvents();
    ExtensionStateBase::OnUpdate(time);
}

void ExtensionState::ShutdownLuaState()
{
    EnteredCheck();
    if (Lua) {
        Lua->OnShutdown();
        Lua->Shutdown();
    }
    Lua.reset();
}

void ExtensionState::InitializeLuaState()
{
    context_ = nextContext_;
    se_assert(context_ != ExtensionStateContext::Uninitialized);
    EnteredCheck();

    Lua = std::make_unique<lua::ClientState>(*this, nextGenerationId_++);
    Lua->Initialize();
}

void ExtensionState::BootstrapLua()
{
    ExtensionStateBase::BootstrapLua();

    auto gameState = GetStaticSymbols().GetClientState();
    if (gameState
        && (*gameState == GameState::LoadLevel
            || (*gameState == GameState::LoadModule && WasStatLoadTriggered())
            || *gameState == GameState::LoadSession
            || *gameState == GameState::Paused
            || *gameState == GameState::PrepareRunning
            || *gameState == GameState::Running)) {
        lua::EmptyEventParams evt;
        ThrowEvent<ModuleResumeEvent>(evt);
    }
}

using namespace bg3se::lua;
using namespace bg3se::ecl::lua;

bool IsEventCancelable(SDL_Event* event)
{
    return event->type == SDL_KEYDOWN
        || event->type == SDL_KEYUP
        || event->type == SDL_MOUSEBUTTONDOWN
        || event->type == SDL_MOUSEBUTTONUP
        || event->type == SDL_MOUSEWHEEL
        || event->type == SDL_CONTROLLERBUTTONDOWN
        || event->type == SDL_CONTROLLERBUTTONUP;
}

void ExtensionState::FireInputEvent(SDL_Event* event, int* result)
{
    EventResult res{ EventResult::Successful };
    bool canPrevent = (result != nullptr) && IsEventCancelable(event);

    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        KeyInputEventParams params{
            .Event = (event->key.type == SDL_KEYDOWN) ? SDLKeyEvent::KeyDown : SDLKeyEvent::KeyUp,
            .Key = (SDLScanCode)event->key.keysym.scancode,
            .Modifiers = (SDLKeyModifier)event->key.keysym.mod,
            .Pressed = event->key.state == SDL_PRESSED,
            .Repeat = event->key.repeat != 0,
        };
        res = ThrowEvent<KeyInputEvent>(params, canPrevent);

    } else if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        MouseButtonEventParams params{
            .Button = event->button.button,
            .Pressed = event->button.state == SDL_PRESSED,
            .Clicks = event->button.clicks,
            .X = event->button.x,
            .Y = event->button.y,
        };
        res = ThrowEvent<MouseButtonInputEvent>(params, canPrevent);

    } else if (event->type == SDL_MOUSEWHEEL) {
        MouseWheelEventParams params{
            .ScrollX = event->wheel.preciseX,
            .ScrollY = event->wheel.preciseY,
            .X = event->wheel.x,
            .Y = event->wheel.y,
        };
        res = ThrowEvent<MouseWheelInputEvent>(params, canPrevent);

    } else if (event->type == SDL_CONTROLLERAXISMOTION) {
        ControllerAxisEventParams params{
            .DeviceId = event->caxis.which,
            .Axis = (SDLControllerAxis)event->caxis.axis,
            .Value = event->caxis.value / 32768.0f,
        };
        res = ThrowEvent<ControllerAxisInputEvent>(params, canPrevent);

    } else if (event->type == SDL_CONTROLLERBUTTONDOWN || event->type == SDL_CONTROLLERBUTTONUP) {
        ControllerButtonEventParams params{
            .DeviceId = event->cbutton.which,
            .Event = (event->type == SDL_CONTROLLERBUTTONDOWN) ? SDLKeyEvent::KeyDown : SDLKeyEvent::KeyUp,
            .Button = (SDLControllerButton)event->cbutton.button,
            .Pressed = event->cbutton.state == SDL_PRESSED,
        };
        res = ThrowEvent<ControllerButtonInputEvent>(params, canPrevent);

    } else if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        ViewportResizedEventParams params{
            .Width = event->window.data1,
            .Height = event->window.data2,
        };
        res = ThrowEvent<ViewportResizedEvent>(params, canPrevent);
    }

    if (res == EventResult::ActionPrevented) {
        *result = 0;
    }
}

void ExtensionState::FireInputEvents()
{
    OPTICK_EVENT(Optick::Category::Input);
    if (!deferredInputEvents_.empty()) {
        SDL_Event evt;
        while (deferredInputEvents_.try_pop(evt)) {
            FireInputEvent(&evt, nullptr);
        }
    }
}

void ExtensionState::OnInputEvent(SDL_Event* event, int& result)
{
    // Only dispatch cancelable events from the SDL thread 
    // (i.e. where we need immediate feedback from Lua code); 
    // others are deferred to the client update loop
    if (Lua && IsEventCancelable(event)) {
        ContextGuard ctx(ContextType::Client);
        FireInputEvent(event, &result);
    } else {
        deferredInputEvents_.push(*event);
    }
}

END_NS()
