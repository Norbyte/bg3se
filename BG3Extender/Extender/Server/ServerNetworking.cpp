#include <stdafx.h>
#include <Extender/Server/ServerNetworking.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(esv)

namespace
{
	void OsirisQueryErrorResponse(net::MessageContext& context, const net::MessageWrapper& msg, const char* error, ...)
	{
		auto message = gExtender->GetServer().GetNetworkManager().GetFreeMessage();
		auto response = message->GetMessage().mutable_s2c_osiris_query_response();
		static char errorformatbuf[1024];
		va_list args;
		va_start(args, error);
		vsnprintf(errorformatbuf, 1024, error, args);
		va_end(args);
		response->set_error(errorformatbuf);
		response->set_responseid(msg.c2s_osiris_query().msgid());
		gExtender->GetServer().GetNetworkManager().Send(message, context.UserID);
	}
}

#define QUERY_ERROR(...) OsirisQueryErrorResponse(context, msg, __VA_ARGS__)

net::ProtocolResult ExtenderProtocol::ProcessMsg(void* unused, net::MessageContext* context, net::Message* msg)
{
	auto base = ExtenderProtocolBase::ProcessMsg(unused, context, msg);
	if (base == net::ProtocolResult::Unhandled && msg->MsgId == NetMessage::NETMSG_CLIENT_CONNECT) {
		gExtender->GetServer().GetNetworkManager().OnClientConnectMessage(context, (net::ClientConnectMessage*)msg);
	}

	return base;
}

void ExtenderProtocol::ProcessExtenderMessage(net::MessageContext& context, net::MessageWrapper & msg)
{
	switch (msg.msg_case()) {
	case net::MessageWrapper::kPostLua:
	{
		auto & postMsg = msg.post_lua();
		esv::LuaServerPin pin(esv::ExtensionState::Get());
		if (pin) {
			pin->OnNetMessageReceived(STDString(postMsg.channel_name()), STDString(postMsg.payload()), context.UserID);
		}
		break;
	}

	case net::MessageWrapper::kC2SExtenderHello:
	{
		auto const& hello = msg.c2s_extender_hello();
		DEBUG("Got extender support notification from user %d (version %d)", context.UserID.Id, hello.version());
		gExtender->GetServer().GetNetworkManager().AllowExtenderMessages(context.UserID.GetPeerId(), hello.version());
		break;
	}

	case net::MessageWrapper::kUserVars:
	{
		SyncUserVars(msg.user_vars());
		break;
	}

	case net::MessageWrapper::kC2SOsirisQuery:
	{
		// FIXME - not yet supported
		auto const& query = msg.c2s_osiris_query();

		if (!gExtender->GetCurrentExtensionState()->GetLua() || gExtender->GetCurrentExtensionState()->GetLua()->RestrictionFlags & lua::State::RestrictOsiris) {
			QUERY_ERROR("Attempted to read Osiris database in restricted context");
		}

		switch (query.type())
		{
			case net::OsirisQueryType::OSIRIS_CALL:
				QUERY_ERROR("Remote Osiris calls not (yet) supported!");
				OsiErrorS("Remote Osiris calls not (yet) supported!");
				break;
			case net::OsirisQueryType::OSIRIS_DEFER:
				QUERY_ERROR("Remote Osiris deferrals not (yet) supported!");
				OsiErrorS("Remote Osiris deferrals not (yet) supported!");
				break;
			case net::OsirisQueryType::OSIRIS_DELETE:
				QUERY_ERROR("Remote Osiris deletions not (yet) supported!");
				OsiErrorS("Remote Osiris deletions not (yet) supported!");
				break;
			case net::OsirisQueryType::OSIRIS_GET:
				ProcessOsirisGet(context, msg);
				break;
		}
		break;
	}

	default:
		OsiErrorS("Unknown extension message type received!");
	}
}

