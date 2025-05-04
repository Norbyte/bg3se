#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

// Base class for all component data
struct BaseComponent
{
};

// Base class for component data that uses external storage
struct BaseProxyComponent : public BaseComponent
{
};

// Base class for all systems
struct BaseSystem : ProtectedGameObject<BaseSystem>
{
    virtual ~BaseSystem() = 0;
    virtual bool VMT01() = 0;
    virtual void SystemLoad() = 0;
    virtual void SystemUnload() = 0;
    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void VMT06() = 0;
};

END_SE()

BEGIN_NS(ecs)

struct EntityWorld;
struct EntityStorageData;

constexpr unsigned ComponentMapSize = 0x880;
constexpr unsigned OneFrameComponentMapSize = 0x280;
constexpr unsigned QueryMapSize = 0xB00;

using ComponentTypeMask = BitArray<uint64_t, ComponentMapSize/64>;
using OneFrameComponentTypeMask = BitArray<uint64_t, OneFrameComponentMapSize /64>;
using QueryMask = BitArray<uint64_t, QueryMapSize/64>;
using EntityTypeMask = BitArray<uint64_t, 4>;

// Component type index, registered statically during game startup
using TComponentTypeIndex = uint16_t;
enum class ComponentTypeIndex : TComponentTypeIndex {};

// Replication component type index, registered statically during game startup
using TReplicationTypeIndex = uint16_t;
enum class ReplicationTypeIndex : TReplicationTypeIndex {};

using TSystemTypeIndex = int32_t;
enum class SystemTypeIndex : TSystemTypeIndex {};

using TQueryIndex = uint16_t;
enum class QueryIndex : TQueryIndex {};

static constexpr ReplicationTypeIndex UndefinedReplicationComponent{ 0xffff };
static constexpr ComponentTypeIndex UndefinedComponent{ 0xffff };
static constexpr SystemTypeIndex UndefinedSystem{ -1 };
static constexpr QueryIndex UndefinedQuery{ 0xffff };

inline constexpr bool IsOneFrame(ComponentTypeIndex idx)
{
    return (TComponentTypeIndex(idx) & 0x8000) == 0x8000;
}

END_NS()

BEGIN_SE()

template <>
inline uint64_t SparseHashMapHash<ecs::ComponentTypeIndex>(ecs::ComponentTypeIndex const& v)
{
    return (ecs::TComponentTypeIndex(v) & 0x7FFF) + (ecs::TComponentTypeIndex(v) >> 15) * ecs::ComponentMapSize;
}

template <>
inline uint64_t HashMapHash<ecs::ComponentTypeIndex>(ecs::ComponentTypeIndex const& v)
{
    auto h0 = ((uint64_t)v & 0x7FFF) + ((uint64_t)v >> 15) * ecs::ComponentMapSize;
    return h0 | (h0 << 16);
}

template <class T>
struct alignas(64) MPMCQueueBounded : public ProtectedGameObject<MPMCQueueBounded<T>>
{
    uint64_t Capacity;
    T* Slots;
    struct alignas(64) {
        uint64_t SomeIndex;
    };
    struct alignas(64) {
        uint64_t Position;
    };
};

END_SE()

BEGIN_NS(ecs)

struct alignas(64) FrameAllocator : public ProtectedGameObject<FrameAllocator>
{
    struct FrameBuffer
    {
        volatile int64_t Offset;
    };

    FrameBuffer* CurrentPage;
    uint64_t PageSize;
    CRITICAL_SECTION CS;
    Array<void*> FrameBuffers[2];

    void* Allocate(uint16_t size);
    FrameBuffer* AllocPage();
    void Free(void* ptr);
};

struct ECBFrameAllocator
{
    inline ECBFrameAllocator(FrameAllocator* allocator)
        : Allocator(allocator)
    {}
    
    inline ECBFrameAllocator(ECBFrameAllocator const& allocator)
        : Allocator(allocator.Allocator)
    {}

    FrameAllocator* Allocator;

