#pragma once

#include <Lua/LuaBinding.h>
#if defined(ENABLE_UI)
#include <Lua/Client/UIEvents.h>
#endif

namespace bg3se::ecl::lua
{
	using namespace bg3se::lua;

	LifetimeHandle GetClientLifetime();
	LifetimePool& GetClientLifetimePool();

	struct GameStateChangedEvent : public EventBase
	{
		ecl::GameState FromState;
		ecl::GameState ToState;
	};

	class ExtensionLibraryClient : public ExtensionLibrary
	{
	public:
		void Register(lua_State * L) override;
		void RegisterLib(lua_State * L) override;
	};


	/*class StatusHandleProxy : public Userdata<StatusHandleProxy>, public Indexable, public NewIndexable<PushPolicy::None>
	{
	public:
		static char const* const MetatableName;

		inline StatusHandleProxy(ComponentHandle character, NetId status)
			: character_(character), statusNetId_(status)
		{}

		inline StatusHandleProxy(ComponentHandle character, ComponentHandle status)
			: character_(character), statusHandle_(status)
		{}

		ecl::Status* Get(lua_State* L);
		int Index(lua_State* L);
		int NewIndex(lua_State* L);

	private:
		ComponentHandle character_;
		ComponentHandle statusHandle_;
		NetId statusNetId_;
	};*/


	class ClientState : public State
	{
	public:
		ClientState(uint32_t generationId);
		~ClientState();

		void Initialize() override;
		bool IsClient() override;
		void OnUpdate(GameTime const& time) override;

		ecs::EntityWorld* GetEntityWorld() override;
		ecs::EntitySystemHelpersBase* GetEntitySystemHelpers() override;
		EntityReplicationEventHooks* GetReplicationEventHooks() override;
		void OnGameStateChanged(GameState fromState, GameState toState);

#if defined(ENABLE_UI)
		UIEventHooks& GetUIEvents()
		{
			return uiEvents_;
		}
#endif

	private:
		ExtensionLibraryClient library_;
#if defined(ENABLE_UI)
		UIEventHooks uiEvents_;
#endif
	};
}
