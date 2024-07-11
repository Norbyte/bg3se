/// <lua_module>Net</lua_module>
BEGIN_NS(ecl::lua::net)

void PostMessageToServer(char const* channel, char const* payload)
{
	auto & networkMgr = gExtender->GetClient().GetNetworkManager();
	auto msg = networkMgr.GetFreeMessage();
	if (msg != nullptr) {
		auto postMsg = msg->GetMessage().mutable_post_lua();
		postMsg->set_channel_name(channel);
		postMsg->set_payload(payload);
		networkMgr.Send(msg);
	} else {
		OsiErrorS("Could not get free message!");
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
