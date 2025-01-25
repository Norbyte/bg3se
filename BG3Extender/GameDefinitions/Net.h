#pragma once

#include <windows.h>
#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
// Temporarily, for ComponentCallbakcList
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(net)

struct Bitstream : ProtectedGameObject<Bitstream>
{
    uint32_t NumBits;
    uint32_t AllocatedBits;
    uint32_t CurrentOffsetBits;
    uint8_t* Buf;
    bool Unknown;
};

struct BitstreamSerializer : ProtectedGameObject<BitstreamSerializer>
{
    virtual void Unknown() = 0;
    virtual void WriteBytes(void const* Buf, uint64_t Size) = 0;
    virtual void ReadBytes(void* Buf, uint64_t Size) = 0;

    uint32_t IsWriting;
    Bitstream* Bitstream;
};

template <class T>
struct QueueCS : public Queue<T>
{
    QueueCS()
    {
        InitializeCriticalSection(&CriticalSection);
    }
    
    ~QueueCS()
    {
        DeleteCriticalSection(&CriticalSection);
    }

    CRITICAL_SECTION CriticalSection;
};

struct Message : Noncopyable<Message>
{
    using SerializeProc = void (Message* msg, BitstreamSerializer* serializer);

    struct VMT
    {
        void* Dtor;
        void* Serialize;
        void* Unknown;
        void* CreateNew;
        void* Reset;
    };

    inline virtual ~Message() {}
    virtual void Serialize(BitstreamSerializer & serializer) = 0;
    virtual void Unknown() = 0;
    virtual Message * CreateNew() = 0;
    virtual void Reset() = 0;

    NetMessage MsgId;
    uint32_t Always4{ 4 };
    uint32_t MsgType{ 1 };
    uint8_t Always0{ 0 };
    uint8_t Unknown1{ 0 };
    uint64_t ReceivedAtTime{ 0 };
    uint32_t ReceiveTimeDelta{ 0 };
    uint32_t Unknown4{ 0 };
};

struct MessagePool : Noncopyable<MessagePool>
{
    Message* Template{ nullptr };
    uint64_t GrowSize{ 1 };
    Queue<Message*> Messages;
    Array<Message*> LeasedMessages;

    Message* GetFreeMessage();
};

struct MessageFactory : ProtectedGameObject<MessagePool>
{
    void* VMT;
    Array<MessagePool*> MessagePools;
    uint32_t UsedPoolSlots;
    uint32_t WriteOffset;
    CRITICAL_SECTION CriticalSection;

    Message* GetFreeMessage(uint32_t messageId);
    void Grow(uint32_t lastMessageId);
    void Register(uint32_t messageId, Message* tmpl);
};

struct MessageContext
{
    UserId UserID;
    Array<PeerId> PeerIDClassNames;
    Array<UserId> UserIDs;
    Message * Msg;
    uint32_t Unknown1;
    uint8_t Unknown2;
};

enum class ProtocolResult : int32_t
{
    Unhandled = 0,
    Handled = 1,
    Abort = 2,
    AbortAndDisconnect = 3
};

struct Protocol
{
    inline virtual ~Protocol() {}
    virtual ProtocolResult ProcessMsg(void * Unused, MessageContext * Unknown, Message * Msg) = 0;
    virtual ProtocolResult PreUpdate(GameTime const& time) = 0;
    virtual ProtocolResult PostUpdate(GameTime const& time) = 0;
    virtual void OnAddedToHost() = 0;
    virtual void OnRemovedFromHost() = 0;
    virtual void Reset() = 0;

    AbstractPeer * Peer;
};

struct PacketHandlers : ProtectedGameObject<PacketHandlers>
{
    LegacyRefMap<int32_t, void*> field_0;
    AbstractPeer* Peer;
    double LastUpdate;
    __int64 field_20;
    __int64 field_28;
    __int64 field_30;
    __int64 field_38;
};

