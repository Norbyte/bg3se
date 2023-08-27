#include <Version.h>

/// <lua_module>Utils</lua_module>
BEGIN_NS(lua::utils)

UserReturn Include(lua_State * L)
{
	auto modGuid = get<std::optional<STDString>>(L, 1);
	auto fileName = get<STDString>(L, 2);

	bool replaceGlobals = lua_gettop(L) > 2 && !lua_isnil(L, 3);
	auto globalsIdx = lua_gettop(L) + 1;

	if (replaceGlobals) {
		luaL_checktype(L, 3, LUA_TTABLE);
#if LUA_VERSION_NUM > 501
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		lua_pushvalue(L, 3);
		lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
	}

	std::optional<int> nret;
	if (modGuid) {
		nret = gExtender->GetCurrentExtensionState()
			->LuaLoadModScript(*modGuid, fileName, true, replaceGlobals ? 3 : 0);
	} else {
		nret = gExtender->GetCurrentExtensionState()
			->LuaLoadFile(fileName, "", true, replaceGlobals ? 3 : 0);
	}

	if (replaceGlobals) {
#if LUA_VERSION_NUM > 501
		lua_pushvalue(L, globalsIdx);
		lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		lua_remove(L, globalsIdx);
#endif
	}

	if (nret) {
		return *nret;
	} else {
		return 0;
	}
}

UserReturn LoadString(lua_State * L, char const* s)
{
	auto status = luaL_loadbufferx(L, s, strlen(s), NULL, "t");
	if (status == LUA_OK) {
		return 1;
	} else {  /* error (message is on top of the stack) */
		lua_pushnil(L);
		lua_insert(L, -2);  /* put before error message */
		return 2;  /* return nil plus error message */
	}
}

void ArgsToStream(lua_State * L, std::stringstream & ss)
{
	int nargs = lua_gettop(L);  /* number of arguments */

	lua_getglobal(L, "tostring");
	for (int i = 1; i <= nargs; i++) {
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		const char * str = lua_tostring(L, -1);  /* get result */
		if (str == nullptr)
			luaL_error(L, "'tostring' must return a string to 'print'");
		if (i > 1) ss << "\t";
		ss << str;
		lua_pop(L, 1);  /* pop result */
	}
	lua_pop(L, 1); // pop tostring
}

int GetExtensionVersion()
{
	return CurrentVersion;
}

std::optional<STDString> GameVersion()
{
	GameVersionInfo gameVersion;
	if (gExtender->GetLibraryManager().GetGameVersion(gameVersion)) {
		std::stringstream ss;
		ss << "v" << gameVersion.Major << "." << gameVersion.Minor << "." << gameVersion.Revision << "." << gameVersion.Build;
		return ss.str().c_str();
	} else {
		return {};
	}
}

int64_t MonotonicTime()
{
	using namespace std::chrono;
	return (int64_t)time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
}

void Print(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogOsirisMsg(ss.str());
}

void PrintWarning(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogOsirisWarning(ss.str());
}

void PrintError(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogLuaError(ss.str());
}

bool IsValidHandle(lua_State* L)
{
	switch (lua_type(L, 1))
	{
	case LUA_TLIGHTUSERDATA:
		return (bool)get<ComponentHandle>(L, 1);

	case LUA_TUSERDATA:
	{
		auto handle = ComponentHandleProxy::AsUserData(L, 1);
		return handle != nullptr ? (bool)handle->Handle() : false;
	}

	default:
		return false;
	}
}

/// <summary>
/// Converts a handle to an integer value for serialization purposes.
/// </summary>
/// <param name="handle">Handle to convert</param>
int64_t HandleToInteger(ComponentHandle handle)
{
	return (int64_t)handle.Handle;
}

/// <summary>
/// Converts an integer value to a handle for serialization purposes.
/// </summary>
/// <param name="i">Integer value to convert</param>
ComponentHandle IntegerToHandle(int64_t i)
{
	return ComponentHandle(i);
}

STDString GetValueType(lua_State* L)
{
	if (lua_type(L, 1) == LUA_TLIGHTUSERDATA) {
		return "lightuserdata";
	} else {
		return lua_typename(L, 1);
	}
}

// Variation of Lua builtin math_random() with custom RNG
UserReturn Random(lua_State *L)
{
	auto state = gExtender->GetCurrentExtensionState();

	lua_Integer low, up;
	switch (lua_gettop(L)) {  /* check number of arguments */
	case 0: {  /* no arguments */
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		push(L, (lua_Number)dist(state->OsiRng));  /* Number between 0 and 1 */
		return 1;
	}
	case 1: {  /* only upper limit */
		low = 1;
		up = luaL_checkinteger(L, 1);
		break;
	}
	case 2: {  /* lower and upper limits */
		low = luaL_checkinteger(L, 1);
		up = luaL_checkinteger(L, 2);
		break;
	}
	default: return luaL_error(L, "wrong number of arguments");
	}
	/* random integer in the interval [low, up] */
	luaL_argcheck(L, low <= up, 1, "interval is empty");
#if LUA_VERSION_NUM > 501
	luaL_argcheck(L, low >= 0 || up <= LUA_MAXINTEGER + low, 1,
		"interval too large");
#endif

	std::uniform_int_distribution<int64_t> dist(low, up);
	push(L, dist(state->OsiRng));
	return 1;
}

int64_t Round(double val)
{
	return (int64_t)round(val);
}

void ShowErrorAndExitGame(STDString message)
{
	gExtender->GetLibraryManager().ShowStartupError(message, true);
}

GlobalSwitches* GetGlobalSwitches()
{
	return GetStaticSymbols().GetGlobalSwitches();
}

void RegisterUtilsLib()
{
	DECLARE_MODULE(Utils, Both)
	BEGIN_MODULE()
	MODULE_NAMED_FUNCTION("Version", GetExtensionVersion)
	MODULE_FUNCTION(GameVersion)
	MODULE_FUNCTION(MonotonicTime)
	MODULE_FUNCTION(Include)
	MODULE_FUNCTION(LoadString)
	MODULE_FUNCTION(Print)
	MODULE_FUNCTION(PrintError)
	MODULE_FUNCTION(PrintWarning)
	MODULE_FUNCTION(GetValueType)
	MODULE_FUNCTION(IsValidHandle)
	MODULE_FUNCTION(HandleToInteger)
	MODULE_FUNCTION(IntegerToHandle)
	MODULE_FUNCTION(Random)
	MODULE_FUNCTION(Round)
	MODULE_FUNCTION(ShowErrorAndExitGame)
	MODULE_FUNCTION(GetGlobalSwitches)
	END_MODULE()
}

END_NS()
