#include <stdafx.h>
#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/LuaBinding.h>
#include <Extender/ScriptExtender.h>

#include <cstdint>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <lobject.h>
#include <lstate.h>
#include <lstring.h>
#include <lapi.h>

BEGIN_NS(lua)

Lifetime* LifetimeHandle::GetLifetime(lua_State* L) const
{
	auto& pool = State::FromLua(L)->GetLifetimePool();
	return pool.Get(*this);
}

LifetimeStackPin::LifetimeStackPin(lua_State* L)
	: LifetimeStackPin(State::FromLua(L)->GetStack())
{}

StaticLifetimeStackPin::StaticLifetimeStackPin(lua_State* L, LifetimeHandle lifetime)
	: StaticLifetimeStackPin(State::FromLua(L)->GetStack(), lifetime)
{}

LifetimeOwnerPin::LifetimeOwnerPin(lua_State* L)
	: LifetimeOwnerPin(State::FromLua(L)->GetLifetimePool())
{}

LifetimeOwnerPin::LifetimeOwnerPin(lua_State* L, LifetimeHandle const& lifetime)
	: LifetimeOwnerPin(State::FromLua(L)->GetLifetimePool(), lifetime)
{}

TValue* lua_index2addr(lua_State* L, int idx)
{
	CallInfo* ci = L->ci;
	if (idx > 0 && idx <= L->top - (ci->func + 1)) {
		TValue* o = ci->func + idx;
		if (o >= L->top) {
			luaL_error(L, "Invalid stack index: %d", idx);
			return nullptr;
		} else {
			return o;
		}
	} else if (idx >= 0) {
		luaL_error(L, "Param #%d missing", idx);
		return nullptr;
	} else {
		luaL_error(L, "Non-absolute stack indices not supported: %d", idx);
		return nullptr;
	}
}
	
TValue* lua_get_top_addr(lua_State* L)
{
	return L->top - 1;
}
	
Table* lua_get_top_table_unsafe(lua_State* L)
{
	return hvalue(L->top - 1);
}

uint32_t get_generation_id(lua_State* L)
{
	return State::FromLua(L)->GetGenerationId();
}

/*
bool ProtectedMethodCallerBase::ProtectedCall(lua_State* L, lua_CFunction fun)
{
	StackCheck _(L);
	auto ret = CallUserFunctionWithTraceback(L, fun);
	if (ret == -1) {
		return false;
	} else if (ret != LUA_OK) {
		ERR("Error while dispatching user method call '%s': %s", Method, lua_tostring(L, -1));
		lua_pop(L, 1);
		return false; 
	} else {
		return true;
	}
}

int ProtectedMethodCallerBase::CallUserFunctionWithTraceback(lua_State* L, lua_CFunction fun)
{
	lua_pushcfunction(L, &TracebackHandler);
	int tracebackHandlerIdx = lua_gettop(L);
	lua_pushcfunction(L, fun);
	lua_pushlightuserdata(L, this);
	Self.Push(L);
	int status = lua_pcall(L, 2, 0, tracebackHandlerIdx);
	lua_remove(L, tracebackHandlerIdx);
	return status;
}
*/

bool ProtectedFunctionCallerBase::ProtectedCall(lua_State* L, lua_CFunction fun, char const* funcDescription)
{
	StackCheck _(L);
	EnterVMCheck(L);
	auto ret = CallUserFunction(L, fun);
	if (ret != LUA_OK) {
		if (funcDescription) {
			ERR("Error while dispatching user function call for %s: %s", funcDescription, lua_tostring(L, -1));
		} else {
			ERR("Error while dispatching user function call: %s", lua_tostring(L, -1));
		}
		lua_pop(L, 1);
		return false; 
	} else {
		return true;
	}
}

int ProtectedFunctionCallerBase::CallUserFunction(lua_State* L, lua_CFunction fun)
{
	lua_pushcfunction(L, fun);
	lua_pushlightuserdata(L, this);
	Function.Push(L);
	return lua_pcall(L, 2, 0, 0);
}

int TracebackHandler(lua_State* L);

bool ProtectedCallC(lua_State* L, lua_CFunction fun, void* context, void* context2, char const* funcDescription, char const*& error)
{
	StackCheck _(L);
	EnterVMCheck(L);

	lua_pushcfunction(L, &TracebackHandler);
	int tracebackHandlerIdx = lua_gettop(L);
	lua_pushcfunction(L, fun);
	lua_pushlightuserdata(L, context);
	lua_pushlightuserdata(L, context2);
	int status = lua_pcall(L, 2, 0, tracebackHandlerIdx);
	lua_remove(L, tracebackHandlerIdx);

	if (status != LUA_OK) {
		error = lua_tostring(L, -1);
		if (funcDescription) {
			ERR("Error while dispatching C call for %s: %s", funcDescription, lua_tostring(L, -1));
		} else {
			ERR("Error while dispatching C call: %s", lua_tostring(L, -1));
		}

		lua_pop(L, 1);
		return false;
	} else {
		return true;
	}
}

void SerializeArray(lua_State* L, Noesis::BaseCollection const* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, obj->Count(), 0);
	for (int i = 0; i < obj->Count(); i++) {
		Serialize(L, obj->GetComponent(i).GetPtr());
		lua_rawseti(L, -2, i + 1);
	}
}

END_NS()

#include <Lua/Shared/LuaGet.inl>
#include <Lua/Shared/LuaPush.inl>
#include <Lua/Shared/LuaCustomizations.inl>
#include <Lua/Shared/LuaDelegate.inl>