struct AbstractPeerVMT
{
    typedef void (* SendToPeerProc)(AbstractPeer *, int32_t *, Message *);
    typedef void (* SendToMultiplePeersProc)(AbstractPeer *, ObjectSet<int32_t> *, Message *, int32_t);
    typedef void (* ClientSendProc)(AbstractPeer *, int32_t, Message *);

    uint64_t Unknown[27];
    SendToPeerProc SendToPeer;
    uint64_t Unknown2[3];
    SendToMultiplePeersProc SendToMultiplePeers;
    ClientSendProc ClientSend;
};

struct AbstractPeerBase : ProtectedGameObject<AbstractPeerBase>
{
    virtual void OnNewIncomingConnection() = 0;
    virtual void OnConnectionRequestAccepted(void*) = 0;
    virtual void OnConnectionLost(void*) = 0;
    virtual void OnConnectionAttemptFailed(void*) = 0;
    virtual void OnPunchThroughFailed(void*) = 0;
    virtual void OnPunchThroughSuccess(void*) = 0;
    virtual void fun_30(void*) = 0;
    virtual void fun_38(void*) = 0;
    virtual void fun_40() = 0;
    virtual void fun_48() = 0;
    virtual void fun_50() = 0;
    virtual void fun_58() = 0;
    virtual void fun_60() = 0;
    virtual void fun_68() = 0;
    virtual void fun_70() = 0;
    virtual void Shutdown() = 0;
    virtual void Init(uint16_t port, uint16_t port2, uint32_t socketType, void*, void*) = 0;
    virtual void Disconnect(uint32_t TPeerId) = 0;
    virtual void CreateConnection(STDString*) = 0;
    virtual void CreateConnection2(void* address) = 0;
    virtual void RequestDisconnect(TPeerId peerId, int delay, int delayInfo) = 0;
    virtual void PreUpdate(GameTime const&) = 0;
    virtual void PostUpdate(GameTime const&) = 0;
    virtual void SendMessageMultiPeerMoveIds(Array<PeerId>& recipients, Message* message, TPeerId excludePeerId) = 0;
    virtual void SendMessageSinglePeer(TPeerId peerId, Message* message) = 0;
    virtual ProtocolResult ProcessMsg(void* unknown, void* context, Message* message) = 0;
    virtual bool ConnectToNatPunchServer(void*, STDString*) = 0;
    virtual bool fun_110() = 0;
};

struct AbstractPeer : public AbstractPeerBase
{
    void* VMT2;
    void* VMT3;
    void* VMT4;
    void* RakNetPeer;
    int SocketBindingType_M;
    int PeerAddressID;
    STDString PeerHost1;
    uint16_t PeerPort1;
    STDString PeerHost2;
    void* field_58;
    void* field_60;
    HashMap<int32_t, void*> field_68;
    HashMap<int32_t, void*> field_A8;
    bool field_E8;
    uint8_t _Pad1[7];
    bool field_F0;
    PacketHandlers Handlers;
    int MessageContextPool;
    QueueCS<Message*> ImmediateIncomingMessages;
    QueueCS<Message*> DeferredIncomingMessages;
    QueueCS<Message*> OutgoingMessages;
    Array<TPeerId> PeerIdClassName;
    Array<void*> DisconnectDelays;
    void* SocketLayerOverride;
    MessageFactory *NetMessageFactory;
    void *AddressManager;
    void *NetEventManager;
    void *RakNetNatPunchthroughClient;
    void *RakNetNatTypeDetectionClient;
    void *FullyConnectedMesh;
    bool UseMesh;
    void *NatPunchDebugInterface;
    int PeerAddressID2;
    GameTime LastTickTime;
    int field_288;
    uint16_t field_28C;
    uint16_t field_28E;
    bool DisconnectRequested;
    bool WasInitialized;
    bool IsTickingProtocols;
    bool NextIsTickingProtocols;
    uint32_t NextTickTime;
    uint32_t MaxConnectRetries;
    uint32_t UpdateInterval;
    STDString RemoteHost1;
    uint16_t RemotePort;
    STDString RemoteHost2;
    bool NeedsConnecting;
    int RetryIndex;
    double LastUpdateTime;
    void *EventVisitor;
    CRITICAL_SECTION FireEventsCriticalSection;
    bool FirePeerEvents;
    bool field_319;
    uint16_t Port1;
    uint16_t Port2;
    Array<Protocol*> ProtocolList;
    HashMap<uint32_t, Protocol*> ProtocolMap;
    QueueCS<Message*> PacketsToSend;
    QueueCS<Message*> Ring2;
    QueueCS<Message*> Ring3;
    uint64_t field_440;
    uint8_t field_448;
};

