#pragma once

#include <GameDefinitions/Enumerations.h>
#include <Lua/LuaBinding.h>

namespace bg3se::ecl::lua
{
	using namespace bg3se::lua;

	LifetimeHolder GetClientLifetime();
	LifetimePool& GetClientLifetimePool();

	struct GameStateChangeEventParams
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


	/*class StatusHandleProxy : public Userdata<StatusHandleProxy>, public Indexable, public NewIndexable, public Pushable<PushPolicy::None>
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
		ClientState();
		~ClientState();

		/*std::optional<STDWString> SkillGetDescriptionParam(SkillPrototype * prototype,
			CDivinityStats_Character * character, ObjectSet<STDString> const & paramTexts, bool isFromItem);
		std::optional<STDWString> StatusGetDescriptionParam(StatusPrototype * prototype, CRPGStats_ObjectInstance* owner,
			CRPGStats_ObjectInstance* statusSource, ObjectSet<STDString> const & paramTexts);*/

		EntityWorldBase* GetEntityWorld() override;
		EntitySystemHelpersBase* GetEntitySystemHelpers() override;
		void OnGameStateChanged(GameState fromState, GameState toState);

	private:
		ExtensionLibraryClient library_;
	};
}
