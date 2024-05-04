#include <stdafx.h>
#include <resource.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <GameDefinitions/Components/All.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Net.h>

#include <Lua/LevelIteratorFunctions.inl>
#include <Lua/Shared/LuaShared.inl>
#include <Lua/Server/ServerStatus.inl>
#include <Lua/Server/FunctorEvents.inl>
#include <Lua/Server/ServerFunctors.inl>
#include <Lua/Server/EntityEvents.inl>



namespace bg3se::lua
{
	LifetimeHandle GetCurrentLifetime()
	{
		if (gExtender->GetServer().IsInServerThread()) {
			return esv::lua::GetServerLifetime();
		} else {
			return ecl::lua::GetClientLifetime();
		}
	}

	LifetimePool& GetLifetimePool()
	{
		if (gExtender->GetServer().IsInServerThread()) {
			return esv::lua::GetServerLifetimePool();
		} else {
			return ecl::lua::GetClientLifetimePool();
		}
	}
}

namespace bg3se::esv::lua
{
	using namespace bg3se::lua;

	LifetimeHandle GetServerLifetime()
	{
		assert(gExtender->GetServer().IsInServerThread());
		return esv::ExtensionState::Get().GetLua()->GetCurrentLifetime();
	}

	LifetimePool& GetServerLifetimePool()
	{
		assert(gExtender->GetServer().IsInServerThread());
		return esv::ExtensionState::Get().GetLua()->GetLifetimePool();
	}

	int RegisterOsirisListener(lua_State* L)
	{
		auto name = get<STDString>(L, 1);
		auto arity = get<int>(L, 2);
		auto typeName = get<char const*>(L, 3);
		luaL_checktype(L, 4, LUA_TFUNCTION);

		OsirisHookSignature::HookType type;
		if (strcmp(typeName, "before") == 0) {
			type = OsirisHookSignature::BeforeTrigger;
		} else if (strcmp(typeName, "after") == 0) {
			type = OsirisHookSignature::AfterTrigger;
		} else if (strcmp(typeName, "beforeDelete") == 0) {
			type = OsirisHookSignature::BeforeDeleteTrigger;
		} else if (strcmp(typeName, "afterDelete") == 0) {
			type = OsirisHookSignature::AfterDeleteTrigger;
		} else {
			luaL_error(L, "Hook type must be 'before', 'beforeDelete', 'after' or 'afterDelete'");
		}

		LuaServerPin lua(ExtensionState::Get());
		RegistryEntry handler(L, 4);
		auto subscriberId = lua->Osiris().GetOsirisCallbacks().Subscribe(name, arity, type, std::move(handler));
		push(L, subscriberId);
		return 1;
	}

	int UnregisterOsirisListener(lua_State* L)
	{
		auto subscriberId = get<uint32_t>(L, 1);

		LuaServerPin lua(ExtensionState::Get());
		push(L, lua->Osiris().GetOsirisCallbacks().Unsubscribe(subscriberId));
		return 1;
	}

	void RegisterOsirisLibrary(lua_State* L)
	{
		static const luaL_Reg extLib[] = {
			{"RegisterListener", RegisterOsirisListener},
			{"UnregisterListener", UnregisterOsirisListener},
			{0,0}
		};

		RegisterLib(L, "Osiris", extLib);
	}

	void ExtensionLibraryServer::RegisterLib(lua_State * L)
	{
		static const luaL_Reg extLib[] = {

			{0,0}
		};

		luaL_newlib(L, extLib); // stack: lib
		lua_setglobal(L, "Ext"); // stack: -

		RegisterSharedMetatables(L);
		RegisterOsirisLibrary(L);
		gModuleRegistry.ConstructState(L, ModuleRole::Server);
	}

	void ExtensionLibraryServer::Register(lua_State * L)
	{
		ExtensionLibrary::Register(L);
		OsiFunctionNameProxy::RegisterMetatable(L);
		RegisterNameResolverMetatable(L);
		CreateNameResolver(L);
	}


