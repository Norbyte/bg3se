#pragma once

#include <Windows.h>
#include <cstdint>

struct lua_State;

namespace bg3api
{

#define MAKE_API_VERSION(major, minor) (((major) << 16) | (minor))

enum class APIVersion : uint32_t
{
    VerInitial = MAKE_API_VERSION(1, 0),
    VerCurrent = VerInitial
};

#undef MAKE_API_VERSION

enum class APIContext : uint32_t
{
    Client = 0,
    Server = 1
};

enum class APILuaStateEvent : uint32_t
{
    // Thrown before destroying the Lua state
    Unloading = 0,
    // Thrown after the Lua state was destroyed
    Unloaded = 1,
    // Thrown when the Lua state was created, but no mods are loaded yet
    Loading = 2,
    // Thrown when all mods were loaded into the Lua state
    Loaded = 3
};

inline bool IsAPIVersionCompatible(APIVersion current, APIVersion expected)
{
    return ((uint32_t)current & 0xFFFF0000) == ((uint32_t)expected & 0xFFFF0000)
        && ((uint32_t)current & 0xFFFF) >= ((uint32_t)expected & 0xFFFF);
}

// Callback signatures
using LuaStateChangeCallbackProc = void (APIContext context, APILuaStateEvent stateEvent, lua_State* L, void* userData);
using ThreadedCallbackProc = void (void* userData);
using LoadCompletionCallbackProc = void (HMODULE, void*);

// Exported function signatures
using SEUpdaterGetModuleProc = HMODULE ();
using SEUpdaterNotifyWhenLoadedProc = void (LoadCompletionCallbackProc* callback, void* userData);
using SEGetAPIVersionProc = APIVersion ();
using SERegisterLuaStateChangeCallbackProc = void (LuaStateChangeCallbackProc* callback, void* userData);
using SEScheduleCallbackOnThreadProc = void (APIContext context, ThreadedCallbackProc* callback, void* userData);
using SEEnterLuaVMProc = lua_State* (APIContext context);
using SEExitLuaVMProc = void (lua_State* state);

}
