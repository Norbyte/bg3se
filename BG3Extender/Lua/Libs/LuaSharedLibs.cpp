#include <stdafx.h>
#include <Lua/Libs/LibraryRegistrationHelpers.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Libs/Debug.inl>
#include <Lua/Libs/IO.inl>
#include <Lua/Libs/Json.inl>
#include <Lua/Libs/Math.inl>
#include <Lua/Libs/Mod.inl>
#include <Lua/Libs/Stats.inl>
#include <Lua/Libs/StatAttributes.inl>
#include <Lua/Libs/StatMisc.inl>
#include <Lua/Libs/Types.inl>
#include <Lua/Libs/Utils.inl>

BEGIN_NS(ecl::lua)

void RegisterClientLibraries()
{
	//ecs::RegisterEntityLib();
}

END_NS()

BEGIN_NS(esv::lua)

void RegisterServerLibraries()
{
	//ecs::RegisterEntityLib();
}

END_NS()

BEGIN_NS(lua)

void InitObjectProxyPropertyMaps();

void RegisterSharedMetatables(lua_State* L)
{
	Userdata<ObjectProxy>::RegisterMetatable(L);
	Userdata<ArrayProxy>::RegisterMetatable(L);
	Userdata<SetProxy>::RegisterMetatable(L);
	Userdata<MapProxy>::RegisterMetatable(L);
	InitObjectProxyPropertyMaps();
	RegisterEntityProxy(L);
	stats::StatsExtraDataProxy::RegisterMetatable(L);
	stats::StatsProxy::RegisterMetatable(L);
	stats::SpellPrototypeProxy::RegisterMetatable(L);
	types::RegisterEnumerations(L);
}

void RegisterSharedLibraries()
{
	utils::RegisterUtilsLib();
	json::RegisterJsonLib();
	types::RegisterTypesLib();
	io::RegisterIOLib();
	math::RegisterMathLib();
	mod::RegisterModLib();
	//loca::RegisterLocalizationLib();
	debug::RegisterDebugLib();
	stats::RegisterStatsLib();
}

void RegisterLibraries()
{
	if (!gModuleRegistry.GetModules().empty()) return;

	RegisterSharedLibraries();
	esv::lua::RegisterServerLibraries();
	ecl::lua::RegisterClientLibraries();
}

END_NS()
