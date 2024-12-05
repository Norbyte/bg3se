/// <lua_module>Net</lua_module>
BEGIN_NS(ecl::lua::net)

using namespace bg3se::lua::net;

void PostMessageToServer(lua_State* L, char const* channel, char const* payload, std::optional<Guid> moduleGuid, std::optional<FunctionRef> requestHandler, std::optional<RequestId> replyId)
{
	auto& networkMgr = gExtender->GetClient().GetNetworkManager();
	auto msg = BuildMessage(L, ReservedUserId, channel, payload, moduleGuid, requestHandler, replyId);
	if (msg != nullptr) {
		networkMgr.Send(msg);
	}
}

bool IsHost()
{
	auto state = GetStaticSymbols().GetServerState();
	return state && state != esv::GameState::Uninitialized;
}

void RegisterNetLib()
{
	DECLARE_MODULE(Net, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(PostMessageToServer)
	MODULE_FUNCTION(IsHost)
	END_MODULE()
}

END_NS()
