/// <lua_module>Net</lua_module>
BEGIN_NS(esv::lua::net)

void BroadcastMessage(lua_State* L, char const* channel, char const* payload, std::optional<Guid> excludeCharacterGuid)
{
	esv::Character* excludeCharacter = nullptr;
	if (excludeCharacterGuid) {
		excludeCharacter = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<Character>(*excludeCharacterGuid);
	}

	auto & networkMgr = gExtender->GetServer().GetNetworkManager();
	auto msg = networkMgr.GetFreeMessage(ReservedUserId);
	if (msg != nullptr) {
		auto postMsg = msg->GetMessage().mutable_post_lua();
		postMsg->set_channel_name(channel);
		postMsg->set_payload(payload);
		if (excludeCharacter != nullptr) {
			networkMgr.Broadcast(msg, excludeCharacter->UserID);
		} else {
			networkMgr.Broadcast(msg, ReservedUserId);
		}
	}
}

void PostMessageToUserInternal(UserId userId, char const* channel, char const* payload)
{
	auto& networkMgr = gExtender->GetServer().GetNetworkManager();
	auto msg = networkMgr.GetFreeMessage(userId);
	if (msg != nullptr) {
		auto postMsg = msg->GetMessage().mutable_post_lua();
		postMsg->set_channel_name(channel);
		postMsg->set_payload(payload);
		networkMgr.Send(msg, userId);
	}
}

void PostMessageToClient(lua_State* L, Guid characterGuid, char const* channel, char const* payload)
{
	auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<Character>(characterGuid);
	if (character == nullptr) return;

	if (character->UserID == ReservedUserId) {
		OsiError("Attempted to send message to character " << characterGuid << " that has no user assigned!");
		return;
	}

	PostMessageToUserInternal(character->UserID, channel, payload);
}

void PostMessageToUser(int userId, char const* channel, char const* payload)
{
	if (UserId(userId) == ReservedUserId) {
		OsiError("Attempted to send message to reserved user ID!");
		return;
	}

	PostMessageToUserInternal(UserId(userId), channel, payload);
}

std::optional<bool> PlayerHasExtender(lua_State* L, Guid characterGuid)
{
	auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<Character>(characterGuid);
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

void RegisterNetLib()
{
	DECLARE_MODULE(Net, Server)
	BEGIN_MODULE()
	MODULE_FUNCTION(BroadcastMessage)
	MODULE_FUNCTION(PostMessageToClient)
	MODULE_FUNCTION(PostMessageToUser)
	MODULE_FUNCTION(PlayerHasExtender)
	MODULE_FUNCTION(IsHost)
	END_MODULE()
}

END_NS()
