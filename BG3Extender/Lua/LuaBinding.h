#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/LuaObjectProxy.h>
#include <Lua/Shared/LuaArrayProxy.h>
#include <Lua/Shared/LuaSetProxy.h>
#include <Lua/Shared/LuaMapProxy.h>
#include <Lua/Shared/LuaEntityProxy.h>

#include <mutex>
#include <unordered_set>
#include <optional>

namespace bg3se
{
	struct CRPGStats_Object;
	class EntitySystemHelpersBase;
}

namespace bg3se::lua
{
	void PushExtFunction(lua_State * L, char const * func);
	void PushInternalFunction(lua_State * L, char const * func);
	void PushModFunction(lua_State* L, char const* mod, char const* func);
	LifetimeHolder GetCurrentLifetime();

	class ExtensionLibrary
	{
	public:
		virtual void Register(lua_State * L);
		virtual void RegisterLib(lua_State * L) = 0;
	};

	class Exception : public std::runtime_error
	{
	public:
		Exception(std::string const& msg)
			: std::runtime_error("Lua error thrown: " + msg)
		{}
	};

	class State : Noncopyable<State>
	{
	public:
		enum RestrictionFlag
		{
			// Disable Osiris calls
			RestrictOsiris = 1 << 0,
			// Disable handle/guid to object conversion functions (Lua only)
			RestrictHandleConversion = 1 << 1,

			// Permit calls only available during module load state
			ScopeModuleLoad = 1 << 16,
			// Permit calls only available during session load state
			ScopeSessionLoad = 1 << 17,
			// Permit calls only available during module resume state
			ScopeModuleResume = 1 << 18,
			// Permit calls only available during initial module load state (before loading any files)
			ScopeModulePreLoad = 1 << 19,

			RestrictAll = 0x0000ffff,
		};

		uint32_t RestrictionFlags{ 0 };
		std::unordered_set<int32_t> OverriddenLevelMaps;

		State();
		~State();

		State(State const &) = delete;
		State(State &&) = delete;
		State & operator = (State const &) = delete;
		State & operator = (State &&) = delete;

		inline lua_State * GetState()
		{
			return L;
		}

		inline LifetimeStack & GetStack()
		{
			return lifetimeStack_;
		}

		inline bool StartupDone() const
		{
			return startupDone_;
		}

		LifetimeHolder GetCurrentLifetime();

		inline LifetimePool& GetLifetimePool()
		{
			return lifetimePool_;
		}

		virtual EntityWorldBase* GetEntityWorld() = 0;
		virtual EntitySystemHelpersBase* GetEntitySystemHelpers() = 0;

		void FinishStartup();
		void LoadBootstrap(STDString const& path, STDString const& modTable);
		virtual void OnGameSessionLoading();
		void OnGameSessionLoaded();
		void OnModuleLoadStarted();
		void OnModuleLoading();
		void OnStatsLoaded();
		void OnModuleResume();
		void OnResetCompleted();

		template <class... Ret, class... Args>
		bool CallExtRet(char const * func, uint32_t restrictions, std::tuple<Ret...>& ret, Args... args)
		{
			StackCheck _(L, sizeof...(Ret));
			Restriction restriction(*this, restrictions);
			LifetimePin _p(lifetimeStack_);
			auto lifetime = lifetimeStack_.GetCurrent();
			PushInternalFunction(L, func);
			(push_proxy(L, lifetime, args), ...);
			return CheckedCall<Ret...>(L, sizeof...(args), ret, func);
		}

		template <class... Args>
		bool CallExt(char const * func, uint32_t restrictions, Args... args)
		{
			StackCheck _(L, 0);
			Restriction restriction(*this, restrictions);
			LifetimePin _p(lifetimeStack_);
			auto lifetime = lifetimeStack_.GetCurrent();
			PushInternalFunction(L, func);
			(push_proxy(L, lifetime, args), ...);
			return CheckedCall(L, sizeof...(args), func);
		}

		std::optional<int> LoadScript(STDString const & script, STDString const & name = "", int globalsIdx = 0);

		/*std::optional<int32_t> GetHitChance(CDivinityStats_Character * attacker, CDivinityStats_Character * target);
		bool GetSkillDamage(SkillPrototype * self, DamagePairList * damageList,
			CRPGStats_ObjectInstance *attackerStats, bool isFromItem, bool stealthed, float * attackerPosition,
			float * targetPosition, DeathType * pDeathType, int level, bool noRandomization);
		std::optional<std::pair<int, bool>> GetSkillAPCost(SkillPrototype* skill, CDivinityStats_Character* character, eoc::AiGrid* aiGrid,
			glm::vec3* position, float* radius);
		void OnNetMessageReceived(STDString const & channel, STDString const & payload, UserId userId);*/

		static STDString GetBuiltinLibrary(int resourceId);
		static State* FromLua(lua_State* L);

	protected:
		lua_State * L;
		bool startupDone_{ false };

		LifetimePool lifetimePool_;
		LifetimeStack lifetimeStack_;

		void OpenLibs();
	};

	class Restriction
	{
	public:
		inline Restriction(State & state, uint32_t flags)
			: state_(state), oldFlags_(state_.RestrictionFlags)
		{
			state_.RestrictionFlags |= flags;
		}

		inline ~Restriction()
		{
			state_.RestrictionFlags = oldFlags_;
		}

	private:
		State & state_;
		uint32_t oldFlags_;
	};

	int GetCellInfo(lua_State* L);

	/*int NewDamageList(lua_State* L);
	int GetSurfaceTemplate(lua_State* L);*/
}

namespace bg3se::lua::stats
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);
}