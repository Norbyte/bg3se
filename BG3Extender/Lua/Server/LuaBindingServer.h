#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/Server/LuaOsirisBinding.h>
#include <Lua/Server/EntityEvents.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Status.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Lua/Shared/LuaStats.h>

BEGIN_NS(lua)

void RegisterLibraries();
void InitObjectProxyPropertyMaps();
void RegisterSharedMetatables(lua_State* L);

END_NS()

namespace bg3se::esv
{
	struct PendingHit;
	class ExtensionState;
}

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	LifetimeHandle GetServerLifetime();
	LifetimePool& GetServerLifetimePool();

	struct GameStateChangedEvent : public EventBase
	{
		esv::GameState FromState;
		esv::GameState ToState;
	};

	class StatusHandleProxy : public Userdata<StatusHandleProxy>, public Indexable, public NewIndexable
	{
	public:
		static char const * const MetatableName;

		inline StatusHandleProxy(EntityHandle character, ComponentHandle status)
			: character_(character), statusHandle_(status)
		{}

		inline StatusHandleProxy(EntityHandle character, NetId status)
			: character_(character), statusNetId_(status)
		{}

		int Index(lua_State * L);
		int NewIndex(lua_State * L);
		esv::Status* Get(lua_State* L);

	private:
		EntityHandle character_;
		ComponentHandle statusHandle_;
		NetId statusNetId_;
	};

	/*class TurnManagerCombatProxy : public Userdata<TurnManagerCombatProxy>, public Indexable<PushPolicy::None>
	{
	public:
		static char const * const MetatableName;

		static void PopulateMetatable(lua_State * L);

		inline TurnManagerCombatProxy(uint8_t combatId)
			: combatId_(combatId)
		{}

		inline esv::TurnManager::Combat * Get()
		{
			return GetEntityWorld()->GetTurnManager()->Combats.Find(combatId_);
		}

		int Index(lua_State * L);

	private:
		uint8_t combatId_;

		static int GetCurrentTurnOrder(lua_State * L);
		static int GetNextTurnOrder(lua_State * L);
		static int UpdateCurrentTurnOrder(lua_State * L);
		static int UpdateNextTurnOrder(lua_State * L);
		static int GetAllTeams(lua_State * L);
	};

	class TurnManagerTeamProxy : public Userdata<TurnManagerTeamProxy>, public Indexable<PushPolicy::None>
	{
	public:
		static char const * const MetatableName;

		inline TurnManagerTeamProxy(eoc::CombatTeamId teamId)
			: teamId_(teamId)
		{}

		inline eoc::CombatTeamId TeamId() const
		{
			return teamId_;
		}

		inline esv::TurnManager::CombatTeam * Get()
		{
			auto combat = GetEntityWorld()->GetTurnManager()->Combats.Find(teamId_.CombatId);
			if (combat) {
				auto team = combat->Teams.Find((uint32_t)teamId_);
				if (team) {
					return *team;
				} else {
					return nullptr;
				}
			} else {
				return nullptr;
			}
		}

		int Index(lua_State * L);

	private:
		eoc::CombatTeamId teamId_;
	};


	class ItemConstructor : public Userdata<ItemConstructor>, public Indexable<PushPolicy::None>
	{
	public:
		static char const* const MetatableName;

		inline ItemConstructor()
		{}

		inline ObjectSet<eoc::ItemDefinition>& Get()
		{
			return definition_;
		}

		int Index(lua_State* L);

	private:
		ObjectSet<eoc::ItemDefinition> definition_;
	};*/

	struct DealDamageEvent : public EventBase
	{
		bg3se::stats::DealDamageFunctor* Functor;
		EntityHandle Caster;
		EntityHandle Target;
		glm::vec3 Position;
		bool IsFromItem;
		SpellIdWithPrototype* SpellId;
		int StoryActionId;
		ActionOriginator* Originator;
		HitDesc* Hit;
		[[bg3::legacy(DamageSums)]] AttackDesc* Attack;
		uint64_t* UnknownThothParam;
		HitWith HitWith;
		EntityHandle Caster2;
		bg3se::SpellId* SpellId2;
	};

	struct DealtDamageEvent : public DealDamageEvent
	{
		HitResult* Result;
	};

	struct BeforeDealDamageEvent : public EventBase
	{
		HitDesc* Hit;
		[[bg3::legacy(DamageSums)]] AttackDesc* Attack;
	};

	struct ExecuteFunctorEvent : public EventBase
	{
		bg3se::stats::Functors* Functor;
		bg3se::stats::ContextData* Params;
	};

	struct AfterExecuteFunctorEvent : public EventBase
	{
		bg3se::stats::Functors* Functor;
		bg3se::stats::ContextData* Params;
		HitResult* Hit;
	};
	
	class FunctorEventHooks
	{
	public:
		FunctorEventHooks(lua::State& state);
		~FunctorEventHooks();

	private:
		lua::State& state_;

		HitResult* OnDealDamage(bg3se::stats::DealDamageFunctor::ApplyDamageProc* next, HitResult* result, bg3se::stats::DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
			ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId,
			int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr,
			HitDesc* hit, AttackDesc* attack, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
			bool entityDamagedEventParam, __int64 a17, SpellId* spellId2);
		void OnEntityDamageEvent(bg3se::stats::StatsSystem_ThrowDamageEventProc* next, void* statsSystem, void* temp5, 
			HitDesc* hit, AttackDesc* attack, bool a5, bool a6);

		template <class TParams>
		void LuaTriggerFunctorPreExecEvent(bg3se::stats::Functors* self, TParams* params)
		{
			ExecuteFunctorEvent evt{ 
				.Functor = self, 
				.Params = params 
			};
			state_.ThrowEvent("ExecuteFunctor", evt, false, 0);
		}

		template <class TParams>
		void LuaTriggerFunctorPostExecEvent(bg3se::stats::Functors* self, TParams* params, HitResult* hit)
		{
			AfterExecuteFunctorEvent evt{ 
				.Functor = self, 
				.Params = params, 
				.Hit = hit
			};
			state_.ThrowEvent("AfterExecuteFunctor", evt, false, 0);
		}

		template <class TParams>
		void OnFunctorExecute(bg3se::stats::ExecuteFunctorProc<TParams>* next, HitResult* hit, bg3se::stats::Functors* self, TParams* params)
		{
			LuaTriggerFunctorPreExecEvent<TParams>(self, params);
			next(hit, self, params);
			LuaTriggerFunctorPostExecEvent<TParams>(self, params, hit);
		}
	};


	class ExtensionLibraryServer : public ExtensionLibrary
	{
	public:
		void Register(lua_State * L) override;
		void RegisterLib(lua_State * L) override;
		STDString GenerateOsiHelpers();

	private:
		static char const * const NameResolverMetatableName;

		void RegisterNameResolverMetatable(lua_State * L);
		void CreateNameResolver(lua_State * L);

		static int LuaIndexResolverTable(lua_State* L);
	};

	class ServerState : public State
	{
	public:
		ServerState(ExtensionState& state, uint32_t generationId);
		~ServerState();

		void Initialize() override;
		bool IsClient() override;

		inline OsirisBinding& Osiris()
		{
			return osiris_;
		}

		void OnGameSessionLoading() override;
		void StoryFunctionMappingsUpdated();

		ecs::EntityWorld* GetEntityWorld() override;
		ecs::EntitySystemHelpersBase* GetEntitySystemHelpers() override;
		EntityReplicationEventHooks* GetReplicationEventHooks() override;

		template <class TArg>
		void Call(char const* mod, char const* func, std::vector<TArg> const & args)
		{
			auto L = GetState();
			LifetimeStackPin _(GetStack());
			lua_checkstack(L, (int)args.size() + 1);
			auto stackSize = lua_gettop(L);

			try {
				if (mod != nullptr) {
					PushModFunction(L, mod, func); // stack: func
				} else {
					lua_getglobal(L, func); // stack: func
				}

				for (auto & arg : args) {
					OsiToLua(L, arg); // stack: func, arg0 ... argn
				}

				auto status = CallWithTraceback(L, (int)args.size(), 0);
				if (status != LUA_OK) {
					LuaError("Failed to call function '" << func << "': " << lua_tostring(L, -1));
					// stack: errmsg
					lua_pop(L, 1); // stack: -
				}
			} catch (Exception &) {
				auto stackRemaining = lua_gettop(L) - stackSize;
				if (stackRemaining > 0) {
					if (mod != nullptr) {
						LuaError("Call to mod function '" << mod << "'.'" << func << "' failed: " << lua_tostring(L, -1));
					} else {
						LuaError("Call to mod function '" << func << "' failed: " << lua_tostring(L, -1));
					}
					lua_pop(L, stackRemaining);
				} else {
					if (mod != nullptr) {
						LuaError("Internal error during call to mod function '" << mod << "'.'" << func << "'");
					} else {
						LuaError("Internal error during call to mod function '" << func << "'");
					}
				}
			}
		}

		std::optional<STDString> GetModPersistentVars(STDString const& modTable);
		void RestoreModPersistentVars(STDString const& modTable, STDString const& vars);
		void OnGameStateChanged(GameState fromState, GameState toState);

		bool Query(char const* mod, char const* name, RegistryEntry * func,
			std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);

	private:
		ExtensionLibraryServer library_;
		OsirisBinding osiris_;
		FunctorEventHooks functorHooks_;
		EntityReplicationEventHooks replicationHooks_;

		bool QueryInternal(char const* mod, char const* name, RegistryEntry * func,
			std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);
	};
}