struct Host : public AbstractPeer
{
    void* GameServerVMT2;
};

template <class T>
struct SaltMap : public ProtectedGameObject<SaltMap<T>>
{
    T** Buckets;
    uint16_t BitsPerBucket;
    uint16_t NumBuckets;
    uint32_t NumElements;
};

struct GameServer : public Host
{
    struct Inner
    {
        __int64 field_0;
        __int64 field_8;
        Array<void*> field_10;
        char field_20;
        BitSet<> field_28;
        __int64 field_38;
        __int64 field_40;
        SaltMap<void*> field_48;
        __int64 field_58;
        __int64 field_60;
        int field_68;
        BitSet<> field_70;
        Array<void*> field_80;
        HashMap<EntityHandle, void*> field_90;
        HashMap<EntityHandle, void*> field_D0;
        HashMap<EntityHandle, void*> field_110;
        HashSet<EntityHandle> field_150;
        HashSet<EntityHandle> field_180;
        BitSet<> field_1B0;
        Array<void*> field_1C0;
        Array<void*> field_1D0;
        Array<void*> field_1E0;
        Array<void*> field_1F0;
        HashMap<int32_t, void*> field_200;
    };

    struct Inner2
    {
        Array<void*> field_0;
        Array<void*> field_10;
        Array<void*> field_20;
        LegacyMap<int32_t, void*> field_30;
    };

    void* VMT3;
    void* field_3C0;
    void* VMT4;
    Inner InnerObj;
    void* LobbyManager;
    ecs::EntityWorld* EntityWorld;
    Array<PeerId> ConnectedPeerIds;
    Inner2 InnerObj2;
    Array<PeerId> ActivePeerIds;
    Array<PeerId> SomePeerIds3;
    Array<PeerId> SomePeerIds;
    Array<PeerId> KickedPeerIds;
    int field_6B8;
    int field_6BC;
    __int64 field_6C0;
    __int64 field_6C8;
    __int64 field_6D0;
    int ServerState;
    int AcceptingClientId;
    __int64 field_6E0;
    __int64 field_6E8;
    __int64 field_6F0;
    __int64 field_6F8;
    __int64 field_700;
    __int64 field_708;
    __int64 field_710;
    __int64 field_718;
    __int64 field_720;
};

struct Client : public AbstractPeer
{
    void* VMT5;
    PeerId HostPeerId;
};

struct GameClient : public Client
{
    UnknownSignal OnPlayerConnected;
    UnknownSignal OnPlayerDisconnected;
    UnknownSignal field_508;
    UnknownSignal field_520;
    ecs::EntityReplicationPeer ReplicationPeer;
    void* LobbyManager;
    uint32_t Status;
    bool field_5FC;
    void* VoiceChatPeer;
    Array<PeerId> ClientPeers;
    Array<UserId> Users;
    Array<uint64_t> UserIdToUserSlot;
    Array<uint64_t> UserSlotToUserId;
    LegacyRefMap<UserId, STDString> UserToName;
    LegacyRefMap<UserId, uint64_t> UserToCommunityUserId;
    __int64 field_668;
    uint32_t LastAttemptNo;
};


struct ClientConnectMessage : public Message
{
    STDString GameVersion;
    STDString Build;
    int field_58;
    char field_5C;
    Guid field_60;
    uint8_t field_70;
    HashSet<Guid> field_78;
    int field_A8;
    uint8_t field_AC;
};

END_NS()
