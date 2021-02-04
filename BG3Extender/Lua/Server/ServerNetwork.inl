#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
	int BroadcastMessage(lua_State * L)
	{
		return luaL_error(L, "Not implemented yet!");
		/*StackCheck _(L, 0);
		auto channel = luaL_checkstring(L, 1);
		auto payload = luaL_checkstring(L, 2);

		esv::Character * excludeCharacter = nullptr;
		if (lua_gettop(L) > 2 && !lua_isnil(L, 3)) {
			auto excludeCharacterGuid = luaL_checkstring(L, 3);
			excludeCharacter = GetEntityWorld()->GetCharacter(excludeCharacterGuid);
			if (excludeCharacter == nullptr) return 0;
		}

		auto & networkMgr = gOsirisProxy->GetNetworkManager();
		auto msg = networkMgr.GetFreeServerMessage(ReservedUserId);
		if (msg != nullptr) {
			auto postMsg = msg->GetMessage().mutable_post_lua();
			postMsg->set_channel_name(channel);
			postMsg->set_payload(payload);
			if (excludeCharacter != nullptr) {
				networkMgr.ServerBroadcast(msg, excludeCharacter->UserID);
			} else {
				networkMgr.ServerBroadcast(msg, ReservedUserId);
			}
		} else {
			OsiErrorS("Could not get free message!");
		}

		return 0;*/
	}

	void PostMessageToUserInternal(UserId userId, char const* channel, char const* payload)
	{
		/*auto& networkMgr = gOsirisProxy->GetNetworkManager();
		auto msg = networkMgr.GetFreeServerMessage(userId);
		if (msg != nullptr) {
			auto postMsg = msg->GetMessage().mutable_post_lua();
			postMsg->set_channel_name(channel);
			postMsg->set_payload(payload);
			networkMgr.ServerSend(msg, userId);
		} else {
			OsiErrorS("Could not get free message!");
		}*/
	}

	int PostMessageToClient(lua_State * L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 0);
		auto characterGuid = luaL_checkstring(L, 1);
		auto channel = luaL_checkstring(L, 2);
		auto payload = luaL_checkstring(L, 3);

		auto character = GetEntityWorld()->GetCharacter(characterGuid);
		if (character == nullptr) return 0;

		if (character->UserID == ReservedUserId) {
			OsiError("Attempted to send message to character " << characterGuid << " that has no user assigned!");
			return 0;
		}

		PostMessageToUserInternal(character->UserID, channel, payload);
		return 0;*/
	}

	int PostMessageToUser(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*StackCheck _(L, 0);
		auto userId = checked_get<int>(L, 1);
		auto channel = luaL_checkstring(L, 2);
		auto payload = luaL_checkstring(L, 3);

		if (UserId(userId) == ReservedUserId) {
			OsiError("Attempted to send message to reserved user ID!");
			return 0;
		}

		PostMessageToUserInternal(UserId(userId), channel, payload);
		return 0;*/
	}

	int PlayerHasExtender(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*auto characterGuid = luaL_checkstring(L, 1);

		auto character = GetEntityWorld()->GetCharacter(characterGuid);
		if (character == nullptr || character->UserID == ReservedUserId) return 0;

		auto& networkMgr = gOsirisProxy->GetNetworkManager();
		bool hasExtender = networkMgr.ServerCanSendExtenderMessages(character->UserID.GetPeerId());
		push(L, hasExtender);
		return 1;*/
	}

	int DumpNetworking(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*auto server = (*GetStaticSymbols().EoCServer)->GameServer;
		auto loadProto = (esv::LoadProtocol*)server->Protocols[4];

		INFO(" === NETWORK DUMP === ");
		INFO("LoadProtocol state: %d", loadProto->State);
		INFO("Received LEVEL_START: %d", loadProto->NumLevelStartPlayers);
		INFO("Received SWAP_READY: %d", loadProto->NumSwapReadyPeers);

		INFO("ModuleLoaded responses:");
		for (auto const& load : loadProto->OS_ModuleLoaded) {
			INFO("    Peer %d", load.PeerId);
		}

		INFO("SessionLoaded responses:");
		for (auto const& load : loadProto->OS_SessionLoaded) {
			INFO("    Peer %d (%s)", load.PeerId, load.field_8.Str);
		}

		INFO("LevelLoaded responses:");
		for (auto const& load : loadProto->OS_LevelLoaded) {
			INFO("    Peer %d (%s)", load.PeerId, load.field_8.Str);
		}

		INFO("FileTransferOutbox items:");
		for (auto const& item : loadProto->OS_FileTransferOutbox) {
			INFO("    Peer %d, user %d", item.PeerId, item.UserId_M);
			for (auto const& path : item.OS_FixedString) {
				INFO("        FS %s", path.Str);
			}
			for (auto const& path : item.OS_Path) {
				INFO("        Path %s", path.Name.c_str());
			}
		}

		return 0;*/
	}
}
