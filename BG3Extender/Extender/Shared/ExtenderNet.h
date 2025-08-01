#pragma once

#include <GameDefinitions/Net.h>
#include <Extender/Shared/ExtenderProtocol.pb.h>
#include <concurrent_queue.h>

BEGIN_NS(net)

class ExtenderMessage : public Message
{
public:
    static constexpr NetMessage MessageId = NetMessage::NETMSG_SCRIPT_EXTENDER;
    static constexpr uint32_t MaxPayloadLength = 0xfffff;

    static constexpr uint32_t VerInitial = 1;
    // Version of protocol, increment each time the protobuf changes
    static constexpr uint32_t ProtoVersion = VerInitial;

    ExtenderMessage();
    ~ExtenderMessage() override;

    void Serialize(BitstreamSerializer & serializer) override;
    void Unknown() override;
    Message * CreateNew() override;
    void Reset() override;

    inline MessageWrapper & GetMessage()
    {
#if defined(_DEBUG)
        return *message_;
#else
        return message_;
#endif
    }

    inline bool IsValid() const
    {
        return valid_;
    }

private:
#if defined(_DEBUG)
    MessageWrapper* message_{ nullptr };
#else
    MessageWrapper message_;
#endif
    bool valid_{ false };
};


class ExtenderProtocolBase : public Protocol
{
public:
    ~ExtenderProtocolBase() override;

    ProtocolResult ProcessMsg(void * unused, MessageContext * unknown, Message* usg) override;
    ProtocolResult PreUpdate(GameTime const& time) override;
    ProtocolResult PostUpdate(GameTime const& time) override;
    void OnAddedToHost() override;
    void OnRemovedFromHost() override;
    void Reset() override;

    void SyncUserVars(MsgUserVars const& msg);

protected:
    virtual void ProcessExtenderMessage(net::MessageContext& context, MessageWrapper & msg) = 0;
    void DoSyncUserVars(MsgUserVars const& msg);
};

struct LocalMessage
{
    STDString Channel;
    STDString Module;
    STDString Payload;
    int32_t RequestId{ 0 };
    int32_t ReplyId{ 0 };
    UserId User;
};

class BaseNetworkManager
{
public:
    virtual ExtenderMessage* GetFreeMessage() = 0;
    virtual ExtenderMessage* GetFreeMessage(UserId userId) = 0;
    virtual void HandleLocalMessage(char const* channel, char const* payload, char const* moduleUuid, int32_t requestId, int32_t replyId, UserId userId) = 0;
    virtual void Update();
    virtual void OnResetExtensionState();

    inline void PushLocalMessage(LocalMessage&& msg)
    {
        localMessages_.push(std::move(msg));
    }

private:
    concurrency::concurrent_queue<LocalMessage, GameAllocator<LocalMessage>> localMessages_;

    void ProcessLocalMessages();
};

END_NS()
