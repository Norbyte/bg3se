#pragma once

/// <lua_module>Net</lua_module>
BEGIN_NS(lua::net)

using RequestId = uint32_t;

class NetworkRequestSystem
{
public:
    NetworkRequestSystem();
    RequestId CreateRequest(LuaDelegate<void(STDString, bool)>&& callback);
    void HandleReply(RequestId replyId, StringView payload, bool binary);
    void Update();

private:
    HashMap<RequestId, LuaDelegate<void(STDString, bool)>> pendingRequests_;
    DeferredLuaDelegateQueue eventQueue_;
    RequestId nextRequestId_{ 1 };
};

END_NS()
