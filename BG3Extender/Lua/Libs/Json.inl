#include <Lua/Libs/Json.h>

#include <fstream>
#include <unordered_set>
#include <json/json.h>
#include <lstate.h>

/// <lua_module>Json</lua_module>
BEGIN_NS(lua::json)

void Parse(lua_State * L, Json::Value const & val);

void ParseArray(lua_State * L, Json::Value const & val)
{
	lua_createtable(L, (int)val.size(), 0);
	int idx = 1;
	for (auto it = val.begin(), end = val.end(); it != end; ++it) {
		push(L, idx++);
		Parse(L, *it);
		lua_rawset(L, -3);
	}
}

void ParseObject(lua_State * L, Json::Value const & val)
{
	lua_createtable(L, 0, (int)val.size());
	for (auto it = val.begin(), end = val.end(); it != end; ++it) {
		Parse(L, it.key());
		Parse(L, *it);
		lua_rawset(L, -3);
	}
}

void Parse(lua_State * L, Json::Value const & val)
{
	switch (val.type()) {
	case Json::nullValue:
		lua_pushnil(L);
		break;

	case Json::intValue:
		push(L, val.asInt64());
		break;

	case Json::uintValue:
		push(L, (int64_t)val.asUInt64());
		break;

	case Json::realValue:
		push(L, val.asDouble());
		break;

	case Json::stringValue:
		push(L, val.asCString());
		break;

	case Json::booleanValue:
		push(L, val.asBool());
		break;

	case Json::arrayValue:
		ParseArray(L, val);
		break;

	case Json::objectValue:
		ParseObject(L, val);
		break;

	default:
		luaL_error(L, "Attempted to parse unknown Json value");
	}
}

bool Parse(lua_State * L, StringView json)
{
	Json::CharReaderBuilder factory;
	std::unique_ptr<Json::CharReader> reader(factory.newCharReader());

	Json::Value root;
	std::string errs;
	if (!reader->parse(json.data(), json.data() + json.size(), &root, &errs)) {
		ERR("Unable to parse JSON: %s", errs.c_str());
		return false;
	}

	Parse(L, root);
	return true;
}

UserReturn LuaParse(lua_State * L)
{
	StackCheck _(L, 1);
	size_t length;
	auto json = luaL_checklstring(L, 1, &length);

	Json::CharReaderBuilder factory;
	std::unique_ptr<Json::CharReader> reader(factory.newCharReader());

	Json::Value root;
	std::string errs;
	if (!reader->parse(json, json + length, &root, &errs)) {
		return luaL_error(L, "Unable to parse JSON: %s", errs.c_str());
	}

	Parse(L, root);
	return 1;
}

TValue* GetStackElem(lua_State* L, int idx)
{
	CallInfo* ci = L->ci;
	if (idx > 0) {
		return ci->func + idx;
	} else if (idx > LUA_REGISTRYINDEX) {
		return L->top + idx;
	} else {
		return nullptr;
	}
}

void* GetTablePointer(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TTABLE);
	auto val = GetStackElem(L, index);
	if (val) {
		return hvalue(val);
	} else {
		return nullptr;
	}
}

void* GetLightCppObjectPointer(lua_State* L, int index)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, meta);

	// Value-type userdata has no pointer (we don't want to mark same enum values/entities as false recursion)
	if (meta.MetatableTag == MetatableTag::EnumValue
		|| meta.MetatableTag == MetatableTag::BitfieldValue
		|| meta.MetatableTag == MetatableTag::Entity
		|| meta.MetatableTag == MetatableTag::ImguiObject
		|| meta.MetatableTag == MetatableTag::OsiFunctionName) {
		return nullptr;
	}

	return (void*)((uintptr_t)meta.Ptr | ((uint64_t)meta.PropertyMapTag << 48) | ((uint64_t)meta.MetatableTag << 62));
}

void* GetPointerValue(lua_State* L, int index)
{
	switch (lua_type(L, index)) {
	case LUA_TTABLE:
		return GetTablePointer(L, index);

	case LUA_TLIGHTCPPOBJECT:
	case LUA_TCPPOBJECT:
		return GetLightCppObjectPointer(L, index);

	default:
		return nullptr;
	}
}

