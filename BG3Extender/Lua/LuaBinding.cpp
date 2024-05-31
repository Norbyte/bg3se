#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/RootTemplates.h>
#include <Lua/LuaBinding.h>
#include "resource.h"
#include <fstream>
#include <lstate.h>
#include <Lua/Shared/EntityComponentEvents.inl>

// Callback from the Lua runtime when a handled (i.e. pcall/xpcall'd) error was thrown.
// This is needed to capture errors for the Lua debugger, as there is no
// built-in mechanism to capture handled errors.
//
// Lua ldebug.c (line ~639) must be patched like this:
//
// void nse_lua_report_handled_error(lua_State* L);
// 
// l_noret luaG_errormsg(lua_State* L) {
// 	if (L->errfunc != 0) {  /* is there an error handling function? */
// 	  nse_lua_report_handled_error(L);
//    StkId errfunc = restorestack(L, L->errfunc);
//
void nse_lua_report_handled_error(lua_State* L)
{
#if !defined(OSI_NO_DEBUGGER)
	char const* err = "(Unknown)";
	if (lua_type(L, -1) == LUA_TSTRING) {
		err = lua_tostring(L, -1);
	}

	auto debugger = bg3se::gExtender->GetLuaDebugger();
	if (debugger) {
		debugger->OnLuaError(L, err);
	}
#endif
}

namespace bg3se::lua::stats
{
	void RestoreLevelMaps(std::unordered_set<int32_t> const&);
}

namespace bg3se::lua
{
	void push(lua_State* L, EntityHandle const& h)
	{
		if (h) {
			EntityProxyMetatable::Make(L, h);
		} else {
			push(L, nullptr);
		}
	}
	
	void push(lua_State* L, ComponentHandle const& h)
	{
		if (h) {
			lua_pushlightuserdata(L, (void*)h.Handle);
		} else {
			push(L, nullptr);
		}
	}

	ComponentHandle do_get(lua_State* L, int index, Overload<ComponentHandle>)
	{
		if (lua_type(L, index) == LUA_TNIL) {
			return ComponentHandle{ ComponentHandle::NullHandle };
		} else {
			return ComponentHandle{ (uint64_t)lua_touserdata(L, index) };
		}
	}

	void push(lua_State* L, ecs::EntityRef const& h)
	{
		if (h.Handle) {
			EntityProxyMetatable::Make(L, h.Handle);
		} else {
			push(L, nullptr);
		}
	}

	void push(lua_State* L, TypeInformationRef const& v)
	{
		if (v) {
			MakeObjectRef(L, v.GetStatic()->Type);
		} else {
			push(L, nullptr);
		}
	}

#if defined(ENABLE_IMGUI)
	void push(lua_State* L, ImguiHandle const& h)
	{
		if (h) {
			ImguiObjectProxyMetatable::Make(L, h);
		} else {
			push(L, nullptr);
		}
	}

	void push(lua_State* L, extui::Renderable* o)
	{
		if (o) {
			ImguiObjectProxyMetatable::Make(L, o);
		} else {
			push(L, nullptr);
		}
	}
#endif

	int TracebackHandler(lua_State * L)
	{
		const char *msg = lua_tostring(L, 1);
		if (msg == NULL) {  /* is error object not a string? */
			if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
				lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
				return 1;  /* that is the message */
			else
				msg = lua_pushfstring(L, "(error object is a %s value)",
					luaL_typename(L, 1));
		}
		luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
		return 1;  /* return the traceback */
	}

	int CallWithTraceback(lua_State * L, int narg, int nres)
	{
		int base = lua_gettop(L) - narg;  /* function index */
		lua_pushcfunction(L, &TracebackHandler);  /* push message handler */
		lua_insert(L, base);  /* put it under function and args */
		int status = lua_pcall(L, narg, nres, base);
		lua_remove(L, base);  /* remove message handler from the stack */
		return status;
	}

