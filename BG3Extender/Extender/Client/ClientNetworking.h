#pragma once

#include <Extender/Shared/ExtenderNet.h>

BEGIN_NS(ecl)

class ExtenderProtocol : public net::ExtenderProtocolBase
{
public:
	static constexpr uint32_t ProtocolId = 100;

protected:
	void ProcessExtenderMessage(net::MessageContext& context, net::MessageWrapper& msg) override;
};

class NetworkManager
{
public:
	void Reset();

	bool CanSendExtenderMessages() const;
	void AllowExtenderMessages();
	void ExtendNetworking();
	net::ExtenderMessage* GetFreeMessage();
	void Send(net::ExtenderMessage* msg);
	void OnClientConnectMessage(net::ClientConnectMessage* msg);
	void OnExtenderHello(net::MsgC2SExtenderHello const& hello);

private:
	ExtenderProtocol* protocol_{ nullptr };

	// Indicates that the client can support extender messages to the server
	// (i.e. the server supports the message ID and won't crash)
	bool extenderSupport_{ false };

	net::Client* GetClient() const;
};

END_NS()
