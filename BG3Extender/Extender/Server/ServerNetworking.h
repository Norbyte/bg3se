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

	void ProcessOsirisGet(net::MessageContext& context, net::MessageWrapper& msg);
};

class NetworkManager
{
public:
	void Reset();

	bool CanSendExtenderMessages(PeerId peerId) const;
	std::optional<uint32_t> GetPeerVersion(PeerId peerId) const;
	void AllowExtenderMessages(PeerId peerId, uint32_t version);
	void OnClientConnectMessage(net::MessageContext* context, net::ClientConnectMessage* msg);

	void ExtendNetworking();

	net::ExtenderMessage * GetFreeMessage(UserId userId);
	net::ExtenderMessage * GetFreeMessage();
	net::GameServer* GetServer() const;

	void Send(net::ExtenderMessage * msg, UserId userId);
	void Broadcast(net::ExtenderMessage * msg, UserId excludeUserId, bool excludeLocalPeer = false);
	void BroadcastToConnectedPeers(net::ExtenderMessage* msg, UserId excludeUserId, bool excludeLocalPeer = false);

private:
	ExtenderProtocol * protocol_{ nullptr };
	// List of clients that support the extender protocol
	std::unordered_map<PeerId, uint32_t> peerVersions_;
};

END_NS()