	// Calls Lua function.
	// Function and arguments must be already pushed to the Lua stack.
	// Returns false if call failed, true tuple otherwise.
	// Function name only needed for error reporting purposes
	bool CheckedCall(lua_State* L, int numArgs, char const* functionName)
	{
		if (CallWithTraceback(L, numArgs, 0) != 0) { // stack: errmsg
			ERR("%s Lua call failed: %s", functionName, lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}

		return true;
	}

	void PushExtFunction(lua_State * L, char const * func)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_getfield(L, -1, func); // stack: Ext, fn
		lua_remove(L, -2); // stack: fn
	}


	void PushInternalFunction(lua_State* L, char const* func)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_getfield(L, -1, "_Internal"); // stack: Ext, _Internal
		lua_remove(L, -2); // stack: _I
		lua_getfield(L, -1, func); // stack: _I, fn
		lua_remove(L, -2); // stack: fn
	}


	void PushModFunction(lua_State* L, char const* mod, char const* func)
	{
		lua_getglobal(L, "Mods"); // stack: Mods
		lua_getfield(L, -1, mod); // stack: Mods, mod
		lua_remove(L, -2); // stack: mod
		lua_getfield(L, -1, func); // stack: mod, fn
		lua_remove(L, -2); // stack: fn
	}

	void ExtensionLibrary::Register(lua_State * L)
	{
		RegisterLib(L);
		AsyncOsiFuture::RegisterMetatable(L);
	}


#if LUA_VERSION_NUM > 501
	// Unsafe libs (IO, OS, package, debug) removed
	static const luaL_Reg loadedlibs[] = {
	  {"_G", luaopen_base},
	  {LUA_TABLIBNAME, luaopen_table},
	  {LUA_STRLIBNAME, luaopen_string},
	  {LUA_MATHLIBNAME, luaopen_math},
	  // debug table is stripped in the sandbox startup script
	  {LUA_DBLIBNAME, luaopen_debug},
	  {NULL, NULL}
	};

#else
	// Unsafe libs (IO, OS, package, debug) removed
	static const luaL_Reg loadedlibs[] = {
	  {"", luaopen_base},
	  {LUA_TABLIBNAME, luaopen_table},
	  {LUA_STRLIBNAME, luaopen_string},
	  {LUA_MATHLIBNAME, luaopen_math},
	  {LUA_BITLIBNAME, luaopen_bit},
	  // debug table is stripped in the sandbox startup script
	  {LUA_DBLIBNAME, luaopen_debug},
	  {NULL, NULL}
	};

	static const luaL_Reg lj_lib_preload[] = {
	#if LJ_HASFFI
	  { LUA_FFILIBNAME,	luaopen_ffi },
	#endif
	  { NULL,		NULL }
	};