    inline void* alloc(std::size_t size)
    {
        se_assert(size <= 0xFFC0);
        return Allocator->Allocate((uint16_t)size);
    }

    template <class T, class ...Args>
    T* allocArray(std::size_t n, Args... args)
    {
        se_assert(n * sizeof(T) <= 0xFFC0);
        auto ptr = static_cast<T*>(Allocator->Allocate((uint16_t)(n * sizeof(T))));
        for (auto i = 0; i < n; i++) {
            new (ptr + i) T(args...);
        }
        return ptr;
    }

    inline void free(void* ptr)
    {
        Allocator->Free(ptr);
    }
};

struct ComponentTypeEntry : public ProtectedGameObject<ComponentTypeEntry>
{
    ComponentTypeIndex TypeId;
    int UncheckedId;
    bool Replicated;
    bool OneFrame;
    bool field_A;
    bool QueryFlags[4];
    uint16_t InlineSize;
    uint16_t ComponentSize;
    void* DtorProc;
#if 0
    void* CountProc;
    void* ModifiedCountProc;
#endif
    Array<ComponentTypeIndex> DependentComponentIndices;
    Array<ComponentTypeIndex> DependencyComponentIndices;
#if 0
    LSStringView Name;
#endif
};

struct StorageComponentMap : public ProtectedGameObject<StorageComponentMap>
{
    EntityTypeMask WriteMask;
    union {
        uint8_t* ComponentIndices;
        std::array<uint8_t, 8> InlineComponentIndices;
    };
    uint8_t Optionals;
    uint8_t OneFrame;
    uint8_t OneFrameOptionals;
    uint8_t FilterOr;
    uint8_t FilterAnd;
    uint8_t NumComponents;
    EntityStorageData* Storage;
    uint16_t StorageId;

    uint8_t GetComponentIndex(uint8_t idx) const
    {
        se_assert(idx < NumComponents);
        if (NumComponents > 8) {
            return ComponentIndices[idx];
        }
        else {
            return InlineComponentIndices[idx];
        }
    }

    uint8_t const* GetComponentIndexPtr(uint8_t idx) const
    {
        se_assert(idx < NumComponents);
        if (NumComponents > 8) {
            return ComponentIndices + idx;
        } else {
            return &InlineComponentIndices[idx];
        }
    }

    inline std::span<uint8_t const> GetComponents() const
    {
        return std::span(GetComponentIndexPtr(0), GetComponentIndexPtr(Optionals));
    }

    inline std::span<uint8_t const> GetOptionals() const
    {
        return std::span(GetComponentIndexPtr(Optionals), GetComponentIndexPtr(OneFrame));
    }

    inline std::span<uint8_t const> GetOneFrameComponents() const
    {
        return std::span(GetComponentIndexPtr(OneFrame), GetComponentIndexPtr(OneFrameOptionals));
    }

    inline std::span<uint8_t const> GetOneFrameOptionals() const
    {
        return std::span(GetComponentIndexPtr(OneFrameOptionals), GetComponentIndexPtr(FilterOr));
    }

    inline std::span<uint8_t const> GetFilterOr() const
    {
        return std::span(GetComponentIndexPtr(FilterOr), GetComponentIndexPtr(FilterAnd));
    }

    inline std::span<uint8_t const> GetFilterAnd() const
    {
        return std::span(GetComponentIndexPtr(FilterAnd), GetComponentIndexPtr(NumComponents));
    }
};

enum class QueryFlags : uint32_t
{
    Modified = 1,
    Added = 2,
    Removed = 4
};

struct QueryDescription : public ProtectedGameObject<QueryDescription>
{
    // TypeList template parameters to ecs::query::spec::Spec<...>
    HashMap<int32_t, StorageComponentMap> EntityStorages;
#if 0
    STDString Name;
#endif
    ComponentTypeIndex* ComponentIndices;
    QueryFlags Flags;
    uint8_t IncludeAny;
    uint8_t Excludes;
    uint8_t AddOrs;
    uint8_t AddAnds;
    uint8_t OneFrames;
    uint8_t OptionalOneFrames;
    uint8_t Optionals;
    uint8_t Immediates;
    uint8_t Writes;
    uint8_t WriteEnd;

