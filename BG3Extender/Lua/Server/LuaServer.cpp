#include <stdafx.h>
#include <resource.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/Server/LuaBindingServer.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Stats.h>
#include <GameDefinitions/Components/Boosts.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Resources.h>

#include <Lua/LevelIteratorFunctions.inl>
#include <Lua/Shared/LuaShared.inl>
#include <Lua/Server/ServerCharacter.inl>
#include <Lua/Server/ServerItem.inl>
#include <Lua/Server/ServerStatus.inl>
#include <Lua/Server/ServerProjectile.inl>
#include <Lua/Server/ServerSurface.inl>
#include <Lua/Server/ServerCombat.inl>
#include <Lua/Server/ServerNetwork.inl>
#include <Lua/Server/FunctorEvents.inl>
#include <Lua/Server/ServerFunctors.inl>



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

	/*
	* 
	int GetAiGrid(lua_State* L)
	{
		auto level = GetStaticSymbols().GetCurrentServerLevel();
		if (!level || !level->AiGrid) {
			OsiError("Current level not available yet!");
			return 0;
		}

		ObjectProxy<eoc::AiGrid>::New(L, level->AiGrid);
		return 1;
	}

	int ExecuteSkillPropertiesOnTarget(lua_State* L)
	{
		StackCheck _(L, 0);
		auto skillId = get<FixedString>(L, 1);
		auto attacker = GetCharacter(L, 2);
		auto target = GetCharacter(L, 3);
		auto position = get<glm::vec3>(L, 4);
		auto propertyContext = get<CRPGStats_Object_PropertyContext>(L, 5);
		auto isFromItem = get<bool>(L, 6);

		SkillPrototype* skillProto{ nullptr };
		auto skillProtoMgr = GetStaticSymbols().eoc__SkillPrototypeManager;
		if (skillProtoMgr && *skillProtoMgr) {
			auto proto = (*skillProtoMgr)->Prototypes.Find(skillId);
			if (*proto) {
				skillProto = *proto;
			}
		}

		if (!skillProto) {
			LuaError("Couldn't find skill prototype for " << skillId);
			return 0;
		}

		auto exec = GetStaticSymbols().esv__ExecuteCharacterSetExtraProperties;
		auto skillProperties = skillProto->GetStats()->PropertyList.Find(GFS.strSkillProperties);

		if (!skillProperties) {
			LuaError("Skill " << skillId << " has no SkillProperties!");
			return 0;
		}

		if (!attacker || !target || !exec) {
			return 0;
		}

		ComponentHandle attackerHandle;
		attacker->GetComponentHandle(attackerHandle);
		ObjectSet<esv::Character*> targets;
		targets.Add(target);

		exec(*skillProperties, attackerHandle.Handle, targets, position, propertyContext, isFromItem,
			skillProto, nullptr, 0.0f, nullptr, false, 2.4f);
		return 0;
	}

	int ExecuteSkillPropertiesOnPosition(lua_State* L)
	{
		StackCheck _(L, 0);
		auto skillId = get<FixedString>(L, 1);
		auto attacker = GetCharacter(L, 2);
		auto position = get<glm::vec3>(L, 3);
		auto radius = get<float>(L, 4);
		auto propertyContext = get<CRPGStats_Object_PropertyContext>(L, 5);
		auto isFromItem = get<bool>(L, 6);

		SkillPrototype* skillProto{ nullptr };
		auto skillProtoMgr = GetStaticSymbols().eoc__SkillPrototypeManager;
		if (skillProtoMgr && *skillProtoMgr) {
			auto proto = (*skillProtoMgr)->Prototypes.Find(skillId);
			if (*proto) {
				skillProto = *proto;
			}
		}

		if (!skillProto) {
			LuaError("Couldn't find skill prototype for " << skillId);
			return 0;
		}

		auto exec = GetStaticSymbols().esv__ExecutePropertyDataOnPositionOnly;
		auto skillProperties = skillProto->GetStats()->PropertyList.Find(GFS.strSkillProperties);

		if (!skillProperties) {
			LuaError("Skill " << skillId << " has no SkillProperties!");
			return 0;
		}

		if (!attacker || !exec) {
			return 0;
		}

		ComponentHandle attackerHandle;
		attacker->GetComponentHandle(attackerHandle);

		exec(*skillProperties, attackerHandle.Handle, &position, radius, propertyContext, isFromItem, skillProto, nullptr);
		return 0;
	}

	int OsirisIsCallable(lua_State* L)
	{
		StackCheck _(L, 1);
		LuaServerPin lua(ExtensionState::Get());
		bool allowed = gExtender->IsStoryLoaded()
			&& ((lua->RestrictionFlags & State::RestrictOsiris) == 0);
		push(L, allowed);
		return 1;
	}



	int GetGameState(lua_State* L)
	{
		auto state = GetStaticSymbols().GetServerState();
		if (state) {
			push(L, *state);
		} else {
			lua_pushnil(L);
		}

		return 1;
	}

	*/

	int XDumpBoosts(lua_State* L)
	{
		auto s = get<FixedString>(L, 1);

		auto& helpers = gExtender->GetServer().GetEntityHelpers();
		auto character = helpers.GetComponent<esv::Character>(s);
		if (character) {
			auto boostContainer = helpers.GetEntityComponent<BoostsContainerComponent>(character->Base.Entity);

			if (boostContainer) {
				std::cout << "Boosts:" << std::endl;
				for (auto const& boostSet : boostContainer->Boosts) {
					for (auto boostHandle : boostSet.Value) {
						auto boost = helpers.GetEntityComponent<BoostInfoComponent>(boostHandle);
						std::cout << EnumInfo<BoostType>::Find(boost->Type)
							<< " Flags=" << boost->Flags_M
							<< " Passive=" << boost->Passive.GetString()
							<< " Arg1=" << boost->Params.Boost.GetString()
							<< " Arg2=" << boost->Params.Params
							<< " Arg3=" << boost->Params.Params2
							<< std::endl;

					}
				}
			}
		}

		return 0;
	}

	int XGetResource(lua_State* L)
	{
		auto s = get<FixedString>(L, 1);

		auto rsrc = GetStaticSymbols().ResourceDefns;
		if (rsrc && *rsrc) {
			auto const& mgrs = (*rsrc)->DefinitionManagers;
			auto mgr = mgrs[0];
		}

		return 0;
	}

	int XGetTag(lua_State* L)
	{
		auto s = get<Guid>(L, 1);

		auto rsrc = gExtender->GetServer().GetEntityHelpers().GetResourceManager<TagResource>();
		if (rsrc) {

			auto tag = reinterpret_cast<TagResource*>((*rsrc)->Resources.Values.begin().get());
			std::cout << tag->Name.GetString() << std::endl;
			std::cout << tag->Icon.GetString() << std::endl;

			auto r2 = (*rsrc)->Resources.Find(s);
			if (r2) {
				std::cout << (*r2)->Name.GetString() << std::endl;
				std::cout << (*r2)->Description << std::endl;
				std::cout << (*r2)->Icon.GetString() << std::endl;
			}
		}

		return 0;
	}

	int XGetByGuid(lua_State* L)
	{
		auto s = get<FixedString>(L, 1);
		auto& types = GetEoCServer()->EntityWorld->Components.Types;
		for (unsigned i = 0; i < types.Size(); i++) {
			auto pool = types[i].Pool;
			if (pool) {
				auto comp = pool->Factory.FindByGuid(s);
				if (comp) {
					auto ch = reinterpret_cast<esv::Character*>(comp);
					auto ch2 = reinterpret_cast<esv::Character*>((uint64_t)comp - 8);
					std::cout << "Found in pool " << i << std::endl;

					auto entity = ch2->Base.Entity;

					auto& helpers = gExtender->GetServer().GetEntityHelpers();
					auto ar = helpers.GetEntityComponent<ActionResourcesComponent>(entity);
					auto armor = helpers.GetEntityComponent<ArmorComponent>(entity);
					auto baseHp = helpers.GetEntityComponent<BaseHpComponent>(entity);
					auto data = helpers.GetEntityComponent<DataComponent>(entity);
					auto exp = helpers.GetEntityComponent<ExperienceComponent>(entity);
					auto health = helpers.GetEntityComponent<HealthComponent>(entity);
					auto passive = helpers.GetEntityComponent<PassiveComponent>(entity);
					auto sense = helpers.GetEntityComponent<HearingComponent>(entity);
					auto spells = helpers.GetEntityComponent<SpellBookComponent>(entity);
					auto stats = helpers.GetEntityComponent<StatsComponent>(entity);
					auto immun = helpers.GetEntityComponent<StatusImmunitiesComponent>(entity);
					auto spi = helpers.GetEntityComponent<SurfacePathInfluencesComponent>(entity);
					auto use = helpers.GetEntityComponent<UseComponent>(entity);
					auto value = helpers.GetEntityComponent<ValueComponent>(entity);
					auto weapon = helpers.GetEntityComponent<WeaponComponent>(entity);
					auto wielding = helpers.GetEntityComponent<WieldingComponent>(entity);
					auto customStats = helpers.GetEntityComponent<CustomStatsComponent>(entity);

					//auto bc = helpers.GetEntityComponent<BoostConditionComponent>(entity);
					auto bcn = helpers.GetEntityComponent<BoostsContainerComponent>(entity);
					auto arm = helpers.GetEntityComponent<ActionResourceConsumeMultiplierBoost>(entity);
					auto cp = helpers.GetEntityComponent<CombatParticipantComponent>(entity);
					auto gc = helpers.GetEntityComponent<GenderComponent>(entity);
					auto sc = helpers.GetEntityComponent<SpellContainerComponent>(entity);
					auto t = helpers.GetEntityComponent<TagComponent>(entity);
					auto ot = helpers.GetEntityComponent<OsirisTagComponent>(entity);
					auto sbp = helpers.GetEntityComponent<SpellBookPreparesComponent>(entity);
					auto cs = helpers.GetEntityComponent<CombatStateComponent>(entity);
					auto tb = helpers.GetEntityComponent<TurnBasedComponent>(entity);
					auto to = helpers.GetEntityComponent<TurnOrderComponent>(entity);
					auto tf = helpers.GetEntityComponent<TransformComponent>(entity);
					auto pc = helpers.GetEntityComponent<PassiveContainerComponent>(entity);

					if (bcn) {
						for (auto const& boostSet : bcn->Boosts) {
							for (auto handle : boostSet.Value) {
								auto bi = helpers.GetEntityComponent<BoostInfoComponent>(handle);
								auto f2c = bi->Passive.GetString();
								auto p2 = bi->Params.Boost.GetString();
								auto ac = helpers.GetEntityComponent<ArmorClassBoostComponent>(handle);
								auto ab = helpers.GetEntityComponent<AbilityBoostComponent>(handle);
								auto rb = helpers.GetEntityComponent<RollBonusBoostComponent>(handle);
								auto ad = helpers.GetEntityComponent<AdvantageBoostComponent>(handle);
								auto ar = helpers.GetEntityComponent<ActionResourceValueBoostComponent>(handle);
								auto ch = helpers.GetEntityComponent<CriticalHitBoostComponent>(handle);
								auto rbb = helpers.GetEntityComponent<ResistanceBoostComponent>(handle);

							}
						}
					}

					return 0;
				}
			}
		}

		std::cout << "NOTFOUND!" << std::endl;
		return 0;
	}

	int XGetByHandle(lua_State* L)
	{
		/*auto s = get<EntityProxy*>(L, 1)->Handle();
		auto& types = GetEoCServer()->EntityWorld->Components.Types;
		for (auto i = 0; i < types.Size; i++) {
			auto pool = types[i].Pool;
			if (pool) {
				auto comp = pool->FindComponentByHandle(s);
				if (comp) {
					std::cout << "Found in pool " << i << std::endl;
				}
			}
		}*/

		return 0;
	}

	int XTest(lua_State* L)
	{
		auto stats = GetStaticSymbols().Stats;
		auto const& functors = (*stats)->StatsFunctors;
		DealDamageFunctor* dd{ nullptr };

		for (auto const& kv : functors) {
			DEBUG("asdf");
			for (auto const& fun : kv.Value->FunctorList) {
				if (fun->TypeId == StatsFunctorActionId::DealDamage && fun->Cast<DealDamageFunctor>()) {
					dd = *fun->Cast<DealDamageFunctor>();
				}
			}
		}

		if (dd) {
			ObjectProxy::MakeRef<DealDamageFunctor>(L, dd, GetCurrentLifetime());
			return 1;
		}

		return 0;
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
		lua->Osiris().GetOsirisCallbacks().Subscribe(name, arity, type, std::move(handler));
		return 0;
	}

	void RegisterOsirisLibrary(lua_State* L)
	{
		static const luaL_Reg extLib[] = {
			{"RegisterListener", RegisterOsirisListener},
			{0,0}
		};

		RegisterLib(L, "Osiris", extLib);
	}

	void ExtensionLibraryServer::RegisterLib(lua_State * L)
	{
		static const luaL_Reg extLib[] = {

			{"XGetByGuid", XGetByGuid},
			{"XGetByHandle", XGetByHandle},
			{"XDumpBoosts", XDumpBoosts},
			{"XGetResource", XGetResource},
			{"XGetTag", XGetTag},
			{"XTest", XTest},

			/*{"GetSurfaceTransformRules", GetSurfaceTransformRules},
			{"UpdateSurfaceTransformRules", UpdateSurfaceTransformRules},
			{"CreateSurfaceAction", CreateSurfaceAction},
			{"ExecuteSurfaceAction", ExecuteSurfaceAction},
			{"CancelSurfaceAction", CancelSurfaceAction},*/

			{"GetAllCharacters", GetAllCharacters},
			{"GetCharactersAroundPosition", GetCharactersAroundPosition},
			{"GetAllItems", GetAllItems},
			{"GetItemsAroundPosition", GetItemsAroundPosition},
			/*{"CreateItemConstructor", CreateItemConstructor},*/
			{"GetCharacter", GetCharacter},
			{"GetItem", GetItem},
			{"GetGameObject", GetGameObject},
			/*{"GetCombat", GetCombat},
			{"GetSurface", GetSurface},
			{"GetAiGrid", GetAiGrid},
			{"NewDamageList", NewDamageList},
			{"GetSurfaceTemplate", GetSurfaceTemplate},
			{"OsirisIsCallable", OsirisIsCallable},*/

			/*{"BroadcastMessage", BroadcastMessage},
			{"PostMessageToClient", PostMessageToClient},
			{"PostMessageToUser", PostMessageToUser},
			{"PlayerHasExtender", PlayerHasExtender},*/

			{0,0}
		};

		luaL_newlib(L, extLib); // stack: lib
		lua_setglobal(L, "Ext"); // stack: -

		RegisterSharedLibraries(L);
		RegisterOsirisLibrary(L);
		RegisterFunctorsLibrary(L);
		RegisterSurfaceLibrary(L);
	}

	void ExtensionLibraryServer::Register(lua_State * L)
	{
		ExtensionLibrary::Register(L);

		/*auto & pm = StaticLuaPropertyMap<DealDamageFunctor>::PropertyMap;
		pm.AddProperty("DamageType", 
			[](lua_State* L, DealDamageFunctor* obj) {
				return LuaWrite(L, obj->Arg1_DamageType) == 1;
			},
			[](lua_State* L, DealDamageFunctor* obj, int index) {
				lua_pushvalue(L, index);
				LuaRead(L, obj->Arg1_DamageType);
				lua_pop(L, 1);
				return true;
			}
		);*/

		/*ObjectProxy<esv::Status>::RegisterMetatable(L);*/
		/*ObjectProxy<eoc::ItemDefinition>::RegisterMetatable(L);*/
		//ObjectProxy<esv::Projectile>::RegisterMetatable(L);
		/*ObjectProxy<esv::ShootProjectileHelper>::RegisterMetatable(L);
		ObjectProxy<esv::SurfaceAction>::RegisterMetatable(L);*/

		OsiFunctionNameProxy::RegisterMetatable(L);
		/*StatusHandleProxy::RegisterMetatable(L);
		TurnManagerCombatProxy::RegisterMetatable(L);
		TurnManagerTeamProxy::RegisterMetatable(L);
		ItemConstructor::RegisterMetatable(L);*/
		RegisterNameResolverMetatable(L);
		CreateNameResolver(L);
	}


	ServerState::ServerState(ExtensionState& state)
		: State(true),
		osiris_(state),
		functorHooks_(*this)
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
		StackCheck _(L, 0);

		library_.Register(L);

		gExtender->GetServer().GetExtensionState().LuaLoadBuiltinFile("ServerStartup.lua");
		/*
		lua_getglobal(L, "Ext"); // stack: Ext
		StatsExtraDataProxy::New(L); // stack: Ext, "ExtraData", ExtraDataProxy
		lua_setfield(L, -2, "ExtraData"); // stack: Ext
		lua_pop(L, 1); // stack: -
		*/
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
		GameStateChangeEventParams params{ fromState, toState };
		ThrowEvent("GameStateChanged", params, false, 0, ReadOnlyEvent{});
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


	EntityWorldBase* ServerState::GetEntityWorld()
	{
		return GetStaticSymbols().GetServerEntityWorld();
	}


	EntitySystemHelpersBase* ServerState::GetEntitySystemHelpers()
	{
		return &gExtender->GetServer().GetEntityHelpers();
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
		Lua = std::make_unique<lua::ServerState>(*this);
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
			if (config.second.MinimumVersion >= 43 && !config.second.ModTable.empty()) {
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