bool CheckForRecursion(lua_State* L, int index, StringifyContext& ctx)
{
	if (ctx.AvoidRecursion) {
		auto ptr = GetPointerValue(L, index);
		if (ptr) {
			auto seenIt = ctx.SeenUserdata.find(ptr);
			if (seenIt != ctx.SeenUserdata.end()) {
				return true;
			} else {
				ctx.SeenUserdata.insert(ptr);
			}
		}
	}

	return false;
}

bool TryGetUserdataPairs(lua_State* L, int index)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, meta);
	auto mt = State::FromLua(L)->GetMetatableManager().GetMetatable(meta.MetatableTag);
	if (lua_cmetatable_push(L, mt, (int)MetamethodName::Pairs)) {
		return true;
	}

	return false;
}

bool IsArrayLikeUserdata(lua_State* L, int index)
{
	StackCheck _(L, 0);

	if (lua_type(L, index) == LUA_TLIGHTCPPOBJECT) {
		CppObjectMetadata meta;
		lua_get_cppobject(L, index, meta);
		return meta.MetatableTag == MetatableTag::Array || meta.MetatableTag == MetatableTag::Set;
	}

	return false;
}

bool IsMapOrArrayLikeUserdata(lua_State* L, int index)
{
	StackCheck _(L, 0);

	if (lua_type(L, index) == LUA_TLIGHTCPPOBJECT) {
		CppObjectMetadata meta;
		lua_get_cppobject(L, index, meta);
		return meta.MetatableTag == MetatableTag::Map
			|| meta.MetatableTag == MetatableTag::Set
			|| meta.MetatableTag == MetatableTag::Array;
	}

	return false;
}

Json::Value Stringify(lua_State * L, int index, unsigned depth, StringifyContext& ctx);

Json::Value StringifyUserdata(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	StackCheck _(L, 0);

	index = lua_absindex(L, index);

	if (CheckForRecursion(L, index, ctx)) {
		return Json::Value("*RECURSION*");
	}

	Json::Value arr;
	if (IsArrayLikeUserdata(L, index)) {
		arr = Json::Value(Json::arrayValue);
	} else {
		arr = Json::Value(Json::objectValue);
	}

	bool isMapOrArray = IsMapOrArrayLikeUserdata(L, index);

	if (!TryGetUserdataPairs(L, index)) {
		return Json::Value();
	}

#if !defined(NDEBUG)
	auto ptrVal = GetPointerValue(L, index);
#endif

	// Call __pairs(obj)
	auto nextIndex = lua_absindex(L, -1);
	lua_pushvalue(L, index);
	lua_call(L, 1, 3); // returns __next, obj, nil

	// Push next, obj, k
	lua_pushvalue(L, nextIndex);
	lua_pushvalue(L, nextIndex + 1);
	lua_pushvalue(L, nextIndex + 2);
	// Call __next(obj, k)
	lua_call(L, 2, 2); // returns k, val

	int numElements{ 0 };
	while (lua_type(L, -2) != LUA_TNIL) {
		if (isMapOrArray && ctx.LimitArrayElements != -1 && numElements > ctx.LimitArrayElements) {
			break;
		}

#if !defined(NDEBUG)
		STDString key;
		if (lua_type(L, -2) == LUA_TSTRING) {
			key = lua_tostring(L, -2);
		} else if (lua_type(L, -2) == LUA_TNUMBER) {
			lua_pushvalue(L, -2);
			key = lua_tostring(L, -1);
			lua_pop(L, 1);
		}
#endif

		Json::Value val(Stringify(L, -1, depth + 1, ctx));

		auto type = lua_type(L, -2);
		if (type == LUA_TSTRING) {
			auto key = lua_tostring(L, -2);
			arr[key] = val;
		} else if (type == LUA_TNUMBER) {
			auto key = lua_tointeger(L, -2);
			if (arr.type() == Json::arrayValue) {
				arr[(uint32_t)key - 1] = val;
			} else {
				lua_pushvalue(L, -2);
				auto key = lua_tostring(L, -1);
				arr[key] = val;
				lua_pop(L, 1);
			}
		} else if ((type == LUA_TLIGHTCPPOBJECT || type == LUA_TCPPOBJECT) && ctx.StringifyInternalTypes) {
			int top = lua_gettop(L);
			lua_getglobal(L, "tostring");  /* function to be called */
			lua_pushvalue(L, -3);   /* value to print */
			lua_call(L, 1, 1);
			const char* key = lua_tostring(L, -1);  /* get result */
			if (key) {
				arr[key] = val;
			}
			int top2 = lua_gettop(L);
			lua_pop(L, 1);  /* pop result */
		} else if (type == LUA_TLIGHTUSERDATA && ctx.StringifyInternalTypes) {
			auto handle = get<EntityHandle>(L, -2);
			char key[100];
			sprintf_s(key, "%016llx", handle.Handle);
			arr[key] = val;
		} else {
			throw std::runtime_error("Can only stringify string or number table keys");
		}

		// Push next, obj, k
		lua_pushvalue(L, nextIndex);
		lua_pushvalue(L, nextIndex + 1);
		lua_pushvalue(L, nextIndex + 3);
		lua_remove(L, -4);
		lua_remove(L, -4);
		// Call __next(obj, k)
		lua_call(L, 2, 2); // returns k, val
		numElements++;
	}

	lua_pop(L, 2);

	// Pop __next, obj, nil
	lua_pop(L, 3);
	return arr;
}

