#pragma once

#include <GameDefinitions/Net.h>
#include <Extender/Shared/ExtenderProtocol.pb.h>

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
};

#if defined(__APPLE__)
class MacOSExtenderProtocol : public ExtenderProtocolBase
{
public:
    ~MacOSExtenderProtocol() override;

    ProtocolResult ProcessMsg(void * unused, MessageContext * unknown, Message* usg) override;
    ProtocolResult PreUpdate(GameTime const& time) override;
    ProtocolResult PostUpdate(GameTime const& time) override;
    void OnAddedToHost() override;
    void OnRemovedFromHost() override;
    void Reset() override;

protected:
    void ProcessExtenderMessage(net::MessageContext& context, MessageWrapper & msg) override;
};
#endif

END_NS()
