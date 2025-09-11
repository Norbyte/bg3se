#include <stdafx.h>
#include <Extender/Server/ServerNetworking.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(esv)

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
        if (gExtender->GetServer().HasExtensionState()) {
            auto& postMsg = msg.post_lua();
            esv::LuaServerPin pin(esv::ExtensionState::Get());
            if (pin) {
                pin->OnNetMessageReceived(postMsg.channel_name(), postMsg.payload(), postMsg.module(), 
                    postMsg.request_id(), postMsg.reply_id(), context.UserID,
                    postMsg.serializer() == net::SerializerType::SERIALIZER_BINARY);
            }
        }
        break;
    }

    case net::MessageWrapper::kC2SExtenderHello:
    {
        auto const& hello = msg.c2s_extender_hello();
        DEBUG("Got extender support notification from user %d (version %d)", context.UserID.Id, hello.version());
        gExtender->GetServer().GetNetworkManager().AllowExtenderMessages(context.UserID.GetPeerId(), (net::ProtoVersion)hello.version());
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
    peerVersions_.clear();
}

bool NetworkManager::CanSendExtenderMessages(PeerId peerId) const
{
    return peerVersions_.find(peerId) != peerVersions_.end();
}

std::optional<net::ProtoVersion> NetworkManager::GetPeerVersion(PeerId peerId) const
{
    auto it = peerVersions_.find(peerId);
    if (it != peerVersions_.end()) {
        return it->second;
    } else {
        return {};
    }
}

void NetworkManager::AllowExtenderMessages(PeerId peerId, net::ProtoVersion version)
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
            hello->set_version((uint32_t)net::ProtoVersion::Current);
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

net::ProtoVersion NetworkManager::SharedVersion()
{
    auto ver{ net::ProtoVersion::Current };
    for (auto const& peer : peerVersions_) {
        ver = std::min(ver, peer.second);
    }

    return ver;
}

void NetworkManager::HandleLocalMessage(net::LocalMessage const& msg)
{
    esv::LuaServerPin pin(esv::ExtensionState::Get());
    if (pin) {
        pin->OnNetMessageReceived(msg.Channel, msg.Payload, msg.Module, msg.RequestId, msg.ReplyId, msg.User, msg.Binary);
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
            if (peerId != LocalPeerId || !excludeLocalPeer) {
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
            if (peerId != LocalPeerId || !excludeLocalPeer) {
                peerIds.push_back(peerId);
            }
        } else {
            WARN("Not sending extender message to peer %d as it does not understand extender protocol!", peerId);
        }
    }

    server->SendMessageMultiPeerMoveIds(peerIds, msg, (TPeerId)excludeUserId.GetPeerId());
}

END_NS()