void ExtenderProtocol::ProcessOsirisGet(net::MessageContext& context, net::MessageWrapper& msg)
{
	auto const& query = msg.c2s_osiris_query();

	auto func = gExtender->GetServer().Osiris().LookupFunction(query.name().c_str(), query.num_args());
	if (func != nullptr && func->Signature->OutParamList.numOutParams() == 0) {
		if (func->Type == FunctionType::Database)
		{
			auto db = func->Node.Get()->Database.Get();

			auto head = db->Facts.Head;
			auto current = head->Next;

			auto message = gExtender->GetServer().GetNetworkManager().GetFreeMessage();
			auto response = message->GetMessage().mutable_s2c_osiris_query_response();
			response->set_responseid(query.msgid());
			response->set_succeeded(true);

			std::vector<std::remove_reference_t<decltype(query.args()[0])>*> args(query.num_args(), nullptr);
			for (int i = 0; i < query.args_size(); i++)
			{
				args[query.args(i).index()] = &query.args(i);
			}

			const auto check_match = [&args](TupleVec const& v){
				for (int i = 0; i < v.Size; i++)
				{
					switch (gExtender->GetServer().Osiris().GetBaseType((ValueType)v.Values[i].TypeId))
					{
						case ValueType::Integer:
							if (args[i] != nullptr &&
								!(args[i]->val_case() == args[i]->kIntv && args[i]->intv() == v.Values[i].Value.Int32))
							{
								return false;
							}
							break;
							
						case ValueType::Integer64:
							if (args[i] != nullptr &&
								!(args[i]->val_case() == args[i]->kIntv && args[i]->intv() == v.Values[i].Value.Int64))
							{
								return false;
							}
							break;

						case ValueType::Real:
							if (args[i] != nullptr &&
								!(args[i]->val_case() == args[i]->kNumv && abs(v.Values[i].Value.Float - args[i]->numv()) <= 0.00001f))
							{
								return false;
							}
							break;

						case ValueType::String:
						case ValueType::GuidString:
							if (args[i] != nullptr &&
								!(args[i]->val_case() == args[i]->kStrv && args[i]->strv() == v.Values[i].Value.String))
							{
								return false;
							}
							break;
					}
				}
				return true;
			};

			while (current != head) {
				if (check_match(current->Item)) {
					auto result = response->add_results();
					result->set_num_retvals((uint32_t)args.size());
					for (int i = 0; i < args.size(); i++)
					{
						if (!args[i])
						{
							auto v = result->add_retvals();
							v->set_index(i);
							switch (gExtender->GetServer().Osiris().GetBaseType((ValueType)current->Item.Values[i].TypeId))
							{
								case ValueType::Integer:
									v->set_intv(current->Item.Values[i].Value.Int32);
									break;
									
								case ValueType::Integer64:
									v->set_intv(current->Item.Values[i].Value.Int64);
									break;

								case ValueType::Real:
									v->set_numv(current->Item.Values[i].Value.Float);
									break;

								case ValueType::String:
								case ValueType::GuidString:
									v->set_strv(current->Item.Values[i].Value.String);
									break;
								default:
									OsiErrorS("Unhandled Osi TypedValue");
									message->GetMessage().mutable_s2c_osiris_query_response()->set_succeeded(false);
									message->GetMessage().mutable_s2c_osiris_query_response()->set_error("Unhandled Osi TypedValue");
									goto sendtime; // Used as a multi-stage break;
							}
						}
					}
				}

				current = current->Next;
			}

		sendtime:
			gExtender->GetServer().GetNetworkManager().Send(message, context.UserID);
		}
		else
		{
			QUERY_ERROR("Function '%s(%d)' is not a database", query.name().c_str(), query.args());
		}
	}
	else
	{
		QUERY_ERROR("No database named '%s(%d)' exists", query.name().c_str(), query.args());
	}
}

void NetworkManager::Reset()
{
	peerVersions_.clear();
}

bool NetworkManager::CanSendExtenderMessages(PeerId peerId) const
{
	return peerVersions_.find(peerId) != peerVersions_.end();
}

std::optional<uint32_t> NetworkManager::GetPeerVersion(PeerId peerId) const
{
	auto it = peerVersions_.find(peerId);
	if (it != peerVersions_.end()) {
		return it->second;
	} else {
		return {};
	}
}

void NetworkManager::AllowExtenderMessages(PeerId peerId, uint32_t version)
{
	peerVersions_.insert_or_assign(peerId, version);
}


