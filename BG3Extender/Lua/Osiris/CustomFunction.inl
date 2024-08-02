#include <Lua/Osiris/CustomFunction.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

bool CustomLuaCall::Call(OsiArgumentDesc const & params)
{
	if (!ValidateArgs(params)) {
		return false;
	}

	LuaServerPin lua(ExtensionState::Get());
	if (!lua) {
		OsiErrorS("Call failed: Lua state not initialized");
		return false;
	}

	auto L = lua->GetState();
	lua_checkstack(L, params.Count() + 1);
	LifetimeStackPin _(lua->GetStack());
	handler_.Push();

	auto param = &params;
	int numParams{ 0 };
	while (param != nullptr) {
		OsiToLua(L, param->Value);
		numParams++;
		param = param->NextParam;
	}

	if (CallWithTraceback(lua->GetState(), numParams, 0) != 0) {
		LuaError("Handler for Osiris call '" << Name() << "' failed: " << lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}

	return true;
}


bool ServerState::Query(char const* mod, char const* name, RegistryEntry * func,
	std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params)
{
	auto L = GetState();
	auto stackSize = lua_gettop(L);

	try {
		return QueryInternal(mod, name, func, signature, params);
	} catch (Exception &) {
		auto stackRemaining = lua_gettop(L) - stackSize;
		if (stackRemaining > 0) {
			if (mod != nullptr) {
				LuaError("Call to mod query '" << mod << "'.'" << func << "' failed: " << lua_tostring(L, -1));
			} else {
				LuaError("Call to mod query '" << func << "' failed: " << lua_tostring(L, -1));
			}
			lua_pop(L, stackRemaining);
		} else {
			if (mod != nullptr) {
				LuaError("Internal error during call to mod query '" << mod << "'.'" << func << "'");
			} else {
				LuaError("Internal error during call to mod query '" << func << "'");
			}
		}

		return false;
	}
}


bool ServerState::QueryInternal(char const* mod, char const* name, RegistryEntry * func,
	std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params)
{
	auto L = GetState();
	lua_checkstack(L, params.Count() + 1);
	LifetimeStackPin _(GetStack());

	auto stackSize = lua_gettop(L);
	if (func) {
		func->Push();
	} else if (mod != nullptr) {
		PushModFunction(L, mod, name);
	} else {
		lua_getglobal(L, name);
	}

	int numParams{ 0 };
	int numOutParams{ 0 };

	auto param = &params;
	int paramIndex{ 0 };
	while (param != nullptr) {
		if (signature[paramIndex].Dir == FunctionArgumentDirection::In) {
			OsiToLua(L, param->Value);
			numParams++;
		} else {
			numOutParams++;
		}

		param = param->NextParam;
		paramIndex++;
	}

	if (CallWithTraceback(L, numParams, LUA_MULTRET) != 0) {
		LuaError("Handler for '" << name << "' failed: " << lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}

	auto numReturnValues = lua_gettop(L) - stackSize;
	if (numReturnValues == 0) {
		// No values returned, assume that the query failed
		return false;
	} else if (numOutParams == 0) {
		// Query has no OUT parameters; it should return "true" or "false".
		if (numReturnValues == 1) {
			auto retType = lua_type(L, -1);
			if (retType != LUA_TBOOLEAN) {
				OsiError("Handler for '" << name << "' returned type " << retType << "; expected boolean");
				return false;
			}

			auto ok = lua_toboolean(L, -1);
			lua_pop(L, numReturnValues);
			return ok ? true : false;
		} else {
			lua_pop(L, numReturnValues);
			OsiError("Handler for '" << name << "' returned " << numReturnValues << " values; expected 1 bool");
			return false;
		}
	} else if (numOutParams != numReturnValues) {
		lua_pop(L, numReturnValues);
		// Query has OUT parameters
		// Number of returned values should match the number of OUT params in the query
		OsiError("Handler for '" << name << "' returned " << numReturnValues << " values; expected " << numOutParams);
		return false;
	} else {
		// Lua call returned correct number of OUT parameters

		param = &params;
		paramIndex = 0;
		int stackIndex{ -numOutParams };
		int numNulls{ 0 };
		while (param != nullptr) {
			if (signature[paramIndex].Dir == FunctionArgumentDirection::Out) {
				if (lua_isnil(L, stackIndex)) {
					numNulls++;
				} else {
					LuaToOsi(L, stackIndex, param->Value, signature[paramIndex].Type);
				}

				numParams++;
				stackIndex++;
			}

			param = param->NextParam;
			paramIndex++;
		}

		lua_pop(L, numReturnValues);

		if (numNulls > 0 && numNulls != numOutParams) {
			OsiError("Handler for '" << name << "' returned " << numNulls <<
				" nil values; every return value must be nil or non-nil");
			return false;
		} else if (numNulls == numOutParams) {
			return false;
		} else {
			return true;
		}
	}

	return true;
}


bool CustomLuaQuery::Query(OsiArgumentDesc & params)
{
	if (!ValidateArgs(params)) {
		return false;
	}

	LuaServerPin lua(ExtensionState::Get());
	if (!lua) {
		OsiErrorS("Call failed: Lua state not initialized");
		return false;
	}

	return lua->Query(nullptr, Name().c_str(), &handler_, Params(), params);
}

END_NS()
