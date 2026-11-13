#pragma once

#include <concurrent_vector.h>
#include <Extender/Shared/APIExports.h>

BEGIN_SE()

struct APIStateChangeCallback
{
	bg3api::LuaStateChangeCallbackProc* Callback;
	void* UserData;
};

class APIHelpers
{
public:
	void ScheduleCallbackOnThread(bg3api::APIContext context, bg3api::ThreadedCallbackProc* callback, void* userData);
	void AddStateChangeListener(bg3api::LuaStateChangeCallbackProc* callback, void* userData);
	void OnLuaStateEvent(ContextType context, bg3api::APILuaStateEvent stateEvent, lua_State* L);

	lua_State* EnterVM(bg3api::APIContext context);
	void ExitVM(lua_State* L);

	inline bool WasActivated() const
	{
		return activated_;
	}

private:
	concurrency::concurrent_vector<APIStateChangeCallback> stateChangeCallbacks_;
	// Indicates that the extension API was used
	std::atomic<bool> activated_{ false };

	ExtensionStateBase* GetExtensionState(bg3api::APIContext context);
};

END_SE()
