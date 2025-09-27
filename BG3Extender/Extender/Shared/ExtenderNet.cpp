#include <stdafx.h>
#include <Extender/Shared/ExtenderNet.h>

BEGIN_NS(net)

Message* MessageFactory::GetFreeMessage(uint32_t messageId)
{
    if (messageId < MessagePools.size()) {
        EnterCriticalSection(&CriticalSection);
        auto msg = MessagePools[messageId]->GetFreeMessage();
        LeaveCriticalSection(&CriticalSection);
        return msg;
    } else {
        ERR("GetFreeMessage(): Message factory not registered for this message type?");
        return nullptr;
    }
}

void MessageFactory::Grow(uint32_t lastMessageId)
{
    if (MessagePools.size() <= lastMessageId) {
        EnterCriticalSection(&CriticalSection);
        unsigned size = MessagePools.size();
        while (MessagePools.size() <= lastMessageId) {
            MessagePools.push_back(nullptr);
        }
        LeaveCriticalSection(&CriticalSection);
    }
}

void MessageFactory::Register(uint32_t messageId, Message* tmpl)
{
    Grow(messageId);

    auto pool = GameAlloc<MessagePool>();
    pool->Template = tmpl;
    MessagePools[messageId] = pool;
}

Message* MessagePool::GetFreeMessage()
{
    if (Messages.empty()) {
        for (unsigned i = 0; i < GrowSize; i++) {
            Messages.push_back(Template->CreateNew());
        }
    }

    if (Messages.empty()) {
        ERR("GetFreeMessage(): No free space left in message pool");
        return nullptr;
    }

    auto msg = Messages.pop();
    LeasedMessages.push_back(msg);
    return msg;
}


ExtenderProtocolBase::~ExtenderProtocolBase() {}

ProtocolResult ExtenderProtocolBase::ProcessMsg(void * Unused, net::MessageContext * Context, net::Message * Msg)
{
    if (Msg->MsgId == ExtenderMessage::MessageId) {
        auto msg = static_cast<ExtenderMessage*>(Msg);
        if (msg->IsValid()) {
            ProcessExtenderMessage(*Context, msg->GetMessage());
        }
        return ProtocolResult::Handled;
    }

    return ProtocolResult::Unhandled;
}

ProtocolResult ExtenderProtocolBase::PreUpdate(GameTime const& time)
{
    return ProtocolResult::Handled;
}

ProtocolResult ExtenderProtocolBase::PostUpdate(GameTime const& time)
{
    return ProtocolResult::Handled;
}

void ExtenderProtocolBase::OnAddedToHost()
{
}

void ExtenderProtocolBase::OnRemovedFromHost()
{
}

void ExtenderProtocolBase::Reset()
{
}

ExtenderMessage::ExtenderMessage()
{
    MsgId = MessageId;
    Reset();
}

ExtenderMessage::~ExtenderMessage() {}

void ExtenderMessage::Serialize(BitstreamSerializer & serializer)
{
    auto& msg = GetMessage();
    if (serializer.IsWriting) {
        uint32_t size = (uint32_t)msg.ByteSizeLong();
        if (size <= MaxPayloadLength) {
            serializer.WriteBytes(&size, sizeof(size));
            void * buf = GameAllocRaw(size);
            msg.SerializeToArray(buf, size);
            serializer.WriteBytes(buf, size);
            GameFree(buf);
        } else {
            // Zero length indicates that a packet failed to serialize
            uint32_t dummy = 0;
            serializer.WriteBytes(&dummy, sizeof(dummy));
            OsiError("Tried to write packet of size " << size << ", max size is " << MaxPayloadLength);
        }
    } else {
        uint32_t size = 0;
        valid_ = false;
        serializer.ReadBytes(&size, sizeof(size));
        if (size > MaxPayloadLength) {
            OsiError("Tried to read packet of size " << size << ", max size is " << MaxPayloadLength);
        } else if (size > 0) {
            void * buf = GameAllocRaw(size);
            serializer.ReadBytes(buf, size);
            valid_ = msg.ParseFromArray(buf, size);
            GameFree(buf);

            if (!valid_) {
                OsiError("Failed to decode extender message");
            }
        }
    }
}

void ExtenderMessage::Unknown() {}

net::Message * ExtenderMessage::CreateNew()
{
    return new ExtenderMessage();
}

void ExtenderMessage::Reset()
{
#if defined(_DEBUG)
    message_ = new MessageWrapper();
#else
    GetMessage().Clear();
#endif
    valid_ = false;
}

void BaseNetworkManager::Update()
{
    ProcessLocalMessages();
}

void BaseNetworkManager::OnResetExtensionState()
{
    localMessages_.clear();
}

void BaseNetworkManager::ProcessLocalMessages()
{
    OPTICK_EVENT(Optick::Category::Network);
    LocalMessage msg;
    while (localMessages_.try_pop(msg)) {
        HandleLocalMessage(msg);
    }
}

END_NS()