    inline std::span<ComponentTypeIndex const> GetIncludes() const
    {
        return std::span(ComponentIndices, ComponentIndices + IncludeAny);
    }

    inline std::span<ComponentTypeIndex const> GetIncludeAny() const
    {
        return std::span(ComponentIndices + IncludeAny, ComponentIndices + Excludes);
    }

    inline std::span<ComponentTypeIndex const> GetExcludes() const
    {
        return std::span(ComponentIndices + Excludes, ComponentIndices + AddOrs);
    }

    inline std::span<ComponentTypeIndex const> GetAddOrs() const
    {
        return std::span(ComponentIndices + AddOrs, ComponentIndices + AddAnds);
    }

    inline std::span<ComponentTypeIndex const> GetAddAnds() const
    {
        return std::span(ComponentIndices + AddAnds, ComponentIndices + OneFrames);
    }

    inline std::span<ComponentTypeIndex const> GetOneFrames() const
    {
        return std::span(ComponentIndices + OneFrames, ComponentIndices + OptionalOneFrames);
    }

    inline std::span<ComponentTypeIndex const> GetOptionalOneFrames() const
    {
        return std::span(ComponentIndices + OptionalOneFrames, ComponentIndices + Optionals);
    }

    inline std::span<ComponentTypeIndex const> GetOptionals() const
    {
        return std::span(ComponentIndices + Optionals, ComponentIndices + Immediates);
    }

    inline std::span<ComponentTypeIndex const> GetImmediates() const
    {
        return std::span(ComponentIndices + Immediates, ComponentIndices + Writes);
    }

    inline std::span<ComponentTypeIndex const> GetWrites() const
    {
        return std::span(ComponentIndices + Writes, ComponentIndices + WriteEnd);
    }

    void* GetFirstMatchingComponent(std::size_t componentSize, bool isProxy);
    void DebugPrint(EntitySystemHelpersBase& eh) const;
};


struct QueryRegistry : public ProtectedGameObject<QueryRegistry>
{
    Array<QueryDescription> Queries;
    Array<QueryIndex> PersistentQueries;
    Array<QueryIndex> AliveQueries;
    Array<QueryIndex> RemovedQueries;
    Array<QueryIndex> ComponentTypes4;
    Array<QueryIndex> ComponentTypes5;
    Array<QueryIndex> FrameData;
};

struct ComponentRegistry : public ProtectedGameObject<ComponentRegistry>
{
    BitSet<> Bitmask;
    StaticArray<ComponentTypeEntry> Types;

    ComponentTypeEntry const* Get(ComponentTypeIndex index) const;
};

struct SystemTypeEntry : public ProtectedGameObject<SystemTypeEntry>
{
    void* System;
    SystemTypeIndex SystemIndex0;
    SystemTypeIndex SystemIndex1;
    __int16 field_10;
    bool Activated;
    void* UpdateProc;
    void* SomeProc2;
    void* SomeProc3;
    void* ECBFlushJob;
    HashSet<SystemTypeIndex> DependencySystems;
    HashSet<SystemTypeIndex> DependentSystems;
    HashSet<uint32_t> HandleMappings2;
    HashSet<uint32_t> HandleMappings;
#if 0
    StringView Name;
    uint64_t field_108;
    QueryMask SomeBitfield;
    QueryMask SomeBitfield2;
#endif
};

struct SystemRegistry : public ProtectedGameObject<SystemRegistry>
{
    void* VMT;
    Array<SystemTypeEntry> Systems;
    uint32_t Unknown;
    uint32_t GrowSize;
};

struct SyncBuffers : public ProtectedGameObject<SyncBuffers>
{
    Array<HashMap<EntityHandle, BitSet<>>> ComponentPools;
    bool Dirty;
};

struct FieldTracker
{
    struct FieldChangeData
    {
        uint64_t Value; // Changes based on field type
        EntityHandle Entity;
        void* Component;
    };

