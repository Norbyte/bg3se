#pragma once

#include <GameDefinitions/Net.h>
#include <CoreLib/Wrappers.h>
#include <GameHooks/EngineHooksFwdDecl.h>

BEGIN_SE()

class Hooks
{
public:
    void Startup();
    void HookNetworkMessages(net::MessageFactory* factory);

    void OnParseDataBuffers(stats::RPGStats* self);
    void OnClientConnectMessage(net::Message::SerializeProc* wrapped, net::Message* msg, net::BitstreamSerializer* serializer);

    enum class ClientConnectMessageSerializeTag{};
    WrappableFunction<ClientConnectMessageSerializeTag, void(net::Message*, net::BitstreamSerializer*)> eocnet__ClientConnectMessage__Serialize;

private:
    bool loaded_{ false };
    bool networkingInitialized_{ false };
};

END_SE()
