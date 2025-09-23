#include <stdafx.h>
#include <Extender/Client/ExtensionStateClient.h>
#include <GameDefinitions/Symbols.h>
#include <Extender/ScriptExtender.h>

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

    LuaClientPin lua(*this);
    auto gameState = GetStaticSymbols().GetClientState();
    if (gameState
        && (*gameState == GameState::LoadLevel
            || (*gameState == GameState::LoadModule && WasStatLoadTriggered())
            || *gameState == GameState::LoadSession
            || *gameState == GameState::Paused
            || *gameState == GameState::PrepareRunning
            || *gameState == GameState::Running)) {
        lua->OnModuleResume();
    }
}

void ExtensionState::FireInputEvents()
{
    OPTICK_EVENT(Optick::Category::Input);
    if (!deferredInputEvents_.empty()) {
        LuaClientPin lua(*this);
        if (lua) {
            SDL_Event evt;
            while (deferredInputEvents_.try_pop(evt)) {
                lua->OnInputEvent(&evt, nullptr);
            }
        }
    }
}

void ExtensionState::OnInputEvent(SDL_Event* event, int& result)
{
    // Only dispatch cancelable events from the SDL thread 
    // (i.e. where we need immediate feedback from Lua code); 
    // others are deferred to the client update loop
    if (Lua && Lua->IsEventCancelable(event)) {
        ContextGuard ctx(ContextType::Client);
        LuaClientPin lua(*this);
        if (lua) {
            lua->OnInputEvent(event, &result);
        }
    } else {
        deferredInputEvents_.push(*event);
    }
}

END_NS()
