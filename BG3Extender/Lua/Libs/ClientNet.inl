/// <lua_module>Net</lua_module>
BEGIN_NS(ecl::lua::net)

using namespace bg3se::lua::net;

void PostMessageToServer(lua_State* L, StringView channel, StringView payload, std::optional<Guid> moduleGuid,
    std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId, std::optional<bool> binary)
{
    if (moduleGuid && gExtender->GetServer().HasExtensionState() && false) {
        bg3se::net::LocalMessage msg;
        BuildMessage(L, msg, UserId{0x10001}, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        gExtender->GetServer().GetNetworkManager().PushLocalMessage(std::move(msg));
    } else {
        auto& networkMgr = gExtender->GetClient().GetNetworkManager();
        auto msg = BuildMessage(L, ReservedUserId, channel, payload, moduleGuid, requestHandler, replyId, binary.value_or(false));
        if (msg != nullptr) {
            networkMgr.Send(msg);
        }
    }
}

bool IsHost()
{
    auto state = GetStaticSymbols().GetServerState();
    return state && state != esv::GameState::Uninitialized;
}

uint32_t Version()
{
    return (uint32_t)gExtender->GetClient().GetNetworkManager().SharedVersion();
}

void RegisterNetLib()
{
    DECLARE_MODULE(Net, Client)
    BEGIN_MODULE()
    MODULE_FUNCTION(PostMessageToServer)
    MODULE_FUNCTION(IsHost)
    MODULE_FUNCTION(Version)
    END_MODULE()
}

END_NS()
