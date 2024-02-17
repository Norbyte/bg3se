#include <stdafx.h>
#include <Extender/Client/ClientNetworking.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(ecl)

void ExtenderProtocol::ProcessExtenderMessage(net::MessageContext& context, net::MessageWrapper& msg)
{
	switch (msg.msg_case()) {
	case net::MessageWrapper::kPostLua:
	{
		auto & postMsg = msg.post_lua();
		ecl::LuaClientPin pin(ecl::ExtensionState::Get());
		if (pin) {
			pin->OnNetMessageReceived(STDString(postMsg.channel_name()), STDString(postMsg.payload()), ReservedUserId);
		}
		break;
	}

	case net::MessageWrapper::kC2SExtenderHello:
	{
		auto const& hello = msg.c2s_extender_hello();
		gExtender->GetClient().GetNetworkManager().OnExtenderHello(hello);
		break;
	}

	case net::MessageWrapper::kS2CResetLua:
	{
		auto & resetMsg = msg.s2c_reset_lua();
		auto & ext = ecl::ExtensionState::Get();
		ext.AddPostResetCallback([&ext]() {
			ext.OnModuleResume();
			ext.OnGameSessionLoading();
			ext.OnGameSessionLoaded();
			ext.OnResetCompleted();
		});
		ext.LuaReset(resetMsg.bootstrap_scripts());
		break;
	}

	case net::MessageWrapper::kS2CSyncStat:
	{
		// FIXME - not supported for now
		// auto stats = GetStaticSymbols().GetStats();
		// stats->SyncObjectFromServer(msg.s2c_sync_stat());
		break;
	}

	case net::MessageWrapper::kS2CKick:
	{
		gExtender->GetLibraryManager().ShowStartupError(msg.s2c_kick().message().c_str(), true);
		break;
	}

	case net::MessageWrapper::kUserVars:
	{
		SyncUserVars(msg.user_vars());
		break;
	}

	default:
		OsiErrorS("Unknown extension message type received!");
	}
}

void NetworkManager::Reset()
{
	extenderSupport_ = false;
}

bool NetworkManager::CanSendExtenderMessages() const
{
	return extenderSupport_;
}

void NetworkManager::AllowExtenderMessages()
{
	extenderSupport_ = true;
}


void NetworkManager::ExtendNetworking()
{
	auto client = GetClient();
	if (client == nullptr) return;

	if (client->NetMessageFactory->MessagePools.size() > (unsigned)net::ExtenderMessage::MessageId) {
		return;
	}

	protocol_ = new ExtenderProtocol();
	client->ProtocolList.insert_at(0, protocol_);
	client->ProtocolMap.set(ExtenderProtocol::ProtocolId, protocol_);

	auto extenderMsg = new net::ExtenderMessage();
	client->NetMessageFactory->Register((uint32_t)net::ExtenderMessage::MessageId, extenderMsg);
	gExtender->GetHooks().HookNetworkMessages(client->NetMessageFactory);
	DEBUG("Registered custom client network protocol");
}

net::Client * NetworkManager::GetClient() const
{
	auto client = GetStaticSymbols().GetEoCClient();
	if (client != nullptr) {
		return client->GameClient;
	} else {
		return nullptr;
	}
}

void NetworkManager::OnClientConnectMessage(net::ClientConnectMessage* msg)
{
	DEBUG("Appending extender signature to ClientConnect");
	msg->Build += " Extender_0";
}

void NetworkManager::OnExtenderHello(net::MsgC2SExtenderHello const& hello)
{
	DEBUG("Got extender support notification from host (version %d)", hello.version());
	AllowExtenderMessages();

	auto helloMsg = GetFreeMessage();
	if (helloMsg != nullptr) {
		auto hello = helloMsg->GetMessage().mutable_c2s_extender_hello();
		hello->set_version(net::ExtenderMessage::ProtoVersion);
		Send(helloMsg);
	} else {
		OsiErrorS("Could not get free message!");
	}
}

net::ExtenderMessage* NetworkManager::GetFreeMessage()
{
	// We need to make sure that no extender message is sent if the other party
	// does not have the new message ID-s installed, otherwise the peer will crash
	// while trying to parse the packet.
	if (!extenderSupport_) {
		ERR("Attempted to send extender message to a host that does not understand extender protocol!");
		return nullptr;
	}

	auto client = GetClient();
	if (client != nullptr) {
		return (net::ExtenderMessage *)client->NetMessageFactory->GetFreeMessage((uint32_t)net::ExtenderMessage::MessageId);
	} else {
		return nullptr;
	}
}

void NetworkManager::Send(net::ExtenderMessage* msg)
{
	auto client = GetClient();
	if (client != nullptr) {
		client->SendMessageSinglePeer((TPeerId)client->HostPeerId, msg);
	}
}

END_NS()
