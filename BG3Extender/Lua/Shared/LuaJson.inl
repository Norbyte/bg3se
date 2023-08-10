#include <stdafx.h>
#include <Lua/LuaSerializers.h>

#include <fstream>
#include <json/json.h>

namespace bg3se::lua::utils
{
	void JsonParse(lua_State * L, Json::Value const & val);

	void JsonParseArray(lua_State * L, Json::Value const& val)
	{
		lua_newtable(L);
		int idx = 1;
		for (auto it = val.begin(), end = val.end(); it != end; ++it) {
			push(L, idx++);
			JsonParse(L, *it);
			lua_settable(L, -3);
		}
	}

	void JsonParseObject(lua_State * L, Json::Value const & val)
	{
		lua_newtable(L);
		for (auto it = val.begin(), end = val.end(); it != end; ++it) {
			JsonParse(L, it.key());
			JsonParse(L, *it);
			lua_settable(L, -3);
		}
	}

	void JsonParse(lua_State * L, Json::Value const & val)
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
			JsonParseArray(L, val);
			break;

		case Json::objectValue:
			JsonParseObject(L, val);
			break;

		default:
			luaL_error(L, "Attempted to parse unknown Json value");
		}
	}


	int JsonParse(lua_State * L)
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

		JsonParse(L, root);
		return 1;
	}

	Json::Value JsonStringify(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata);

	Json::Value JsonStringifyUserdata(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata)
	{
		StackCheck _(L, 0);

		index = lua_absindex(L, index);
		if (!lua_getmetatable(L, index)) {
			return Json::Value::null;
		}

		push(L, "__pairs");
		lua_gettable(L, -2);
		lua_remove(L, -2);
		// No __pairs function, can't iterate this object
		if (lua_type(L, -1) == LUA_TNIL) {
			lua_pop(L, 1);
			return Json::Value::null;
		}

		Json::Value arr(Json::objectValue);

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

		while (lua_type(L, -2) != LUA_TNIL) {
			Json::Value val(JsonStringify(L, -1, depth + 1, stringifyInternalTypes, iterateUserdata));

			if (lua_type(L, -2) == LUA_TSTRING) {
				auto key = lua_tostring(L, -2);
				arr[key] = val;
			} else if (lua_type(L, -2) == LUA_TNUMBER) {
				lua_pushvalue(L, -2);
				auto key = lua_tostring(L, -1);
				arr[key] = val;
				lua_pop(L, 1);
			} else if (lua_type(L, -2) == LUA_TUSERDATA && stringifyInternalTypes) {
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
		}

		lua_pop(L, 2);

		// Pop __next, obj, nil
		lua_pop(L, 3);
		return arr;
	}

	Json::Value JsonStringifyObject(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata)
	{
		Json::Value arr(Json::objectValue);
		lua_pushnil(L);

		if (index < 0) index--;

		while (lua_next(L, index) != 0) {
			Json::Value val(JsonStringify(L, -1, depth + 1, stringifyInternalTypes, iterateUserdata));

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

	Json::Value JsonStringifyArray(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata)
	{
		Json::Value arr(Json::arrayValue);
		lua_pushnil(L);

		if (index < 0) index--;

		while (lua_next(L, index) != 0) {
			arr.append(JsonStringify(L, -1, depth + 1, stringifyInternalTypes, iterateUserdata));
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

	Json::Value JsonStringifyTable(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata)
	{
		if (JsonCanStringifyAsArray(L, index)) {
			return JsonStringifyArray(L, index, depth, stringifyInternalTypes, iterateUserdata);
		} else {
			return JsonStringifyObject(L, index, depth, stringifyInternalTypes, iterateUserdata);
		}
	}


	Json::Value JsonStringify(lua_State * L, int index, int depth, bool stringifyInternalTypes, bool iterateUserdata)
	{
		if (depth > 64) {
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
			return JsonStringifyTable(L, index, depth, stringifyInternalTypes, iterateUserdata);

		case LUA_TUSERDATA:
		{
			auto obj = JsonStringifyUserdata(L, index, depth, stringifyInternalTypes, iterateUserdata);
			if (!obj.isNull()) {
				return obj;
			}
		}

		// Fallthrough

		case LUA_TLIGHTUSERDATA:
		case LUA_TFUNCTION:
		case LUA_TTHREAD:
			if (stringifyInternalTypes) {
				auto val = Json::Value(luaL_tolstring(L, index, NULL));
				lua_pop(L, 1);
				return val;
			} else {
				throw std::runtime_error("Attempted to stringify a lightuserdata, userdata, function or thread value");
			}

		default:
			throw std::runtime_error("Attempted to stringify an unknown type");
		}
	}


	int JsonStringify(lua_State * L)
	{
		StackCheck _(L, 1);
		int nargs = lua_gettop(L);
		if (nargs < 1) {
			return luaL_error(L, "JsonStringify expects at least one parameter.");
		}

		if (nargs > 4) {
			return luaL_error(L, "JsonStringify expects at most three parameters.");
		}

		bool beautify{ true };
		if (nargs >= 2) {
			beautify = lua_toboolean(L, 2) == 1;
		}

		bool stringifyInternalTypes{ false };
		if (nargs >= 3) {
			stringifyInternalTypes = lua_toboolean(L, 3) == 1;
		}

		bool iterateUserdata{ false };
		if (nargs >= 4) {
			iterateUserdata = lua_toboolean(L, 4) == 1;
		}

		Json::Value root;
		try {
			root = JsonStringify(L, 1, 0, stringifyInternalTypes, iterateUserdata);
		} catch (std::runtime_error & e) {
			return luaL_error(L, "%s", e.what());
		}

		Json::StreamWriterBuilder builder;
		if (beautify) {
			builder["indentation"] = "\t";
		}
		std::stringstream ss;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(root, &ss);

		push(L, ss.str());
		return 1;
	}

	void RegisterJsonLib(lua_State* L)
	{
		static const luaL_Reg jsonLib[] = {
			{"Parse", JsonParse},
			{"Stringify", JsonStringify},
			{0,0}
		};

		RegisterLib(L, "Json", jsonLib);
	}
}
