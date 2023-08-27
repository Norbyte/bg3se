#include <stdafx.h>
#include <Lua/LuaHelpers.h>
#include <Lua/Shared/LuaLifetime.h>
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

Table* lua_get_array(lua_State* L, int idx)
{
	auto val = lua_index2addr(L, idx);
	if (!ttistable(val)) {
		luaL_error(L, "Param %d: expected a table", idx);
		return nullptr;
	}

	auto tab = hvalue(val);
	if (tab->lsizenode > 0) {
		luaL_error(L, "Param %d must be an array-like table, not a hashtable", idx);
		return nullptr;
	}

	return tab;
}

unsigned int lua_get_array_size(Table* arr)
{
	unsigned int i;
	for (i = arr->sizearray; i > 0 && ttisnil(&arr->array[i - 1]); i--) {}
	return i;
}

Table* lua_get_array_n(lua_State* L, int idx, int size)
{
	auto arr = lua_get_array(L, idx);
	auto arrSize = lua_get_array_size(arr);
	if (arrSize != size) {
		luaL_error(L, "Param %d: expected %d-element array, got a %d-element array", idx, size, arrSize);
		return nullptr;
	}

	return arr;
}

float lua_val_get_float(lua_State* L, TValue* val)
{
	if (ttisfloat(val)) {
		return (float)fltvalue(val);
	} else if (ttisinteger(val)) {
		return (float)ivalue(val);
	} else {
		luaL_error(L, "Expected number value, got %s", lua_typename(L, ttnov(val)));
		return 0.0f;
	}
}

glm::vec2 get_raw(lua_State* L, Table* arr, Overload<glm::vec2>)
{
	return glm::vec2{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
	};
}

glm::vec3 get_raw(lua_State* L, Table* arr, Overload<glm::vec3>)
{
	return glm::vec3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
	};
}

glm::vec4 get_raw(lua_State* L, Table* arr, Overload<glm::vec4>)
{
	return glm::vec4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
	};
}

glm::quat get_raw(lua_State* L, Table* arr, Overload<glm::quat>)
{
	// quat constructor uses W,X,Y,Z
	return glm::quat{
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
	};
}

glm::mat3 get_raw(lua_State* L, Table* arr, Overload<glm::mat3>)
{
	return glm::mat3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
	};
}

glm::mat3x4 get_raw(lua_State* L, Table* arr, Overload<glm::mat3x4>)
{
	return glm::mat3x4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
	};
}

glm::mat4x3 get_raw(lua_State* L, Table* arr, Overload<glm::mat4x3>)
{
	return glm::mat4x3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
	};
}

glm::mat4 get_raw(lua_State* L, Table* arr, Overload<glm::mat4>)
{
	return glm::mat4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
		lua_val_get_float(L, arr->array + 12),
		lua_val_get_float(L, arr->array + 13),
		lua_val_get_float(L, arr->array + 14),
		lua_val_get_float(L, arr->array + 15),
	};
}

void set_raw(Table* tab, glm::vec2 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
}

void set_raw(Table* tab, glm::vec3 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
}

void set_raw(Table* tab, glm::vec4 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
	setfltvalue(tab->array + 3, v.w);
}

void set_raw(Table* tab, glm::quat const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
	setfltvalue(tab->array + 3, v.w);
}

void set_raw(Table* tab, glm::mat3 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[1].x);
	setfltvalue(tab->array + 4, m[1].y);
	setfltvalue(tab->array + 5, m[1].z);
	setfltvalue(tab->array + 6, m[2].x);
	setfltvalue(tab->array + 7, m[2].y);
	setfltvalue(tab->array + 8, m[2].z);
}

void set_raw(Table* tab, glm::mat3x4 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[0].w);
	setfltvalue(tab->array + 4, m[1].x);
	setfltvalue(tab->array + 5, m[1].y);
	setfltvalue(tab->array + 6, m[1].z);
	setfltvalue(tab->array + 7, m[1].w);
	setfltvalue(tab->array + 8, m[2].x);
	setfltvalue(tab->array + 9, m[2].y);
	setfltvalue(tab->array + 10, m[2].z);
	setfltvalue(tab->array + 11, m[2].w);
}

void set_raw(Table* tab, glm::mat4x3 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[1].x);
	setfltvalue(tab->array + 4, m[1].y);
	setfltvalue(tab->array + 5, m[1].z);
	setfltvalue(tab->array + 6, m[2].x);
	setfltvalue(tab->array + 7, m[2].y);
	setfltvalue(tab->array + 8, m[2].z);
	setfltvalue(tab->array + 9, m[3].x);
	setfltvalue(tab->array + 10, m[3].y);
	setfltvalue(tab->array + 11, m[3].z);
}

