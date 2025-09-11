#pragma once

#include <Extender/Shared/ExtenderNet.h>

BEGIN_NS(esv)

class ExtenderProtocol : public net::ExtenderProtocolBase
{
public:
    static constexpr uint32_t ProtocolId = 101;

    net::ProtocolResult ProcessMsg(void* unused, net::MessageContext* unknown, net::Message* usg) override;

protected:
    void ProcessExtenderMessage(net::MessageContext& context, net::MessageWrapper& msg) override;
};

class NetworkManager : public net::BaseNetworkManager
{
public:
    void Reset();

    bool CanSendExtenderMessages(PeerId peerId) const;
    std::optional<net::ProtoVersion> GetPeerVersion(PeerId peerId) const;
    void AllowExtenderMessages(PeerId peerId, net::ProtoVersion version);
    void OnClientConnectMessage(net::MessageContext* context, net::ClientConnectMessage* msg);

    void ExtendNetworking();

    net::ExtenderMessage * GetFreeMessage(UserId userId) override;
    net::ExtenderMessage * GetFreeMessage() override;
    net::ProtoVersion SharedVersion() override;
    void HandleLocalMessage(net::LocalMessage const& msg) override;
    net::GameServer* GetServer() const;

    void Send(net::ExtenderMessage * msg, UserId userId);
    void Broadcast(net::ExtenderMessage * msg, UserId excludeUserId, bool excludeLocalPeer = false);
    void BroadcastToConnectedPeers(net::ExtenderMessage* msg, UserId excludeUserId, bool excludeLocalPeer = false);

private:
    ExtenderProtocol * protocol_{ nullptr };
    // List of clients that support the extender protocol
    std::unordered_map<PeerId, net::ProtoVersion> peerVersions_;
};

END_NS()