    virtual ~FieldTracker();
    virtual void Add(EntityHandle entity, void* component) = 0;
    virtual void FireEvents() = 0;
    virtual void Remove(EntityHandle entity) = 0;

    Signal<FieldChangeData*> ChangeEvent;
    HashMap<EntityHandle, FieldChangeData> Changes;
};

struct ComponentDeserializer : public ProtectedGameObject<ComponentDeserializer>
{
    virtual ~ComponentDeserializer() = 0;
    // Other functions umapped

    SparseArray<FieldTracker*> FieldTrackers;
};


struct EntityReplicationPeer : public ProtectedGameObject<EntityReplicationPeer>
{
    ecs::EntityWorld* World;
    SyncBuffers** Buffers;
    net::GameClient* Client;
    HashMap<NetId, EntityHandle> NetIdToEntity;
    HashMap<EntityHandle, NetId> EntityToNetId;
    SparseArray<ComponentDeserializer*> Deserializers;
};


struct EntityHandleGenerator : public ProtectedGameObject<EntityHandleGenerator>
{
    struct alignas(64) ThreadState : public ProtectedGameObject<ThreadState>
    {
        struct Entry
        {
            uint32_t Index;
            uint32_t Salt;
        };

        PagedArray<Entry> Entries;
        uint32_t NextFreeSlotIndex;
        uint32_t LastFreeSlotIndex;
        uint32_t NumFreeSlots;

        Entry* Add();
    };

    std::array<ThreadState, 0x40> ThreadStates;

    EntityHandle Create();
    bool IsEntityAlive(EntityHandle entity) const;
};

struct EntityStorageComponentPage
{
    struct ComponentInfo
    {
        void* ComponentBuffer;
        void* ModificationInfo;
    };

    std::array<ComponentInfo, 256> Components;
};
        
struct EntityStorageData : public ProtectedGameObject<EntityStorageData>
{
    static constexpr std::size_t PageSize = 64;

    struct ComponentEntry
    {
        uint16_t Index;
        ComponentTypeIndex ComponentTypeId;
        bool QueryFlag1;
        bool QueryFlag3;
        void* DtorProc;
    };
            
    struct EntityStorageIndex
    {
        uint16_t PageIndex{ 0xffff };
        uint16_t EntryIndex{ 0xffff };

        inline operator bool() const
        {
            return PageIndex != 0xffff;
        }
    };

    struct HandlePage
    {
        EntityHandle Pool[PageSize];
    };

    struct PageInfo
    {
        uint64_t CapacityMask;
        uint64_t UsedMask;
    };


    ComponentTypeMask ComponentsInClass;
    uint64_t EntityTypesMask;
    uint16_t* ComponentSizes;
    ComponentEntry* ComponentDtors;
    uint16_t EntityClassId;
    uint16_t TotalSize;
    uint16_t ComponentIndexListSize;
    bool SomeQueryFlag;
    bool AggregateQueryFlag3;
    bool RegisteredForQueries;
    bool RegisteredForQueries2;
    Array<EntityStorageComponentPage*> Components;
    Array<HandlePage*> Handles;
    Array<PageInfo> PageInfos;
    int16_t field_148;
    int16_t SmallPageIndex;
    Array<uint16_t> PageToComponentPoolIndex;
    HashMap<ComponentTypeIndex, uint8_t> ComponentTypeToIndex;
    HashMap<EntityHandle, EntityStorageIndex> InstanceToPageMap;
    HashMap<uint64_t, uint16_t> AddedComponentFrameStorageIDs;
    HashMap<uint64_t, uint16_t> RemovedComponentFrameStorageIDs2;
    Array<Array<EntityHandle>> ComponentAddedEntityMap;
    Array<Array<EntityHandle>> ComponentRemovedEntityMap;
    HashMap<ComponentTypeIndex, HashMap<EntityHandle, void*>> OneFrameComponents;
    bool HasOneFrameComponents;
    __int64 field_2C8;
    EntityTypeMask ComponentMask; // Valid indices into Components pool
    Array<QueryIndex> RegisteredQueries;
    Array<QueryIndex> AddComponentQueries;
    QueryMask AddComponentQueryMap;
    Array<QueryIndex> RemoveComponentQueries;
    QueryMask RemoveComponentQueryMap;