void set_raw(Table* tab, glm::mat4 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[0].w);
	setfltvalue(tab->array + 4, m[1].x);
	setfltvalue(tab->array + 5, m[1].y);
	setfltvalue(tab->array + 6, m[1].z);
	setfltvalue(tab->array + 7, m[1].w);
	setfltvalue(tab->array + 8, m[2].x);
	setfltvalue(tab->array + 9, m[2].y);
	setfltvalue(tab->array + 10, m[2].z);
	setfltvalue(tab->array + 11, m[2].w);
	setfltvalue(tab->array + 12, m[3].x);
	setfltvalue(tab->array + 13, m[3].y);
	setfltvalue(tab->array + 14, m[3].z);
	setfltvalue(tab->array + 15, m[3].w);
}

glm::vec2 do_get(lua_State* L, int index, Overload<glm::vec2>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 2);
	return get_raw(L, arr, Overload<glm::vec2>{});
}

glm::vec3 do_get(lua_State* L, int index, Overload<glm::vec3>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 3);
	return get_raw(L, arr, Overload<glm::vec3>{});
}

glm::vec4 do_get(lua_State* L, int index, Overload<glm::vec4>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 4);
	return get_raw(L, arr, Overload<glm::vec4>{});
}

glm::quat do_get(lua_State* L, int index, Overload<glm::quat>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 4);
	return get_raw(L, arr, Overload<glm::quat>{});
}

glm::mat3 do_get(lua_State* L, int index, Overload<glm::mat3>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 9);
	return get_raw(L, arr, Overload<glm::mat3>{});
}

glm::mat3x4 do_get(lua_State* L, int index, Overload<glm::mat3x4>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 12);
	return get_raw(L, arr, Overload<glm::mat3x4>{});
}

glm::mat4x3 do_get(lua_State* L, int index, Overload<glm::mat4x3>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 12);
	return get_raw(L, arr, Overload<glm::mat4x3>{});
}

glm::mat4 do_get(lua_State* L, int index, Overload<glm::mat4>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 16);
	return get_raw(L, arr, Overload<glm::mat4>{});
}

MathParam do_get(lua_State* L, int index, Overload<MathParam>)
{
	auto i = lua_absindex(L, index);
	auto arg = lua_index2addr(L, i);
	MathParam val;
	if (ttisnumber(arg)) {
		val.f = lua_val_get_float(L, arg);
		val.Arity = 1;
	} else if (ttistable(arg)) {
		auto tab = hvalue(arg);
		if (tab->lsizenode > 0) {
			luaL_error(L, "Param %d must be an array-like table, not a hashtable", index);
		}

		val.Arity = lua_get_array_size(tab);
		switch (val.Arity) {
		case 3: val.vec3 = get_raw(L, tab, Overload<glm::vec3>{}); break;
		case 4: val.vec4 = get_raw(L, tab, Overload<glm::vec4>{}); break;
		case 9: val.mat3 = get_raw(L, tab, Overload<glm::mat3>{}); break;
		case 16: val.mat4 = get_raw(L, tab, Overload<glm::mat4>{}); break;
		default: luaL_error(L, "Param %d: Unsupported vector or matrix size (%d)", index, val.Arity); break;
		}
	} else {
		luaL_error(L, "Param %d: expected a table", index);
		val.Arity = 0;
	}

	return val;
}

