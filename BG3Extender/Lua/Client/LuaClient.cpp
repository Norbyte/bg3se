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


ClientState::ClientState(uint32_t generationId)
	: State(generationId, false)
#if defined(ENABLE_UI)
	, uiEvents_(*this)
#endif
{}

ClientState::~ClientState()
{
	auto & sym = GetStaticSymbols();

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
	return nullptr;
}

void ClientState::OnGameStateChanged(GameState fromState, GameState toState)
{
	GameStateChangedEvent params{
		.FromState = fromState, 
		.ToState = toState
	};
	ThrowEvent("GameStateChanged", params, false, 0);
}

END_NS()