Json::Value StringifyTableAsObject(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	Json::Value arr(Json::objectValue);
	lua_pushnil(L);

	if (index < 0) index--;

	while (lua_next(L, index) != 0) {
		Json::Value val(Stringify(L, -1, depth + 1, ctx));

		if (lua_type(L, -2) == LUA_TSTRING) {
			auto key = lua_tostring(L, -2);
			arr[key] = val;
		} else if (lua_type(L, -2) == LUA_TNUMBER) {
			lua_pushvalue(L, -2);
			auto key = lua_tostring(L, -1);
			arr[key] = val;
			lua_pop(L, 1);
		} else {
			throw std::runtime_error("Can only stringify string or number table keys");
		}

		lua_pop(L, 1);
	}

	return arr;
}

Json::Value StringifyTableAsArray(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	Json::Value arr(Json::arrayValue);
	lua_pushnil(L);

	if (index < 0) index--;

	while (lua_next(L, index) != 0) {
		arr.append(Stringify(L, -1, depth + 1, ctx));
		lua_pop(L, 1);
	}

	return arr;
}

bool JsonCanStringifyAsArray(lua_State * L, int index)
{
	Json::Value arr(Json::objectValue);
	lua_pushnil(L);

	if (index < 0) index--;

	int next = 1;
	bool isArray = true;
	while (lua_next(L, index) != 0) {
#if LUA_VERSION_NUM > 501
		if (lua_isinteger(L, -2)) {
			auto key = lua_tointeger(L, -2);
			if (key != next++) {
				isArray = false;
			}
		} else {
			isArray = false;
		}
#else
		if (lua_isnumber(L, -2)) {
			auto key = lua_tonumber(L, -2);
			if (abs(key - next++) < 0.0001) {
				isArray = false;
			}
		} else {
			isArray = false;
		}
#endif

		lua_pop(L, 1);
	}

	return isArray;
}

Json::Value StringifyTable(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	if (CheckForRecursion(L, index, ctx)) {
		return Json::Value("*RECURSION*");
	}

	if (JsonCanStringifyAsArray(L, index)) {
		return StringifyTableAsArray(L, index, depth, ctx);
	} else {
		return StringifyTableAsObject(L, index, depth, ctx);
	}
}


Json::Value StringifyInternalType(lua_State * L, int index, StringifyContext& ctx)
{
	if (ctx.StringifyInternalTypes) {
		auto val = Json::Value(luaL_tolstring(L, index, NULL));
		lua_pop(L, 1);
		return val;
	} else {
		throw std::runtime_error("Attempted to stringify a lightuserdata, userdata, function or thread value");
	}
}

Json::Value TryStringifyUserdata(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	CppValueMetadata meta;
	index = lua_absindex(L, index);
	if (lua_try_get_cppvalue(L, index, EnumValueMetatable::MetaTag, meta)) {
		return Json::Value(EnumValueMetatable::GetLabel(meta).GetString());
	}

	if (lua_try_get_cppvalue(L, index, BitfieldValueMetatable::MetaTag, meta)) {
		return BitfieldValueMetatable::ToJson(meta);
	}

	if (ctx.IterateUserdata) {
		if (ctx.LimitDepth != -1 && depth > (uint32_t)ctx.LimitDepth) {
			return Json::Value("*DEPTH LIMIT EXCEEDED*");
		}

		auto obj = StringifyUserdata(L, index, depth, ctx);
		if (!obj.isNull()) {
			return obj;
		}
	}

	return StringifyInternalType(L, index, ctx);
}