	ServerState::ServerState(ExtensionState& state, uint32_t generationId)
		: State(generationId, true),
		osiris_(state),
		functorHooks_(*this),
		replicationHooks_(*this)
	{}

	ServerState::~ServerState()
	{
		if (gExtender) {
#if !defined(OSI_NO_DEBUGGER)
			auto debugger = gExtender->GetLuaDebugger();
			if (debugger) {
				debugger->ServerStateDeleted();
			}
#endif

			// FIXME - HANDLE IN SERVER LOGIC!
			gExtender->GetServer().Osiris().GetCustomFunctionManager().ClearDynamicEntries();
		}
	}

	bool ServerState::IsClient()
	{
		return false;
	}

	void ServerState::Initialize()
	{
		State::Initialize();

		StackCheck _(L, 0);

		library_.Register(L);

		gExtender->GetServer().GetExtensionState().LuaLoadBuiltinFile("ServerStartup.lua");
		// Ext is not writeable after loading SandboxStartup!
		gExtender->GetServer().GetExtensionState().LuaLoadBuiltinFile("SandboxStartup.lua");

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugger();
		if (debugger) {
			debugger->ServerStateCreated(this);
		}
#endif
	}


	void ServerState::OnGameStateChanged(GameState fromState, GameState toState)
	{
		GameStateChangedEvent params{
			.FromState = fromState, 
			.ToState = toState
		};
		ThrowEvent("GameStateChanged", params, false, 0);
	}


	std::optional<STDString> ServerState::GetModPersistentVars(STDString const& modTable)
	{
		std::tuple<std::optional<char const*>> ret;
		if (CallExtRet("_GetModPersistentVars", RestrictAll, ret, modTable)) {
			return std::get<0>(ret);
		} else {
			return {};
		}
	}


	void ServerState::RestoreModPersistentVars(STDString const& modTable, STDString const& vars)
	{
		CallExt("_RestoreModPersistentVars", RestrictAll, modTable, vars);
	}


	ecs::EntityWorld* ServerState::GetEntityWorld()
	{
		return GetStaticSymbols().GetServerEntityWorld();
	}


	ecs::EntitySystemHelpersBase* ServerState::GetEntitySystemHelpers()
	{
		return &gExtender->GetServer().GetEntityHelpers();
	}

	EntityReplicationEventHooks* ServerState::GetReplicationEventHooks()
	{
		return &replicationHooks_;
	}


	void ServerState::OnGameSessionLoading()
	{
		osiris_.GetIdentityAdapterMap().UpdateAdapters();

		State::OnGameSessionLoading();
	}

	void ServerState::StoryFunctionMappingsUpdated()
	{
		auto helpers = library_.GenerateOsiHelpers();
		LoadScript(helpers, "bootstrapper");
	}
}

namespace bg3se::esv
{

	ExtensionState & ExtensionState::Get()
	{
		return gExtender->GetServer().GetExtensionState();
	}

	ExtensionState::ExtensionState()
		: ExtensionStateBase(true)
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

	ModManager * ExtensionState::GetModManager()
	{
		auto server = GetEoCServer();
		if (server) {
			return &server->ModManager;
		} else {
			return nullptr;
		}
	}

	void ExtensionState::OnGameSessionLoading()
	{
		dynamicStats_.clear();
		persistentStats_.clear();
		cachedPersistentVars_.clear();
		bg3se::ExtensionStateBase::OnGameSessionLoading();
	}

	void ExtensionState::Reset()
	{
		ExtensionStateBase::Reset();
	}

	void ExtensionState::DoLuaReset()
	{
		if (Lua) Lua->Shutdown();
		Lua.reset();

		context_ = nextContext_;
		Lua = std::make_unique<lua::ServerState>(*this, nextGenerationId_++);
		LuaStatePin<ExtensionState, lua::ServerState> pin(*this);
		pin->Initialize();
		pin->StoryFunctionMappingsUpdated();
	}