    void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
    void* GetOneFrameComponent(EntityHandle entityHandle, ComponentTypeIndex type) const;
    void* GetComponent(EntityStorageIndex const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
    void* GetComponent(EntityStorageIndex const& entityPtr, uint8_t componentSlot, std::size_t componentSize, bool isProxy) const;

    inline bool HasComponent(ComponentTypeIndex type) const
    {
        return ComponentsInClass[(uint16_t)type];
    }
};


struct EntityStorageContainer : public ProtectedGameObject<EntityStorageContainer>
{
    struct TypeSalt
    {
        int32_t Salt;
        uint16_t EntityClassIndex;
    };

    struct ThreadSalts : public ProtectedGameObject<ThreadSalts>
    {
        std::array<PagedArray<TypeSalt>, 0x40> Buckets;
        uint32_t Size;
    };

    Array<EntityStorageData*> Entities;
    HashMap<uint64_t, uint16_t> TypeHashToEntityTypeIndex;
    ThreadSalts Salts;
    HashMap<uint64_t, uint64_t> field_458;
    BitSet<> UsedFrameDataStorages;
    ComponentRegistry* ComponentRegistry;
    QueryRegistry* Queries;

    EntityStorageData* GetEntityStorage(EntityHandle entityHandle) const;
};

struct ComponentOps : public ProtectedGameObject<ComponentOps>
{
    virtual ~ComponentOps() = 0;
    virtual void fun_08() = 0;
    virtual void fun_10() = 0;
    virtual void DefaultConstructComponents() = 0;
    virtual void AddImmediateDefaultComponent(uint64_t entity, int retryCount) = 0;

    __int64 field_8;
    __int64 field_10;
    __int64 field_18;
    __int64 field_20;
    __int16 TypeId;
};

struct ComponentPool : public ProtectedGameObject<ComponentPool>
{
    Array<void*> Pages;
    void* GrowProc;
    uint64_t field_18;
    FrameAllocator* Allocator;
    uint32_t NextFreeIndex;
    uint16_t ComponentSize;
    ComponentTypeIndex ComponentTypeId;
    void* DtorProc;
};

using ComponentSignal = Signal<EntityRef*, void*>;

struct ComponentCallbacks : public ProtectedGameObject<ComponentCallbacks>
{
    void* VMT;
    ComponentSignal OnConstruct;
    ComponentSignal OnDestroy;
};

struct ComponentCallbackRegistry : public ProtectedGameObject<ComponentCallbackRegistry>
{
    Array<ComponentCallbacks*> Callbacks;

    ComponentCallbacks* Get(ComponentTypeIndex index);
};

struct ECBEntityComponentChange
{
    // -1 = removed
    EntityStorageData::EntityStorageIndex PoolIndex;
    uint16_t field_4;
    ComponentTypeIndex ComponentTypeId;
};

struct ECBEntityChangeSet
{
    inline ECBEntityChangeSet(FrameAllocator* allocator)
        : Store(4, ECBFrameAllocator(allocator))
    {}

    PagedArray<ECBEntityComponentChange, ECBFrameAllocator> Store;
    uint64_t X{ 0 };
    uint64_t Y{ 0 };
    EntityChangeFlags Flags{ 0 };
    int16_t field_2A{ -1 };
};

struct ComponentFrameStorage
{
    inline ComponentFrameStorage(FrameAllocator* allocator)
        : Pages(3, allocator)
    {}

    PagedArray<void*, ECBFrameAllocator> Pages;
    uint32_t NumComponents{ 0 };
    uint16_t ComponentSizeInBytes{ 0 };
    ComponentTypeIndex ComponentTypeId{ 0 };
    void* DestructorProc{ nullptr };

