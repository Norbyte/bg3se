#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/Proxies/LuaObjectProxy.h>
#include <Lua/Shared/Proxies/LuaArrayProxy.h>
#include <Lua/Shared/Proxies/LuaSetProxy.h>
#include <Lua/Shared/Proxies/LuaMapProxy.h>
#include <Lua/Shared/LuaEvent.h>
#include <Lua/Shared/Proxies/LuaEntityProxy.h>

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
	LifetimeHandle GetCurrentLifetime();

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
		static_assert(LUA_EXTRASPACE >= sizeof(State*), "Should have enough extra space in Lua for state ptr");

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

		State(bool isServer);
		~State();

		State(State const &) = delete;
		State(State &&) = delete;
		State & operator = (State const &) = delete;
		State & operator = (State &&) = delete;

		static State* FromLua(lua_State* L);

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

		LifetimeHandle GetCurrentLifetime();

		inline LifetimeHandle GetGlobalLifetime()
		{
			return globalLifetime_;
		}

		inline LifetimePool& GetLifetimePool()
		{
			return lifetimePool_;
		}

		virtual void Initialize() = 0;
		virtual void Shutdown();
		virtual bool IsClient() = 0;

		virtual EntityWorld* GetEntityWorld() = 0;
		virtual EntitySystemHelpersBase* GetEntitySystemHelpers() = 0;

		void FinishStartup();
		void LoadBootstrap(STDString const& path, STDString const& modTable);
		virtual void OnGameSessionLoading();
		void OnGameSessionLoaded();
		void OnModuleLoadStarted();
		void OnModuleLoading();
		void OnStatsLoaded();
		void OnModuleResume();
		void OnLevelLoading();
		void OnResetCompleted();
		virtual void OnUpdate(GameTime const& time);
		void OnStatsStructureLoaded();

		template <class... Ret, class... Args>
		bool CallExtRet(char const * func, uint32_t restrictions, std::tuple<Ret...>& ret, Args... args)
		{
			StackCheck _(L, sizeof...(Ret));
			// FIXME - Restriction restriction(*this, restrictions);
			LifetimeStackPin _p(lifetimeStack_);
			auto lifetime = lifetimeStack_.GetCurrent();
			PushInternalFunction(L, func);
			(push_proxy(L, lifetime, args), ...);
			return CheckedCall<Ret...>(L, sizeof...(args), ret, func);
		}

		template <class... Args>
		bool CallExt(char const * func, uint32_t restrictions, Args... args)
		{
			StackCheck _(L, 0);
			// FIXME - Restriction restriction(*this, restrictions);
			LifetimeStackPin _p(lifetimeStack_);
			auto lifetime = lifetimeStack_.GetCurrent();
			PushInternalFunction(L, func);
			(push_proxy(L, lifetime, args), ...);
			return CheckedCall(L, sizeof...(args), func);
		}

		template <class TEvent, class TReadWrite>
		bool ThrowEvent(char const* eventName, TEvent& evt, bool canPreventAction, uint32_t restrictions, TReadWrite)
		{
			auto stackSize = lua_gettop(L);

			try {
				StackCheck _(L, 0);
				// FIXME - Restriction restriction(*this, restrictions);
				LifetimeStackPin _p(lifetimeStack_);
				PushInternalFunction(L, "_ThrowEvent");
				EventObject::Make(L, _p.GetLifetime(), eventName, evt, canPreventAction, TReadWrite{});
				return CheckedCall(L, 1, "_ThrowEvent");
			} catch (Exception &) {
				auto stackRemaining = lua_gettop(L) - stackSize;
				if (stackRemaining > 0) {
					LuaError("Failed to dispatch event '" << eventName << "': " << lua_tostring(L, -1));
					lua_pop(L, stackRemaining);
				} else {
					LuaError("Internal error while dispatching event '" << eventName << "'");
				}

				return false;
			}
		}

		std::optional<int> LoadScript(STDString const & script, STDString const & name = "", int globalsIdx = 0);

		/*void OnNetMessageReceived(STDString const & channel, STDString const & payload, UserId userId);*/

		static STDString GetBuiltinLibrary(int resourceId);

	protected:
		lua_State * L;
		bool startupDone_{ false };

		LifetimePool lifetimePool_;
		LifetimeStack lifetimeStack_;
		LifetimeHandle globalLifetime_;

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

	struct DoConsoleCommandEventParams
	{
		STDString Command;
	};

	struct TickEventParams
	{
		GameTime Time;
	};
}

namespace bg3se::lua::stats
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, FixedString const& attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, FixedString const& attributeName, int valueIdx);
}