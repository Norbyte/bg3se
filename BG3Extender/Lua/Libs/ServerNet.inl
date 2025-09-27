/// <lua_module>Net</lua_module>
BEGIN_NS(esv::lua::net)

using namespace bg3se::lua::net;

void BroadcastMessage(lua_State* L, StringView channel, StringView payload, std::optional<Guid> excludeCharacterGuid,
    std::optional<Guid> moduleGuid, std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId,
    std::optional<bool> binary)
{
    esv::Character* excludeCharacter = nullptr;
    if (excludeCharacterGuid) {
        excludeCharacter = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<esv::Character>(*excludeCharacterGuid);
    }

    auto& networkMgr = gExtender->GetServer().GetNetworkManager();
    if (moduleGuid 
        && excludeCharacter == nullptr 
        && networkMgr.LocalPeerOnly()
        && gExtender->GetConfig().LocalMessagePassing) {
        bg3se::net::LocalMessage msg;
        BuildMessage(L, msg, ReservedUserId, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        gExtender->GetClient().GetNetworkManager().PushLocalMessage(std::move(msg));
    } else {
        auto msg = BuildMessage(L, ReservedUserId, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        if (msg != nullptr) {
            if (excludeCharacter != nullptr) {
                networkMgr.Broadcast(msg, excludeCharacter->UserID);
            } else {
                networkMgr.Broadcast(msg, ReservedUserId);
            }
        }
    }
}

void PostMessageToUserInternal(lua_State* L, UserId userId, StringView channel, StringView payload, std::optional<Guid> moduleGuid,
    std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, std::optional<bool> binary)
{
    if (moduleGuid 
        && (uint32_t)userId.GetPeerId() == 1
        && gExtender->GetConfig().LocalMessagePassing) {
        bg3se::net::LocalMessage msg;
        BuildMessage(L, msg, userId, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        gExtender->GetClient().GetNetworkManager().PushLocalMessage(std::move(msg));
    } else {
        auto& networkMgr = gExtender->GetServer().GetNetworkManager();
        auto msg = BuildMessage(L, userId, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        if (msg != nullptr) {
            networkMgr.Send(msg, userId);
        }
    }
}

void PostMessageToClient(lua_State* L, Guid characterGuid, StringView channel, StringView payload, std::optional<Guid> moduleGuid,
    std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, std::optional<bool> binary)
{
    auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<esv::Character>(characterGuid);
    if (character == nullptr) return;

    if (character->UserID == ReservedUserId) {
        OsiError("Attempted to send message to character " << characterGuid << " on channel '" << channel << "' that has no user assigned!");
        return;
    }

    PostMessageToUserInternal(L, character->UserID, channel, payload, moduleGuid, requestHandler, replyId, binary);
}

void PostMessageToUser(lua_State* L, int userId, StringView channel, StringView payload, std::optional<Guid> moduleGuid,
    std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, std::optional<bool> binary)
{
    if (UserId(userId) == ReservedUserId) {
        OsiError("Attempted to send message on channel '" << channel << "' to reserved user ID!");
        return;
    }

    PostMessageToUserInternal(L, UserId(userId), channel, payload, moduleGuid, requestHandler, replyId, binary);
}

std::optional<bool> PlayerHasExtender(lua_State* L, Guid characterGuid)
{
    auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<esv::Character>(characterGuid);
    if (character == nullptr || character->UserID == ReservedUserId) return {};

    // FIXME - access server from Lua context!
    auto& networkMgr = gExtender->GetServer().GetNetworkManager();
    return networkMgr.CanSendExtenderMessages(character->UserID.GetPeerId());
}

// Kinda pointless on the server, but we'll leave it here for consistency between client/server APIs
bool IsHost()
{
    return true;
}

uint32_t Version()
{
    return (uint32_t)gExtender->GetServer().GetNetworkManager().SharedVersion();
}

void RegisterNetLib()
{
    DECLARE_MODULE(Net, Server)
    BEGIN_MODULE()
    MODULE_FUNCTION(BroadcastMessage)
    MODULE_FUNCTION(PostMessageToClient)
    MODULE_FUNCTION(PostMessageToUser)
    MODULE_FUNCTION(PlayerHasExtender)
    MODULE_FUNCTION(IsHost)
    MODULE_FUNCTION(Version)
    END_MODULE()
}

END_NS()