Json::Value Stringify(lua_State * L, int index, unsigned depth, StringifyContext& ctx)
{
	if (depth > ctx.MaxDepth) {
		throw std::runtime_error("Recursion depth exceeded while stringifying JSON");
	}

	switch (lua_type(L, index)) {
	case LUA_TNIL:
		return Json::Value(Json::nullValue);

	case LUA_TBOOLEAN:
		return Json::Value(lua_toboolean(L, index) == 1);

	case LUA_TNUMBER:
#if LUA_VERSION_NUM > 501
		if (lua_isinteger(L, index)) {
			return Json::Value(lua_tointeger(L, index));
		} else {
			return Json::Value(lua_tonumber(L, index));
		}
#else
		return Json::Value(lua_tonumber(L, index));
#endif

	case LUA_TSTRING:
		return Json::Value(lua_tostring(L, index));

	case LUA_TTABLE:
		if (ctx.LimitDepth != -1 && depth > (uint32_t)ctx.LimitDepth) {
			return Json::Value("*DEPTH LIMIT EXCEEDED*");
		}

		return StringifyTable(L, index, depth, ctx);

	case LUA_TLIGHTCPPOBJECT:
	case LUA_TCPPOBJECT:
		return TryStringifyUserdata(L, index, depth, ctx);

	case LUA_TLIGHTUSERDATA:
	case LUA_TFUNCTION:
	case LUA_TTHREAD:
		return StringifyInternalType(L, index, ctx);

	default:
		throw std::runtime_error("Attempted to stringify an unknown type");
	}
}


std::string Stringify(lua_State * L, StringifyContext& ctx, int index)
{
	StackCheck _(L);

	Json::Value root;
	root = Stringify(L, index, 0, ctx);

	Json::StreamWriterBuilder builder;
	if (ctx.Beautify) {
		builder["indentation"] = "\t";
	}
	std::stringstream ss;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root, &ss);

	return ss.str();
}

UserReturn LuaStringify(lua_State * L)
{
	StackCheck _(L, 1);
	int nargs = lua_gettop(L);
	if (nargs < 1) {
		return luaL_error(L, "Stringify expects at least one parameter.");
	}

	if (nargs > 4) {
		return luaL_error(L, "Stringify expects at most three parameters.");
	}

	StringifyContext ctx;

	if (nargs >= 2) {
		// New stringify API - Json.Stringify(obj, paramTable)
		if (lua_type(L, 2) == LUA_TTABLE) {
			ctx.Beautify = try_gettable<bool>(L, "Beautify", 2, true);
			ctx.StringifyInternalTypes = try_gettable<bool>(L, "StringifyInternalTypes", 2, false);
			ctx.IterateUserdata = try_gettable<bool>(L, "IterateUserdata", 2, false);
			ctx.AvoidRecursion = try_gettable<bool>(L, "AvoidRecursion", 2, false);
			ctx.MaxDepth = try_gettable<uint32_t>(L, "MaxDepth", 2, 64);
			ctx.LimitDepth = try_gettable<int32_t>(L, "LimitDepth", 2, -1);
			ctx.LimitArrayElements = try_gettable<int32_t>(L, "LimitArrayElements", 2, -1);

			if (ctx.MaxDepth > 64) {
				ctx.MaxDepth = 64;
			}
		} else {
			// Old stringify API - Json.Stringify(obj, beautify, stringifyInternalTypes, iterateUserdata)
			ctx.Beautify = lua_toboolean(L, 2) == 1;

			if (nargs >= 3) {
				ctx.StringifyInternalTypes = lua_toboolean(L, 3) == 1;
			}

			if (nargs >= 4) {
				ctx.IterateUserdata = lua_toboolean(L, 4) == 1;
			}
		}
	}

	DisablePropertyWarnings();
	try {
		push(L, Stringify(L, ctx, 1));
	} catch (std::runtime_error& e) {
		return luaL_error(L, "%s", e.what());
	}
	EnablePropertyWarnings();

	return 1;
}

void RegisterJsonLib()
{
	DECLARE_MODULE(Json, Both)
	BEGIN_MODULE()
	MODULE_NAMED_FUNCTION("Parse", LuaParse)
	MODULE_NAMED_FUNCTION("Stringify", LuaStringify)
	END_MODULE()
}

END_NS()
