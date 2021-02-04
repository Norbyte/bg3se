#include <stdafx.h>
#include <OsirisProxy.h>
#include <PropertyMaps.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);
}

#include <Lua/Shared/LuaCharacter.inl>
#include <Lua/Shared/LuaItem.inl>
#include <Lua/Shared/StatEntries.inl>
#include <Lua/Shared/StatEnumerators.inl>
#include <Lua/Shared/StatAttributes.inl>
#include <Lua/Shared/StatMisc.inl>

#include <Lua/Shared/RootTemplates.inl>