void push(lua_State* L, glm::vec2 const& v)
{
	lua_createtable(L, 2, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::vec3 const& v)
{
	lua_createtable(L, 3, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::vec4 const& v)
{
	lua_createtable(L, 4, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::quat const& v)
{
	lua_createtable(L, 4, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::mat3 const& m)
{
	lua_createtable(L, 9, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat3x4 const& m)
{
	lua_createtable(L, 12, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat4x3 const& m)
{
	lua_createtable(L, 12, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat4 const& m)
{
	lua_createtable(L, 16, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

/*void push(lua_State* L, Ref const& v)
{
	v.Push(L);
}

void push(lua_State* L, PersistentRef const& v)
{
	v.Push(L);
}*/

void push(lua_State* L, RegistryEntry const& v)
{
	if (v) {
		v.Push();
	} else {
		lua_pushnil(L);
	}
}

/*void push(lua_State* L, PersistentRegistryEntry const& v)
{
	if (!v.TryPush(L)) {
		lua_pushnil(L);
	}
}

Ref do_get(lua_State* L, int index, Overload<Ref>)
{
	return Ref(L, index);
}*/

RegistryEntry do_get(lua_State* L, int index, Overload<RegistryEntry>)
{
	return RegistryEntry(L, index);
}
/*
PersistentRef do_get(lua_State* L, int index, Overload<PersistentRef>)
{
	return PersistentRef(L, index);
}

PersistentRegistryEntry do_get(lua_State* L, int index, Overload<PersistentRegistryEntry>)
{
	return PersistentRegistryEntry(L, index);
}
*/
void assign(lua_State* L, int idx, glm::vec2 const& v)
{
	auto tab = lua_get_array_n(L, idx, 2);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::vec3 const& v)
{
	auto tab = lua_get_array_n(L, idx, 3);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::vec4 const& v)
{
	auto tab = lua_get_array_n(L, idx, 4);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::quat const& v)
{
	auto tab = lua_get_array_n(L, idx, 4);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::mat3 const& m)
{
	auto tab = lua_get_array_n(L, idx, 9);
	set_raw(tab, m);
}

void assign(lua_State* L, int idx, glm::mat3x4 const& m)
{
	auto tab = lua_get_array_n(L, idx, 12);
	set_raw(tab, m);
}

void assign(lua_State* L, int idx, glm::mat4 const& m)
{
	auto tab = lua_get_array_n(L, idx, 16);
	set_raw(tab, m);
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


bool ProtectedFunctionCallerBase::ProtectedCall(lua_State* L, lua_CFunction fun, char const* funcDescription)
{
	StackCheck _(L);
	auto ret = CallUserFunctionWithTraceback(L, fun);
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

int ProtectedFunctionCallerBase::CallUserFunctionWithTraceback(lua_State* L, lua_CFunction fun)
{
	lua_pushcfunction(L, &TracebackHandler);
	int tracebackHandlerIdx = lua_gettop(L);
	lua_pushcfunction(L, fun);
	lua_pushlightuserdata(L, this);
	Function.Push(L);
	int status = lua_pcall(L, 2, 0, tracebackHandlerIdx);
	lua_remove(L, tracebackHandlerIdx);
	return status;
}
*/

void push_enum_value(lua_State* L, EnumUnderlyingType value, EnumInfoStore<EnumUnderlyingType> const& store)
{
	if (value < store.Labels.size()) {
		push(L, store.Labels[(uint32_t)value]);
	} else {
		push(L, nullptr);
	}
}

void push_bitfield_value(lua_State* L, EnumUnderlyingType value, BitmaskInfoStore<EnumUnderlyingType> const& store)
{
	lua_newtable(L);
	int i = 1;
	for (auto const& kv : store.Values) {
		if ((value & kv.Value) == kv.Value) {
			push(L, i++);
			push(L, kv.Key);
			lua_settable(L, -3);
		}
	}
}

EnumUnderlyingType get_enum_value(lua_State* L, int index, EnumInfoStore<EnumUnderlyingType> const& store)
{
	switch (lua_type(L, index)) {
	case LUA_TSTRING:
	{
		auto val = lua_tostring(L, index);
		auto index = store.Values.find(FixedString(val));
		if (index != store.Values.end()) {
			return index.Value();
		} else {
			luaL_error(L, "Param '%s' is not a valid '%s' enum label: %s", val, store.LuaName.GetString(), val);
		}
		break;
	}

	case LUA_TNUMBER:
	{
		auto val = lua_tointeger(L, index);
		auto key = store.Values.find_by_value(val);
		if (key != store.Values.end()) {
			return val;
		} else {
			luaL_error(L, "Param %d is not a valid '%s' enum index: %d", index, store.LuaName.GetString(), val);
		}
		break;
	}

	default:
		luaL_error(L, "Param %d: expected integer or string '%s' enumeration value, got %s", index,
			store.LuaName.GetString(), lua_typename(L, lua_type(L, index)));
	}

	return 0;
}

EnumUnderlyingType get_bitfield_value(lua_State* L, int index, BitmaskInfoStore<EnumUnderlyingType> const& store, bool maskInvalidBits)
{
	luaL_checktype(L, index, LUA_TTABLE);
	EnumUnderlyingType flags = 0;

	for (auto idx : iterate(L, index)) {
		auto label = get<FixedString>(L, idx);
		auto val = store.Values.find(label);
		if (val != store.Values.end()) {
			flags |= val.Value();
		} else {
			luaL_error(L, "Label '%s' is not valid for enumeration '%s'", label.GetString(), store.LuaName.GetString());
		}
	}

	return flags;
}

uint32_t get_generation_id(lua_State* L)
{
	return State::FromLua(L)->GetGenerationId();
}

END_NS()
