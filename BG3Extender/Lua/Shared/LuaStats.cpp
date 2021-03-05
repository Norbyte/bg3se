#include <stdafx.h>
#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua::stats
{
	// TODO - remove?
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);
}

#include <Lua/Shared/LuaCharacter.inl>
#include <Lua/Shared/LuaItem.inl>
#include <Lua/Shared/StatEntries.inl>
#include <Lua/Shared/StatEnumerators.inl>
#include <Lua/Shared/StatAttributes.inl>
#include <Lua/Shared/StatMisc.inl>
#include <Lua/Shared/LuaObjectProxies.inl>

#include <Lua/Shared/RootTemplates.inl>


namespace bg3se::lua::stats
{
	void RegisterStatsLib(lua_State* L)
	{
		static const luaL_Reg statsLib[] = {
			{"GetAllStats", GetAllStats},
			{"GetStatsLoadedBefore", GetStatsLoadedBefore},

			{"GetStat", GetStat},
			{"CreateStat", CreateStat},
			{"SyncStat", SyncStatWrapper},
			{"StatSetPersistence", StatSetPersistenceWrapper},

			{"GetModifierAttributes", GetModifierAttributes},
			{"EnumIndexToLabel", EnumIndexToLabel},
			{"EnumLabelToIndex", EnumLabelToIndex},

			{"UpdateTreasureTable", UpdateTreasureTable},
			{"UpdateTreasureCategory", UpdateTreasureCategory},

			/*{"ExecuteSkillPropertiesOnTarget", ExecuteSkillPropertiesOnTarget},
			{"ExecuteSkillPropertiesOnPosition", ExecuteSkillPropertiesOnPosition},*/
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, statsLib); // stack: ext, lib
		lua_setfield(L, -2, "Stats");
		lua_pop(L, 1);

		RegisterStatsObjects(L);
	}
}

namespace bg3se::lua
{
	void RegisterSharedLibraries(lua_State* L)
	{
		RegisterObjectProxies(L);
		InitObjectProxyPropertyMaps(L);

		stats::RegisterStatsLib(L);
		utils::RegisterStaticDataLib(L);
		utils::RegisterUtilsLib(L);
		utils::RegisterLocalizationLib(L);
		utils::RegisterJsonLib(L);
		utils::RegisterIOLib(L);
		utils::RegisterMathLib(L);
	}
}
