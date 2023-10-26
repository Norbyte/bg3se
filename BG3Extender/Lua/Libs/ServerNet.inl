/// <lua_module>Net</lua_module>
BEGIN_NS(esv::lua::net)

void BroadcastMessage(lua_State* L, char const* channel, char const* payload, std::optional<char const*> excludeCharacterGuid)
{
	esv::CharacterComponent* excludeCharacter = nullptr;
	if (excludeCharacterGuid) {
		excludeCharacter = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<CharacterComponent>(*excludeCharacterGuid);
	}

	auto & networkMgr = gExtender->GetServer().GetNetworkManager();
	auto msg = networkMgr.GetFreeMessage(ReservedUserId);
	if (msg != nullptr) {
		auto postMsg = msg->GetMessage().mutable_post_lua();
		postMsg->set_channel_name(channel);
		postMsg->set_payload(payload);
		if (excludeCharacter != nullptr) {
			networkMgr.Broadcast(msg, excludeCharacter->Character->UserID);
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

void PostMessageToClient(lua_State* L, char const* characterGuid, char const* channel, char const* payload)
{
	auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<CharacterComponent>(characterGuid);
	if (character == nullptr) return;

	if (character->Character->UserID == ReservedUserId) {
		OsiError("Attempted to send message to character " << characterGuid << " that has no user assigned!");
		return;
	}

	PostMessageToUserInternal(character->Character->UserID, channel, payload);
}

void PostMessageToUser(int userId, char const* channel, char const* payload)
{
	if (UserId(userId) == ReservedUserId) {
		OsiError("Attempted to send message to reserved user ID!");
		return;
	}

	PostMessageToUserInternal(UserId(userId), channel, payload);
}

std::optional<bool> PlayerHasExtender(lua_State* L, char const* characterGuid)
{
	auto character = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<CharacterComponent>(characterGuid);
	if (character == nullptr || character->Character->UserID == ReservedUserId) return {};

	// FIXME - access server from Lua context!
	auto& networkMgr = gExtender->GetServer().GetNetworkManager();
	return networkMgr.CanSendExtenderMessages(character->Character->UserID.GetPeerId());
}

void RegisterNetLib()
{
	DECLARE_MODULE(Net, Server)
	BEGIN_MODULE()
	MODULE_FUNCTION(BroadcastMessage)
	MODULE_FUNCTION(PostMessageToClient)
	MODULE_FUNCTION(PostMessageToUser)
	MODULE_FUNCTION(PlayerHasExtender)
	END_MODULE()
}

END_NS()
