#include <Extender/Version.h>
#include <GameDefinitions/Dialog.h>

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
		nret = ExtensionStateBase::FromLua(L).LuaLoadModScript(*modGuid, fileName, true, replaceGlobals ? 3 : 0);
	} else {
		nret = ExtensionStateBase::FromLua(L).LuaLoadFile(fileName, "", true, replaceGlobals ? 3 : 0);
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
	bool replaceGlobals = lua_gettop(L) > 1 && !lua_isnil(L, 2);
	auto globalsIdx = lua_gettop(L) + 1;

	if (replaceGlobals) {
		luaL_checktype(L, 2, LUA_TTABLE);
#if LUA_VERSION_NUM > 501
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		lua_pushvalue(L, 2);
		lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
	}

	auto status = luaL_loadbufferx(L, s, strlen(s), NULL, "t");

	if (replaceGlobals) {
#if LUA_VERSION_NUM > 501
		lua_pushvalue(L, globalsIdx);
		lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		lua_remove(L, globalsIdx);
#endif
	}

	if (status == LUA_OK) {
		return 1;
	} else {  /* error (message is on top of the stack) */
		lua_pushnil(L);
		lua_insert(L, -2);  /* put before error message */
		return 2;  /* return nil plus error message */
	}
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

// Keep a reference for game start to reduce precision loss caused by casting an uint64 to double
LARGE_INTEGER AppStartCounter;

double MicrosecTime()
{
	LARGE_INTEGER perf, freq;
	QueryPerformanceCounter(&perf);
	QueryPerformanceFrequency(&freq);

	return (perf.QuadPart - AppStartCounter.QuadPart) / (double(freq.QuadPart) / 1000000.0);
}

double GameTime(lua_State* L)
{
	return ExtensionStateBase::FromLua(L).Time().Time;
}

bool IsValidHandle(lua_State* L)
{
	switch (lua_type(L, 1))
	{
	case LUA_TLIGHTUSERDATA:
		return (bool)get<EntityHandle>(L, 1);

	case LUA_TLIGHTCPPOBJECT:
	{
		auto handle = EntityProxyMetatable::Get(L, 1);
		return (bool)handle;
	}

	default:
		return false;
	}
}

/// <summary>
/// Converts a handle to an integer value for serialization purposes.
/// </summary>
/// <param name="handle">Handle to convert</param>
int64_t HandleToInteger(EntityHandle handle)
{
	return (int64_t)handle.Handle;
}

/// <summary>
/// Converts an integer value to a handle for serialization purposes.
/// </summary>
/// <param name="i">Integer value to convert</param>
EntityHandle IntegerToHandle(int64_t i)
{
	return EntityHandle(i);
}

STDString GetValueType(lua_State* L)
{
	if (lua_type(L, 1) == LUA_TLIGHTUSERDATA) {
		return "lightuserdata";
	} else {
		return lua_typename(L, 1);
	}
}

void ShowErrorAndExitGame(STDString message)
{
	gExtender->GetLibraryManager().ShowStartupError(message, true);
}

void ShowError(STDString message)
{
	gExtender->GetLibraryManager().ShowStartupError(message, false);
}

GlobalSwitches* GetGlobalSwitches()
{
	return GetStaticSymbols().GetGlobalSwitches();
}

Array<STDString> GetCommandLineParams()
{
	Array<STDString> params;
	auto cmdLine = ToUTF8(GetCommandLineW());

	STDString::size_type pos{ 0 };
	while (pos < cmdLine.size()) {
		auto next = cmdLine.find(' ', pos);
		if (next == STDString::npos) {
			next = cmdLine.size();
		}

		auto arg = cmdLine.substr(pos, next - pos);
		if (!arg.empty()) {
			params.push_back(arg);
		}
		pos = next + 1;
	}

	return params;
}

dlg::DialogManager* GetDialogManager(lua_State* L)
{
	if (gExtender->GetServer().IsInServerThread()) {
		auto ds = State::FromLua(L)->GetEntitySystemHelpers()->GetSystem<esv::DialogSystem>();
		return ds ? ds->GameInterface.DialogManager : nullptr;
	} else {
		return nullptr;
	}
}

void RegisterUtilsLib()
{
	QueryPerformanceCounter(&AppStartCounter);

	DECLARE_MODULE(Utils, Both)
	BEGIN_MODULE()
	MODULE_NAMED_FUNCTION("Version", GetExtensionVersion)
	MODULE_FUNCTION(GameVersion)
	MODULE_FUNCTION(MonotonicTime)
	MODULE_FUNCTION(MicrosecTime)
	MODULE_FUNCTION(GameTime)
	MODULE_FUNCTION(Include)
	MODULE_FUNCTION(LoadString)
	MODULE_FUNCTION(GetValueType)
	MODULE_FUNCTION(IsValidHandle)
	MODULE_FUNCTION(HandleToInteger)
	MODULE_FUNCTION(IntegerToHandle)
	MODULE_FUNCTION(ShowErrorAndExitGame)
	MODULE_FUNCTION(ShowError)
	MODULE_FUNCTION(GetGlobalSwitches)
	MODULE_FUNCTION(GetCommandLineParams)
	MODULE_FUNCTION(GetDialogManager)
	END_MODULE()
}

END_NS()
