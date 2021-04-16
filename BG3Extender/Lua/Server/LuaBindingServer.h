#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/Server/ServerOsirisBinding.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Status.h>
#include <ExtensionHelpers.h>

namespace bg3se::esv
{
	struct PendingHit;
	class ExtensionState;
}

namespace bg3se::esv::lua
{
	using namespace ::bg3se::lua;

	LifetimeHolder GetServerLifetime();

	class StatusHandleProxy : public Userdata<StatusHandleProxy>, public Indexable, public NewIndexable, public Pushable
	{
	public:
		static char const * const MetatableName;

		inline StatusHandleProxy(ObjectHandle character, ObjectHandle status)
			: character_(character), statusHandle_(status)
		{}

		inline StatusHandleProxy(ObjectHandle character, NetId status)
			: character_(character), statusNetId_(status)
		{}

		int Index(lua_State * L);
		int NewIndex(lua_State * L);
		esv::Status* Get(lua_State* L);

	private:
		ObjectHandle character_;
		ObjectHandle statusHandle_;
		NetId statusNetId_;
	};


	/*class TurnManagerCombatProxy : public Userdata<TurnManagerCombatProxy>, public Indexable, public Pushable<PushPolicy::None>
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

	class TurnManagerTeamProxy : public Userdata<TurnManagerTeamProxy>, public Indexable, public Pushable<PushPolicy::None>
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


	class ItemConstructor : public Userdata<ItemConstructor>, public Indexable, public Pushable<PushPolicy::None>
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
	
	class FunctorEventHooks
	{
	public:
		FunctorEventHooks(lua::State& state);
		~FunctorEventHooks();

	private:
		lua::State& state_;

		NewHit* OnDealDamage(DealDamageFunctor::ApplyDamageProc* next, NewHit* result, DealDamageFunctor* functor, EntityWorldHandle* casterHandle,
			EntityWorldHandle* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, int storyActionId,
			ActionOriginator* originator, GuidResourceDefinitionManagerBase* classResourceMgr, Hit* hit, DamageSums* damageSums, HitWith hitWith);

		template <class TParams>
		void LuaTriggerFunctorExecEvent(StatsFunctorSet* self, TParams* params)
		{
			// FIXME - ObjectProxy2<StatsFunctorSet>::New(L, self); instead of nullptr!
			state_.CallExt<>("_OnExecuteFunctor", 0, nullptr, params);
		}

		template <class TParams, class TNext>
		void OnFunctorExecute(TNext* next, Hit* hit, StatsFunctorSet* self, TParams* params)
		{
			LuaTriggerFunctorExecEvent<TParams>(self, params);
			next(hit, self, params);
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
		ServerState(ExtensionState& state);
		~ServerState();

		ServerState(ServerState const &) = delete;
		ServerState(ServerState &&) = delete;
		ServerState & operator = (ServerState const &) = delete;
		ServerState & operator = (ServerState &&) = delete;

		inline uint32_t GenerationId() const
		{
			return generationId_;
		}

		inline IdentityAdapterMap & GetIdentityAdapterMap()
		{
			return identityAdapters_;
		}

		inline OsiArgumentPool<OsiArgumentDesc> & GetArgumentDescPool()
		{
			return argDescPool_;
		}

		inline OsiArgumentPool<TypedValue> & GetTypedValuePool()
		{
			return tvPool_;
		}

		inline OsiArgumentPool<ListNode<TypedValue *>> & GetTypedValueNodePool()
		{
			return tvNodePool_;
		}

		inline OsiArgumentPool<ListNode<TupleLL::Item>> & GetTupleNodePool()
		{
			return tupleNodePool_;
		}

		inline OsirisCallbackManager& GetOsirisCallbacks()
		{
			return osirisCallbacks_;
		}

		void OnGameSessionLoading() override;

		void StoryLoaded();
		void StoryFunctionMappingsUpdated();
		void StorySetMerging(bool isMerging);

		template <class TArg>
		void Call(char const* mod, char const* func, std::vector<TArg> const & args)
		{
			auto L = GetState();
			LifetimePin _(GetStack());
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

		bool Query(char const* mod, char const* name, RegistryEntry * func,
			std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);

		std::optional<STDString> GetModPersistentVars(STDString const& modTable);
		void RestoreModPersistentVars(STDString const& modTable, STDString const& vars);
		void OnGameStateChanged(GameState fromState, GameState toState);

	private:
		ExtensionLibraryServer library_;
		OsiArgumentPool<OsiArgumentDesc> argDescPool_;
		OsiArgumentPool<TypedValue> tvPool_;
		OsiArgumentPool<ListNode<TypedValue *>> tvNodePool_;
		OsiArgumentPool<ListNode<TupleLL::Item>> tupleNodePool_;
		IdentityAdapterMap identityAdapters_;
		// ID of current story instance.
		// Used to invalidate function/node pointers in Lua userdata objects
		uint32_t generationId_{ 0 };
		OsirisCallbackManager osirisCallbacks_;
		FunctorEventHooks functorHooks_;

		bool QueryInternal(char const* mod, char const* name, RegistryEntry * func,
			std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params);
	};
}
