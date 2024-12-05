#pragma once

/// <lua_module>Net</lua_module>
BEGIN_NS(lua::net)

using RequestId = uint32_t;

class NetworkRequestSystem
{
public:
	RequestId CreateRequest(LuaDelegate<void(STDString)>&& callback);
	void HandleReply(RequestId replyId, char const* payload);
	void Update();

private:
	HashMap<RequestId, LuaDelegate<void(STDString)>> pendingRequests_;
	DeferredLuaDelegateQueue eventQueue_;
	RequestId nextRequestId_{ 1 };
};

END_NS()
