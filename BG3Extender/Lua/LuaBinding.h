#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/Proxies/LuaObjectProxy.h>
#include <Lua/Shared/Proxies/LuaEvent.h>
#include <Lua/Shared/Proxies/LuaEntityProxy.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
#include <Lua/Shared/Proxies/LuaCppClass.h>
#include <Lua/Shared/Proxies/LuaCppObjectProxy.h>
#include <Lua/Shared/Proxies/LuaCppValue.h>
#include <Lua/Shared/Proxies/LuaEnumValue.h>
#include <Lua/Shared/Proxies/LuaBitfieldValue.h>
#include <Lua/Shared/Proxies/LuaUserVariableHolder.h>
#include <Extender/Shared/UserVariables.h>

#include <mutex>
#include <unordered_set>
#include <optional>

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

		State(uint32_t generationId, bool isServer);
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

		inline uint32_t GetGenerationId()
		{
			return generationId_;
		}

		inline LuaInternalState* GetInternalState()
		{
			return internal_;
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

		inline CppMetatableManager& GetMetatableManager()
		{
			return metatableManager_;
		}

		inline CachedUserVariableManager& GetVariableManager()
		{
			return variableManager_;
		}

		inline CachedModVariableManager& GetModVariableManager()
		{
			return modVariableManager_;
		}

		virtual void Initialize() = 0;
		virtual void Shutdown();
		virtual bool IsClient() = 0;

		virtual ecs::EntityWorld* GetEntityWorld() = 0;
		virtual ecs::EntitySystemHelpersBase* GetEntitySystemHelpers() = 0;
		virtual EntityEventHooks* GetEntityEventHooks() = 0;

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
		void OnNetMessageReceived(STDString const& channel, STDString const& payload, UserId userId);

		template <class... Ret, class... Args>
		bool CallExtRet(char const * func, uint32_t restrictions, std::tuple<Ret...>& ret, Args... args)
		{
			StackCheck _(L);
			// FIXME - Restriction restriction(*this, restrictions);
			LifetimeStackPin _p(lifetimeStack_);
			auto lifetime = lifetimeStack_.GetCurrent();
			PushInternalFunction(L, func);
			(push(L, args, lifetime), ...);
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
			(push(L, args, lifetime), ...);
			return CheckedCall(L, sizeof...(args), func);
		}

		template <class TEvent>
		EventResult ThrowEvent(char const* eventName, TEvent& evt, bool canPreventAction = false, uint32_t restrictions = 0)
		{
			static_assert(std::is_base_of_v<EventBase, TEvent>, "Event object must be a descendant of EventBase");
			StackCheck _(L, 0);
			LifetimeStackPin _p(GetStack());
			PushInternalFunction(L, "_ThrowEvent");
			MakeObjectRef(L, &evt);
			return DispatchEvent(evt, eventName, canPreventAction, restrictions);
		}

		std::optional<int> LoadScript(STDString const & script, STDString const & name = "", int globalsIdx = 0);

		/*void OnNetMessageReceived(STDString const & channel, STDString const & payload, UserId userId);*/

		static STDString GetBuiltinLibrary(int resourceId);

	protected:
		lua_State * L;
		LuaInternalState* internal_{ nullptr };
		bool startupDone_{ false };
		uint32_t generationId_;

		LifetimePool lifetimePool_;
		LifetimeStack lifetimeStack_;
		LifetimeHandle globalLifetime_;

		CppMetatableManager metatableManager_;

		CachedUserVariableManager variableManager_;
		CachedModVariableManager modVariableManager_;

		void OpenLibs();
		EventResult DispatchEvent(EventBase& evt, char const* eventName, bool canPreventAction, uint32_t restrictions);
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

	struct DoConsoleCommandEvent : public EventBase
	{
		STDString Command;
	};

	struct TickEvent : public EventBase
	{
		GameTime Time;
	};

	struct NetMessageEvent : public EventBase
	{
		STDString Channel;
		STDString Payload;
		UserId UserID;
	};
}

namespace bg3se::lua::stats
{
	int LuaStatGetAttribute(lua_State* L, bg3se::stats::Object* object, FixedString const& attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, bg3se::stats::Object* object, FixedString const& attributeName, int valueIdx);
}