#endif

	void RegisterLib(lua_State* L, char const* name, luaL_Reg const * lib)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_createtable(L, 0, 0); // stack: ext, lib
		luaL_setfuncs(L, lib, 0);
		lua_setfield(L, -2, name);
		lua_pop(L, 1);
	}

	int LuaPanic(lua_State * L)
	{
		char const* err = "(Unknown)";
		if (lua_type(L, -1) == LUA_TSTRING) {
			err = lua_tostring(L, -1);
		}

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugger();
		if (debugger) {
			debugger->OnLuaError(L, err);
		}
#endif

		throw Exception(err);
	}

	void* LuaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
	{
		(void)ud; (void)osize;  /* not used */
		if (nsize == 0) {
			GameFree(ptr);
			return NULL;
		} else {
			auto newBuf = GameAllocRaw(nsize);
			if (ptr != nullptr) {
				memcpy(newBuf, ptr, std::min(nsize, osize));
				GameFree(ptr);
			}

			return newBuf;
		}
	}

	LifetimeHandle GetCurrentLifetime(lua_State* L)
	{
		return State::FromLua(L)->GetCurrentLifetime();
	}

	LuaStateWrapper::LuaStateWrapper()
	{
		L = lua_newstate(LuaAlloc, nullptr);
		Internal = lua_new_internal_state();
		lua_setup_cppobjects(L, &LuaCppAlloc, &LuaCppFree, &LuaCppGetLightMetatable, &LuaCppGetMetatable, &LuaCppCanonicalize);
		lua_setup_strcache(L, &LuaCacheString, &LuaReleaseString);
#if LUA_VERSION_NUM <= 501
		luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
#endif
		lua_atpanic(L, &LuaPanic);
	}

	LuaStateWrapper::~LuaStateWrapper()
	{
		lua_close(L);
	}

	State::State(uint32_t generationId, bool isServer)
		: generationId_(generationId),
		lifetimeStack_(lifetimePool_),
		globalLifetime_(lifetimePool_.Allocate()),
		variableManager_(isServer ? gExtender->GetServer().GetExtensionState().GetUserVariables() : gExtender->GetClient().GetExtensionState().GetUserVariables(), isServer),
		modVariableManager_(isServer ? gExtender->GetServer().GetExtensionState().GetModVariables() : gExtender->GetClient().GetExtensionState().GetModVariables(), isServer),
		entityHooks_(*this),
		timers_(*this, isServer)
	{
		*reinterpret_cast<State**>(lua_getextraspace(L.L)) = this;
		OpenLibs();
	}

	State::~State()
	{
		lifetimePool_.Release(globalLifetime_);
	}

	void State::Initialize()
	{
		entityHooks_.BindECS();
	}

	void State::Shutdown()
	{
		variableManager_.Invalidate();
		modVariableManager_.Invalidate();
	}

	State* State::FromLua(lua_State* L)
	{
		return *reinterpret_cast<State**>(lua_getextraspace(L));
	}

	LifetimeHandle State::GetCurrentLifetime()
	{
		if (lifetimeStack_.IsEmpty()) {
			OsiErrorS("Attempted to construct Lua object proxy while lifetime stack is empty");
			return LifetimeHandle{};
		} else {
			return lifetimeStack_.GetCurrent();
		}
	}

	void State::LoadBootstrap(STDString const& path, STDString const& modTable)
	{
		CallExt("_LoadBootstrap", RestrictAll, path, modTable);
	}

	void State::FinishStartup()
	{
		assert(!startupDone_);
		startupDone_ = true;

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugMessageHandler();
		if (debugger && debugger->IsDebuggerReady()) {
			debugger->SendModInfo();
		}
#endif
	}
		
	void State::OpenLibs()
	{
#if LUA_VERSION_NUM > 501
		const luaL_Reg *lib;
		/* "require" functions from 'loadedlibs' and set results to global table */
		for (lib = loadedlibs; lib->func; lib++) {
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);  /* remove lib */
		}
#else
		const luaL_Reg* lib;
		for (lib = loadedlibs; lib->func; lib++) {
			lua_pushcfunction(L, lib->func);
			lua_pushstring(L, lib->name);
			lua_call(L, 1, 0);
		}
		luaL_findtable(L, LUA_REGISTRYINDEX, "_PRELOAD",
			sizeof(lj_lib_preload) / sizeof(lj_lib_preload[0]) - 1);
		for (lib = lj_lib_preload; lib->func; lib++) {
			lua_pushcfunction(L, lib->func);
			lua_setfield(L, -2, lib->name);
		}
		lua_pop(L, 1);
