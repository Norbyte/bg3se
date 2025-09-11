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

class NetworkManager : public net::BaseNetworkManager
{
public:
    void Reset();

    bool CanSendExtenderMessages() const;
    void AllowExtenderMessages();
    void ExtendNetworking();
    net::ExtenderMessage* GetFreeMessage() override;
    net::ExtenderMessage* GetFreeMessage(UserId userId) override;
    net::ProtoVersion SharedVersion() override;
    void HandleLocalMessage(net::LocalMessage const& msg) override;
    void Send(net::ExtenderMessage* msg);
    void OnClientConnectMessage(net::ClientConnectMessage* msg);
    void OnExtenderHello(net::MsgC2SExtenderHello const& hello);

private:
    ExtenderProtocol* protocol_{ nullptr };

    // Indicates that the client can support extender messages to the server
    // (i.e. the server supports the message ID and won't crash)
    bool extenderSupport_{ false };
    // Assume old host until we receive the exact version number
    net::ProtoVersion hostVersion_{ net::ProtoVersion::VerInitial };

    net::GameClient* GetClient() const;
};

END_NS()
