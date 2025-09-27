#include <Lua/Libs/Net.h>

/// <lua_module>Net</lua_module>
BEGIN_NS(lua::net)

NetworkRequestSystem::NetworkRequestSystem()
    : eventQueue_("Network event")
{}

RequestId NetworkRequestSystem::CreateRequest(LuaDelegate<void(STDString, bool)>&& callback)
{
    auto id = nextRequestId_++;
    pendingRequests_.set(id, std::move(callback));
    return id;
}

void NetworkRequestSystem::HandleReply(RequestId replyId, StringView payload, bool binary)
{
    auto it = pendingRequests_.find(replyId);
    if (it) {
        eventQueue_.Call(it->Value(), STDString(payload), binary);
        pendingRequests_.remove(it);
    } else {
        WARN("No handler found for net message request id %d", replyId);
    }
}

void NetworkRequestSystem::Update()
{
    OPTICK_EVENT();
    eventQueue_.Flush();
}

bg3se::net::ExtenderMessage* BuildMessage(lua_State* L, UserId userId, StringView channel, StringView payload, std::optional<Guid> moduleGuid,
    std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, bool binary)
{
    bg3se::net::BaseNetworkManager* networkMgr;
    se_assert(GetCurrentContextType() != ContextType::None);
    if (GetCurrentContextType() == ContextType::Server) {
        networkMgr = &gExtender->GetServer().GetNetworkManager();
    } else if (GetCurrentContextType() == ContextType::Client) {
        networkMgr = &gExtender->GetClient().GetNetworkManager();
    } else {
        ERR("Tried to send net message from unknown context!");
        return nullptr;
    }

    auto msg = networkMgr->GetFreeMessage(ReservedUserId);
    if (msg != nullptr) {
        auto postMsg = msg->GetMessage().mutable_post_lua();
        postMsg->set_channel_name(channel.data());
        postMsg->set_payload(payload.data(), payload.size());
        if (moduleGuid) {
            postMsg->set_module(moduleGuid->ToString().c_str());
        }
        if (requestHandler) {
            auto requestId = State::FromLua(L)->GetNetworkRequests().CreateRequest(LuaDelegate<void(STDString, bool)>(L, *requestHandler));
            postMsg->set_request_id(requestId);
        }
        if (replyId) {
            postMsg->set_reply_id(*replyId);
        }
        if (binary) {
            postMsg->set_serializer(bg3se::net::SerializerType::SERIALIZER_BINARY);
        } else {
            postMsg->set_serializer(bg3se::net::SerializerType::SERIALIZER_JSON);
        }
    }

    return msg;
}

void BuildMessage(lua_State* L, bg3se::net::LocalMessage& msg, UserId userId, StringView channel, StringView payload,
    std::optional<Guid> moduleGuid, std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, bool binary)
{
    msg.Channel = channel;
    msg.Payload = payload;
    if (moduleGuid) {
        msg.Module = moduleGuid->ToString();
    }
    if (requestHandler) {
        auto requestId = State::FromLua(L)->GetNetworkRequests().CreateRequest(LuaDelegate<void(STDString, bool)>(L, *requestHandler));
        msg.RequestId = requestId;
    }
    if (replyId) {
        msg.ReplyId = *replyId;
    }
    msg.User = userId;
    msg.Binary = binary;
}

END_NS()
