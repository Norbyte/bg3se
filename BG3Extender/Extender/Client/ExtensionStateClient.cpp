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
	if (Lua) Lua->Shutdown();
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

void ExtensionState::OnUpdate(GameTime const& time)
{
	FireInputEvents();
	ExtensionStateBase::OnUpdate(time);
}

void ExtensionState::DoLuaReset()
{
	if (Lua) Lua->Shutdown();
	Lua.reset();

	context_ = nextContext_;
	assert(context_ != ExtensionStateContext::Uninitialized);
	Lua = std::make_unique<lua::ClientState>(*this, nextGenerationId_++);
	Lua->Initialize();
}

void ExtensionState::LuaStartup()
{
	ExtensionStateBase::LuaStartup();

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
	if (!deferredInputEvents_.empty()) {
		LuaClientPin lua(*this);
		if (lua) {
			for (auto& e : deferredInputEvents_) {
				lua->OnInputEvent(&e, nullptr);
			}
		}

		deferredInputEvents_.clear();
	}
}

void ExtensionState::OnInputEvent(SDL_Event* event, int& result)
{
	// Only dispatch cancelable events from the SDL thread 
	// (i.e. where we need immediate feedback from Lua code); 
	// others are deferred to the client update loop
	if (Lua && Lua->IsEventCancelable(event)) {
		LuaClientPin lua(*this);
		if (lua) {
			lua->OnInputEvent(event, &result);
		}
	} else {
		deferredInputEvents_.push_back(*event);
	}
}

END_NS()