#endif
	}

	std::optional<int> State::LoadScript(STDString const & script, STDString const & name, int globalsIdx)
	{
		int top = lua_gettop(L);

		/* Load the file containing the script we are going to run */
		int status = luaL_loadbufferx(L, script.c_str(), script.size(), name.c_str(), "text");
		if (status != LUA_OK) {
			LuaError("Failed to parse script: " << lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
			return {};
		}

#if LUA_VERSION_NUM <= 501
		if (globalsIdx != 0) {
			lua_pushvalue(L, globalsIdx);
			lua_setfenv(L, -2);
		}
#endif

		/* Ask Lua to run our little script */
		LifetimeStackPin _(lifetimeStack_);
		status = CallWithTraceback(L, 0, LUA_MULTRET);
		if (status != LUA_OK) {
			LuaError("Failed to execute script: " << lua_tostring(L, -1));
			lua_pop(L, 1); // pop error message from the stack
			return {};
		}

		return lua_gettop(L) - top;
	}

	void State::OnGameSessionLoading()
	{
		EmptyEvent params;
		ThrowEvent("SessionLoading", params, false, RestrictAll | ScopeSessionLoad);
	}

	void State::OnGameSessionLoaded()
	{
		EmptyEvent params;
		ThrowEvent("SessionLoaded", params, false, RestrictAll);
	}

	void State::OnModuleLoadStarted()
	{
		EmptyEvent params;
		ThrowEvent("ModuleLoadStarted", params, false, RestrictAll | ScopeModulePreLoad);
	}

	void State::OnStatsLoaded()
	{
		EmptyEvent params;
		ThrowEvent("StatsLoaded", params, false, RestrictAll | ScopeModuleLoad);
	}

	void State::OnModuleResume()
	{
		EmptyEvent params;
		ThrowEvent("ModuleResume", params, false, RestrictAll | ScopeModuleResume);
	}

	void State::OnLevelLoading()
	{
		variableManager_.Invalidate();
		modVariableManager_.Invalidate();
	}

	void State::OnResetCompleted()
	{
		EmptyEvent params;
		ThrowEvent("ResetCompleted", params, false, 0);
	}

	void State::OnUpdate(GameTime const& time)
	{
		timers_.Update(time.Time);

		TickEvent params{ .Time = time };
		ThrowEvent("Tick", params, false, 0);

		lua_gc(L, LUA_GCSTEP, 10);
		variableManager_.Flush();
		modVariableManager_.Flush();
	}

	void State::OnStatsStructureLoaded()
	{
		EmptyEvent params;
		ThrowEvent("StatsStructureLoaded", params, false, 0);
	}

	void State::OnNetMessageReceived(STDString const& channel, STDString const& payload, UserId userId)
	{
		NetMessageEvent params;
		params.Channel = channel;
		params.Payload = payload;
		params.UserID = userId;
		ThrowEvent("NetMessage", params);
	}

	AsyncOsiFuture* State::CreateOsirisFuture(uint32_t id)
	{
		auto ret = Userdata<AsyncOsiFuture>::New(L);
		futureManager_.emplace(id, RegistryEntry(L, -1));
		return ret;
	}

	void State::ResolveOsirisFuture(uint32_t id, std::string_view err)
	{
		auto found = futureManager_.find(id);
		if (found != futureManager_.end())
		{
			found->second.Push();
			auto future = AsyncOsiFuture::CheckUserData(L, -1);
			future->Resolve(err);

			futureManager_.erase(found);
		}
	}

	void State::ResolveOsirisFuture(uint32_t id, Array<Array<std::variant<std::monostate, StringView, int64_t, float>>>& results)
	{
		auto found = futureManager_.find(id);
		if (found != futureManager_.end())
		{
			found->second.Push();
			auto future = AsyncOsiFuture::CheckUserData(L, -1);
			future->Resolve(results);

			futureManager_.erase(found);
		}
	}

	STDString State::GetBuiltinLibrary(int resourceId)
	{
		auto resource = GetExeResource(resourceId);
		if (resource) {
			return STDString(resource->c_str());
		} else {
			return STDString();
		}
	}

	EventResult State::DispatchEvent(EventBase& evt, char const* eventName, bool canPreventAction, uint32_t restrictions)
	{
		auto stackSize = lua_gettop(L) - 2;

		try {
			Restriction restriction(*this, restrictions);
			evt.Name = FixedString(eventName);
			evt.CanPreventAction = canPreventAction;

			if (!CheckedCall(L, 1, "_ThrowEvent")) {
				return EventResult::Failed;
			}

			if (evt.ActionPrevented) {
				return EventResult::ActionPrevented;
			} else {
				return EventResult::Successful;
			}
		} catch (Exception&) {
			auto stackRemaining = lua_gettop(L) - stackSize;
			if (stackRemaining > 0) {
				LuaError("Failed to dispatch event '" << eventName << "': " << lua_tostring(L, -1));
				lua_pop(L, stackRemaining);
			}
			else {
				LuaError("Internal error while dispatching event '" << eventName << "'");
			}

			return EventResult::Failed;
		}
	}
}
