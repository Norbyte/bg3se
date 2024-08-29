#include <Lua/Osiris/FunctionProxy.h>

BEGIN_NS(esv::lua)

int OsiFunctionNameMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	auto prop = get<FixedString>(L, 2);
	if (prop == GFS.strGet) {
		push(L, &LuaGet);
	} else if (prop == GFS.strDelete) {
		push(L, &LuaDelete);
	} else if (prop == GFS.strExists) {
		push(L, &LuaExists);
	} else if (prop == GFS.strType) {
		push(L, &LuaType);
	} else if (prop == GFS.strInputArities) {
		GetInputArities(L, self);
	} else if (prop == GFS.strArities) {
		GetArities(L, self);
	} else {
		luaL_error(L, "Not a valid OsiFunction method or property: %s", prop.GetString());
	}

	return 1;
}


int OsiFunctionNameMetatable::Call(lua_State* L, CppValueMetadata& self)
{
	auto cache = Get(L, self);

	auto func = cache->GetFunction(lua_gettop(L) - 1);
	if (func) {
		return func->LuaCall(L);
	} else {
		return luaL_error(L, "No function named '%s' exists that can be called with %d parameters.", 
			cache->name.GetString(), lua_gettop(L) - 1);
	}
}


int OsiFunctionNameMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	char name[200];
	sprintf_s(name, "OsiFunction(%s)", Get(L, self)->name.GetString());
	push(L, name);
	return 1;
}


char const* OsiFunctionNameMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	return "OsiFunctionName";
}


int OsiFunctionNameMetatable::LuaGet(lua_State* L)
{
	auto arity = lua_gettop(L) - 1;
	auto cache = Get(L);

	auto func = cache->GetFunction(arity);
	if (func == nullptr) {
		return luaL_error(L, "No database named '%s(%d)' exists",
			cache->name.GetString(), arity);
	}

	if (!func->IsDB()) {
		return luaL_error(L, "'%s(%d)' is not a database", cache->name.GetString(), arity);
	}

	return func->LuaGet(L);
}


int OsiFunctionNameMetatable::LuaDelete(lua_State* L)
{
	auto arity = lua_gettop(L) - 1;
	auto cache = Get(L);

	auto func = cache->GetFunction(arity);
	if (func == nullptr) {
		return luaL_error(L, "No database named '%s(%d)' exists",
			cache->name.GetString(), arity);
	}

	if (!func->IsDB()) {
		return luaL_error(L, "'%s(%d)' is not a database", cache->name.GetString(), arity);
	}

	return func->LuaDelete(L);
}


int OsiFunctionNameMetatable::LuaExists(lua_State* L)
{
	int arity = get<int>(L, 2);
	auto func = Get(L)->GetFunction(arity);
	push(L, func != nullptr);
	return 1;
}


int OsiFunctionNameMetatable::LuaType(lua_State* L)
{
	int arity = get<int>(L, 2);
	auto func = Get(L)->GetFunction(arity);
	if (func == nullptr || !func->IsBound()) {
		push(L, nullptr);
	} else {
		OsiFunctionType type{ OsiFunctionType::Call };
		switch (func->GetOsiFunction()->Type)
		{
		case FunctionType::Event:

		case FunctionType::Query:
		case FunctionType::SysQuery:
		case FunctionType::UserQuery:
			type = OsiFunctionType::Query;
			break;

		case FunctionType::Call:
		case FunctionType::SysCall:
			type = OsiFunctionType::Call;
			break;

		case FunctionType::Database:
			type = OsiFunctionType::DB;
			break;

		case FunctionType::Proc:
			type = OsiFunctionType::Proc;
			break;
		}

		push(L, type);
	}

	return 1;
}


void OsiFunctionNameMetatable::GetInputArities(lua_State* L, CppValueMetadata& self)
{
	auto cache = Get(L, self);

	lua_newtable(L);
	auto i = 1;

	for (unsigned arity = 0; arity < cache->inputArgsToArity.size(); arity++) {
		if (cache->inputArgsToArity[arity]) {
			settable(L, i++, arity);
		}
	}
}

void OsiFunctionNameMetatable::GetArities(lua_State* L, CppValueMetadata& self)
{
	auto cache = Get(L, self);

	lua_newtable(L);
	auto i = 1;

	for (unsigned arity = 0; arity < cache->namesByArity.size(); arity++) {
		if (cache->namesByArity[arity].IsBound()) {
			settable(L, i++, arity);
		}
	}
}


END_NS()
