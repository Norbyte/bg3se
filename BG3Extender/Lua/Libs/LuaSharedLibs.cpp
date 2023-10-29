#include <stdafx.h>
#include <Lua/Libs/LibraryRegistrationHelpers.h>
#include <Lua/Shared/LuaModule.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Libs/Debug.inl>
#include <Lua/Libs/Entity.inl>
#include <Lua/Libs/IO.inl>
#include <Lua/Libs/Json.inl>
#include <Lua/Libs/Localization.inl>
#include <Lua/Libs/Math.inl>
#include <Lua/Libs/Mod.inl>
#include <Lua/Libs/StatAttributes.inl>
#include <Lua/Libs/StatMisc.inl>
#include <Lua/Libs/Stats.inl>
#include <Lua/Libs/StaticData.inl>
#include <Lua/Libs/Types.inl>
#include <Lua/Libs/Utils.inl>
#include <Lua/Libs/Vars.inl>
#include <Lua/Libs/ClientNet.inl>
#include <Lua/Libs/ServerNet.inl>

BEGIN_NS(ecl::lua)

void RegisterClientLibraries()
{
	net::RegisterNetLib();
}

END_NS()

BEGIN_NS(esv::lua)

void RegisterServerLibraries()
{
	net::RegisterNetLib();
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
	entity::RegisterEntityLib();
	json::RegisterJsonLib();
	types::RegisterTypesLib();
	io::RegisterIOLib();
	loca::RegisterLocalizationLib();
	math::RegisterMathLib();
	mod::RegisterModLib();
	debug::RegisterDebugLib();
	stats::RegisterStatsLib();
	res::RegisterStaticDataLib();
	vars::RegisterVarsLib();
}

void RegisterLibraries()
{
	if (!gModuleRegistry.GetModules().empty()) return;

	RegisterSharedLibraries();
	esv::lua::RegisterServerLibraries();
	ecl::lua::RegisterClientLibraries();
}

END_NS()