    inline void* GetComponent(EntityStorageData::EntityStorageIndex const& index) const
    {
        se_assert(index.PageIndex < Pages.size());
        auto page = Pages[index.PageIndex];
        return reinterpret_cast<uint8_t*>(page) + (index.EntryIndex * ComponentSizeInBytes);
    }
};

struct ImmediateWorldCache : public ProtectedGameObject<ImmediateWorldCache>
{
    struct ComponentChange
    {
        void* Ptr{ nullptr };
        EntityStorageData::EntityStorageIndex StorageIndex;
    };

    struct ComponentChanges
    {
        inline ComponentChanges(FrameAllocator* allocator)
            : Components(5, allocator),
            FrameStorage(allocator)
        {}

        PagedHashMap<EntityHandle, ComponentChange, ECBFrameAllocator> Components;
        ComponentFrameStorage FrameStorage;
        void* field_70{ nullptr };
        void* field_78{ nullptr };
    };

    struct Changes
    {
        ComponentTypeMask AvailableComponentTypes;
        ComponentChanges* ComponentsByType;
        uint64_t Unknown;

        void* GetChange(EntityHandle entityHandle, ComponentTypeIndex type) const;
        ComponentChanges* AddComponentChanges(ComponentTypeEntry const* type, FrameAllocator* allocator);
    };

    Changes WriteChanges;
    Changes ReadChanges;
    ComponentCallbackRegistry* Callbacks;
    FrameAllocator* Allocator;
    EntityWorld* EntityWorld;
    EntityHandleGenerator* HandleGenerator;
    __int64 field_158;

    ComponentChanges* AddComponentChanges(ComponentTypeIndex type);
    bool RemoveComponent(EntityHandle entity, ComponentTypeIndex type);
};

struct ECBData : public ProtectedGameObject<ECBData>
{
    PagedHashMap<EntityHandle, ECBEntityChangeSet, ECBFrameAllocator> EntityChanges;
    DoubleIndexedPagedArray<ComponentTypeIndex, ComponentFrameStorage, ECBFrameAllocator> ComponentPools;
    bool field_A0;

    ECBEntityChangeSet const* GetEntityChange(EntityHandle const& entity) const;
    ECBEntityChangeSet* GetOrAddEntityChange(EntityHandle const& entity);
};

struct EntityCommandBuffer : public ProtectedGameObject<EntityCommandBuffer>
{
    EntityHandleGenerator* HandleGenerator;
    FrameAllocator* Allocator;
    ECBData Data;
    uint32_t ThreadId;

    EntityHandle CreateEntity();
    EntityHandle CreateEntityImmediate();
    bool DestroyEntity(EntityHandle entity);
    void* GetComponentChange(ComponentTypeIndex type, EntityStorageData::EntityStorageIndex const& index) const;
};

struct GroupAllocator : public ProtectedGameObject<GroupAllocator>
{
    CriticalSection CS;
#if 0
    uint64_t PendingAcquire;
    uint64_t PendingRelease;
#endif
};

struct EntityWorldSettings
{
    int JobPriority;
    int UpdateJobPriority;
#if 0
    // FIXME - check if correct?
    STDString Path;
    __int64 field_20;
    uint8_t field_28;
    uint8_t field_30;
#endif
};

struct ComponentOpsRegistry
{
    Array<ComponentOps*> Ops;

    ComponentOps* Get(ComponentTypeIndex id) const;
};

struct EntityStorageChangeBatch
{
    PagedHashMap<EntityHandle, ECBEntityChangeSet, ECBFrameAllocator> EntityChanges;
    EntityStorageContainer* Storage;
    FrameAllocator* FrameAllocator;
    SparseArray<uint16_t> OneFrameComponentTypes;
    __int64 field_78;
    ComponentTypeMask QueryFlag0Mask;
    ComponentTypeMask QueryFlag1Mask;
    BitSet<> field_280;
};


struct ECBExecutor
{
    EntityHandleGenerator* HandleGenerator;
    EntityStorageContainer* Storage;
    ComponentRegistry* ComponentRegistry;
    ComponentOpsRegistry* ComponentOps;
    ComponentTypeMask RemovedComponentsMask;
    ComponentTypeMask AddedComponentsMask;
    OneFrameComponentTypeMask AddedOneFrameComponentsMask;
    Array<ComponentPool> ComponentPools;
    PagedHashMap<EntityHandle, ECBEntityChangeSet, ECBFrameAllocator> EntityChanges;
    __int64 field_2B8;
    EntityStorageChangeBatch ChangeBatch;
    FrameAllocator* FrameAllocator;
    void* ScratchMemory;
};


struct EntityWorld : public ProtectedGameObject<EntityWorld>
{
    using UpdateProc = void (EntityWorld* self, GameTime const& time);
    using FlushECBsProc = bool (EntityWorld* self);

