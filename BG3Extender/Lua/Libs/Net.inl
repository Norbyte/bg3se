#include <Lua/Libs/Net.h>

/// <lua_module>Net</lua_module>
BEGIN_NS(lua::net)

RequestId NetworkRequestSystem::CreateRequest(LuaDelegate<void(STDString)>&& callback)
{
    auto id = nextRequestId_++;
    pendingRequests_.set(id, std::move(callback));
    return id;
}

void NetworkRequestSystem::HandleReply(RequestId replyId, char const* payload)
{
    auto it = pendingRequests_.find(replyId);
    if (it) {
        eventQueue_.Call(it->Value(), STDString(payload));
        pendingRequests_.remove(it);
    } else {
        WARN("No handler found for net message request id %d", replyId);
    }
}

void NetworkRequestSystem::Update()
{
    eventQueue_.Flush();
}

bg3se::net::ExtenderMessage* BuildMessage(lua_State* L, UserId userId, char const* channel, char const* payload, std::optional<Guid> moduleGuid, std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId)
{
    bg3se::net::BaseNetworkManager* networkMgr;
    if (gExtender->GetServer().IsInServerThread()) {
        networkMgr = &gExtender->GetServer().GetNetworkManager();
    } else {
        networkMgr = &gExtender->GetClient().GetNetworkManager();
    }

    auto msg = networkMgr->GetFreeMessage(ReservedUserId);
    if (msg != nullptr) {
        auto postMsg = msg->GetMessage().mutable_post_lua();
        postMsg->set_channel_name(channel);
        postMsg->set_payload(payload);
        if (moduleGuid) {
            postMsg->set_module(moduleGuid->ToString().c_str());
        }
        if (requestHandler) {
            auto requestId = State::FromLua(L)->GetNetworkRequests().CreateRequest(LuaDelegate<void(STDString)>(L, *requestHandler));
            postMsg->set_request_id(requestId);
        }
        if (replyId) {
            postMsg->set_reply_id(*replyId);
        }
    }

    return msg;
}

END_NS()
