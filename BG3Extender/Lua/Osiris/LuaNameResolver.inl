#include <Lua/Osiris/FunctionProxy.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

char const * const ExtensionLibraryServer::NameResolverMetatableName = "OsiProxyNameResolver";

int LuaIndexResolverTable(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	auto name = get<FixedString>(L, 2);

	auto state = ServerState::FromLua(L);
	auto& resolver = state->Osiris().GetNameResolver();
	auto index = resolver.GetNameIndex(name);
	if (!index) {
		index = resolver.GetLegacyNameIndex(name);
		if (index) {
			WARN("COMPATIBILITY WARNING: Osiris symbol '%s' referenced using incorrect case; the correct name is '%s'",
				name.GetString(), resolver.GetCache(*index)->name.GetString());
		}
	}

	if (index) {
		OsiFunctionNameMetatable::Make(L, *index);

		lua_pushvalue(L, 1); // stack: fun, tab
		push(L, name); // stack: fun, tab, name
		lua_pushvalue(L, -3); // stack: fun, tab, name, fun
		lua_rawset(L, -3); // stack: fun
		lua_pop(L, 1);
	} else {
		push(L, nullptr);
	}

	return 1;
}

void ExtensionLibraryServer::RegisterNameResolverMetatable(lua_State * L)
{
	lua_register(L, NameResolverMetatableName, nullptr);
	luaL_newmetatable(L, NameResolverMetatableName); // stack: mt
	lua_pushcfunction(L, &LuaIndexResolverTable); // stack: mt, &LuaIndexResolverTable
	lua_setfield(L, -2, "__index"); // mt.__index = &LuaIndexResolverTable; stack: mt
	lua_pop(L, 1); // stack: mt
}

void ExtensionLibraryServer::CreateNameResolver(lua_State * L)
{
	lua_newtable(L); // stack: osi
	luaL_setmetatable(L, NameResolverMetatableName); // stack: osi
	lua_setglobal(L, "Osi"); // stack: -
}


STDString ExtensionLibraryServer::GenerateOsiHelpers()
{
	std::stringstream ss;

	auto const& sigs = gExtender->GetServer().Osiris().GetCustomFunctionInjector().OsiSymbols();
	for (auto const & sig : sigs) {
		if (sig.EoCFunctionId != 0 && sig.params.size() <= 16) {
			ss << sig.name << " = Osi." << sig.name << "\r\n";
		}
	}

	return STDString(ss.str());
}

END_NS()