    SyncBuffers* Replication;
    ComponentRegistry ComponentRegistry_;
    SystemRegistry Systems;
    Array<SystemTypeEntry*> ActiveSystems;
    Array<void*> ECBFlushJobs;
    void* SystemDependencyExecutor;
    MPMCQueueBounded<void*> DependencyExecutorQueue;
    GameTime Time;
    void* ECSUpdateBatch;
    void* field_160x;
    int field_160;
    Array<EntityCommandBuffer> CommandBuffers;
    ComponentCallbackRegistry ComponentCallbacks;
    bool RegisterPhaseEnded;
    bool Active;
    bool NeedsOptimize;
    bool PerformingECSUpdate;
    QueryRegistry Queries;
    EntityHandleGenerator* HandleGenerator;
    EntityStorageContainer* Storage;
    FrameAllocator ComponentData;
    Array<void*> SystemDependencyExecutors;
    ComponentOpsRegistry ComponentOps;
    ScratchString field_2E0;
    ECBExecutor* Executor;
    GroupAllocator GroupAllocator;
    ImmediateWorldCache* Cache;
    EntityWorldSettings Settings;
#if 0
    uint32_t DebugUpdateFrame;
    void* LegacyWorldViewChecker;
    CRITICAL_SECTION CS2;
#endif

    void* GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy);

    EntityStorageData* GetEntityStorage(EntityHandle entityHandle) const;
    bool IsValid(EntityHandle entityHandle) const;
    EntityCommandBuffer* Deferred();
};

struct PeerReplicationData
{
    PeerId PeerID;
    HashSet<EntityHandle> ReplicatedToPeer;
    HashSet<EntityHandle> RequestedForReplication;
    HashSet<EntityHandle> RequestedToStopReplication;
    HashMap<EntityHandle, BitSet<>> Components;
    HashMap<EntityHandle, BitSet<>> Components2;
};

struct NetIdGenerator
{
    PagedArray<NetId> AssignedNetIds;
    NetId NextFreeNetId;
    NetId LastFreeNetId;
    int FreeSlots;
};

struct EntitiesDirtyFieldsBuffer
{
    Array<HashMap<EntityHandle, BitSet<>>> field_0;
    bool field_10;
};

struct EntitiesDirtyCollectionIndicesBuffer
{
    BitSet<> field_0;
    Array<void*> field_10;
};

struct EntityReplicationAuthority : public ProtectedGameObject<EntityReplicationAuthority>
{
    net::Host* Host;
    EntityWorld* World;
    EntitiesDirtyFieldsBuffer DirtyFields;
    EntitiesDirtyCollectionIndicesBuffer DirtyCollectionIndices;
    NetIdGenerator NetIdGenerator;
    SparseArray<void*> field_70_IComponentSerializer;
    HashMap<EntityHandle, NetId> EntityToNetId;
    HashMap<NetId, EntityHandle> NetIdToEntity;
    HashMap<EntityHandle, BitSet<>> field_110;
    HashSet<EntityHandle> ReplicateEntities;
    HashSet<EntityHandle> StopReplicateEntities;
    SparseArray<PeerReplicationData> ReplicationData;
    Array<PeerId> Peers;
    Array<PeerId> ConnectedPeers;
    Array<void*> field_1F0_pIComponentSubscription;
    HashMap<PeerId, void*> field_200_PeerID_pECSReplicationMessage;
};


END_NS()