void NetworkManager::OnClientConnectMessage(net::MessageContext* context, net::ClientConnectMessage* msg)
{
	if (!msg->Build.contains(" Extender")) {
		DEBUG("No extender trailer found in ClientConnect message");
	} else if (!msg->Build.contains(" Extender_0")) {
		WARN("Extender signature incorrect in ClientConnect message");
	} else {
		DEBUG("Sending ExtenderHello to client");
		ExtendNetworking();
		auto helloMsg = GetFreeMessage();
		if (helloMsg != nullptr) {
			auto hello = helloMsg->GetMessage().mutable_c2s_extender_hello();
			hello->set_version(net::ExtenderMessage::ProtoVersion);
			Send(helloMsg, context->UserID);
		} else {
			OsiErrorS("Could not get free message!");
		}
	}
}

void NetworkManager::ExtendNetworking()
{
	auto server = GetServer();
	if (server == nullptr) return;

	if (server->NetMessageFactory->MessagePools.size() > (unsigned)net::ExtenderMessage::MessageId) {
		return;
	}

	protocol_ = new ExtenderProtocol();
	server->ProtocolList.insert_at(0, protocol_);
	server->ProtocolMap.set(ExtenderProtocol::ProtocolId, protocol_);

	auto extenderMsg = new net::ExtenderMessage();
	server->NetMessageFactory->Register((uint32_t)net::ExtenderMessage::MessageId, extenderMsg);
	gExtender->GetHooks().HookNetworkMessages(server->NetMessageFactory);
	DEBUG("Registered custom server network protocol");
}

net::GameServer * NetworkManager::GetServer() const
{
	auto server = GetStaticSymbols().esv__EoCServer;
	if (server != nullptr && *server != nullptr) {
		return (*server)->GameServer;
	} else {
		return nullptr;
	}
}

net::ExtenderMessage * NetworkManager::GetFreeMessage(UserId userId)
{
	if (userId && !CanSendExtenderMessages(userId.GetPeerId())) {
		ERR("Attempted to send extender message to user %d that does not understand extender protocol!", userId.Id);
		return nullptr;
	}

	return GetFreeMessage();
}

net::ExtenderMessage * NetworkManager::GetFreeMessage()
{
	auto server = GetServer();
	if (server != nullptr) {
		return (net::ExtenderMessage*)server->NetMessageFactory->GetFreeMessage((uint32_t)net::ExtenderMessage::MessageId);
	} else {
		return nullptr;
	}
}

void NetworkManager::Send(net::ExtenderMessage * msg, UserId userId)
{
	auto server = GetServer();
	if (server != nullptr) {
		server->SendMessageSinglePeer((TPeerId)userId.GetPeerId(), msg);
	}
}

void NetworkManager::Broadcast(net::ExtenderMessage * msg, UserId excludeUserId, bool excludeLocalPeer)
{
	auto server = GetServer();
	if (server == nullptr) return;

	Array<PeerId> peerIds;
	for (auto peerId : server->ActivePeerIds) {
		if (CanSendExtenderMessages(peerId)) {
			if (peerId != 1 || !excludeLocalPeer) {
				peerIds.push_back(peerId);
			}
		} else {
			WARN("Not sending extender message to peer %d as it does not understand extender protocol!", peerId);
		}
	}

	server->SendMessageMultiPeerMoveIds(peerIds, msg, (TPeerId)excludeUserId.GetPeerId());
}

void NetworkManager::BroadcastToConnectedPeers(net::ExtenderMessage* msg, UserId excludeUserId, bool excludeLocalPeer)
{
	auto server = GetServer();
	if (server == nullptr) return;

	Array<PeerId> peerIds;
	for (auto peerId : server->ConnectedPeerIds) {
		if (CanSendExtenderMessages(peerId)) {
			if (peerId != TPeerId(1) || !excludeLocalPeer) {
				peerIds.push_back(peerId);
			}
		} else {
			WARN("Not sending extender message to peer %d as it does not understand extender protocol!", peerId);
		}
	}

	server->SendMessageMultiPeerMoveIds(peerIds, msg, (TPeerId)excludeUserId.GetPeerId());
}

END_NS()
