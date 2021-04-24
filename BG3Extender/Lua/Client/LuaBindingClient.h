#pragma once

#include <GameDefinitions/Enumerations.h>
#include <Lua/LuaBinding.h>

namespace bg3se::ecl::lua
{
	using namespace bg3se::lua;

	LifetimeHolder GetClientLifetime();
	LifetimePool& GetClientLifetimePool();

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

		inline StatusHandleProxy(ObjectHandle character, NetId status)
			: character_(character), statusNetId_(status)
		{}

		inline StatusHandleProxy(ObjectHandle character, ObjectHandle status)
			: character_(character), statusHandle_(status)
		{}

		ecl::Status* Get(lua_State* L);
		int Index(lua_State* L);
		int NewIndex(lua_State* L);

	private:
		ObjectHandle character_;
		ObjectHandle statusHandle_;
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
		void OnGameStateChanged(GameState fromState, GameState toState);

	private:
		ExtensionLibraryClient library_;
	};
}