	void ExtensionState::LuaStartup()
	{
		ExtensionStateBase::LuaStartup();

		LuaServerPin lua(*this);
		auto gameState = GetStaticSymbols().GetServerState();
		if (gameState
			&& (*gameState == esv::GameState::LoadLevel
				|| (*gameState == esv::GameState::LoadModule && WasStatLoadTriggered())
				|| *gameState == esv::GameState::LoadSession
				|| *gameState == esv::GameState::Sync
				|| *gameState == esv::GameState::Paused
				|| *gameState == esv::GameState::Running)) {
			lua->OnModuleResume();
		}
	}

	void ExtensionState::MarkPersistentStat(FixedString const& statId)
	{
		persistentStats_.insert(statId);
	}

	void ExtensionState::UnmarkPersistentStat(FixedString const& statId)
	{
		auto it = persistentStats_.find(statId);
		if (it != persistentStats_.end()) {
			persistentStats_.erase(it);
		}
	}

	void ExtensionState::MarkDynamicStat(FixedString const& statId)
	{
		dynamicStats_.insert(statId);
	}


	std::optional<STDString> ExtensionState::GetModPersistentVars(FixedString const& mod)
	{
		auto modTable = GetModTable(mod);
		if (modTable) {
			LuaServerPin lua(*this);
			if (lua) {
				auto vars = lua->GetModPersistentVars(*modTable);
				if (vars) {
					return vars;
				}
			}
		}

		// If we failed to retrieve vars from the Lua state for some reason, 
		// return the persistent vars we loaded from the savegame instead
		auto cachedIt = cachedPersistentVars_.find(mod);
		if (cachedIt != cachedPersistentVars_.end()) {
			ERR("Persistent variables for mod %s could not be retrieved, saving cached values!", mod.GetString());
			return cachedIt->second;
		} else {
			return {};
		}
	}

	void ExtensionState::RestoreModPersistentVars(FixedString const& mod, STDString const& vars)
	{
		cachedPersistentVars_.insert(std::make_pair(mod, vars));

		auto modTable = GetModTable(mod);
		if (modTable) {
			DEBUG("Restoring persistent vars for mod %s (%ld bytes)", mod.GetString(), vars.size());
			LuaServerPin lua(*this);
			if (lua) {
				lua->RestoreModPersistentVars(*modTable, vars);
			}
		} else {
			ERR("Savegame has persistent variables for mod %s, but it is not loaded or has no ModTable! Variables may be lost on next save!", mod.GetString());
		}
	}

	std::optional<STDString> ExtensionState::GetModTable(FixedString const& mod)
	{
		auto const& configs = GetConfigs();
		auto configIt = configs.find(mod);
		if (configIt != configs.end() && !configIt->second.ModTable.empty()) {
			return configIt->second.ModTable;
		} else {
			return {};
		}
	}

	std::unordered_set<FixedString> ExtensionState::GetPersistentVarMods()
	{
		std::unordered_set<FixedString> names;
		for (auto const& kv : cachedPersistentVars_) {
			names.insert(kv.first);
		}

		for (auto const& config : GetConfigs()) {
			if (config.second.MinimumVersion >= 4 && !config.second.ModTable.empty()) {
				names.insert(config.first);
			}
		}

		return names;
	}

	void ExtensionState::StoryLoaded()
	{
		DEBUG("ExtensionStateServer::StoryLoaded()");
		LuaServerPin lua(*this);
		if (lua) {
			lua->Osiris().StoryLoaded();
		}
	}

	void ExtensionState::StoryFunctionMappingsUpdated()
	{
		LuaServerPin lua(*this);
		if (lua) {
			lua->StoryFunctionMappingsUpdated();
		}
	}

	void ExtensionState::StorySetMerging(bool isMerging)
	{
		LuaServerPin lua(*this);
		if (lua) {
			lua->Osiris().StorySetMerging(isMerging);
		}
	}
}
