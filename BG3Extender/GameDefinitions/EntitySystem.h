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

END_SE()

BEGIN_NS(ecs)

struct EntityWorld;
struct EntityStorageData;

using ComponentTypeMask = BitArray<uint64_t, 32>;
using QueryMask = BitArray<uint64_t, 40>;
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
	return (ecs::TComponentTypeIndex(v) & 0x7FFF) + (ecs::TComponentTypeIndex(v) >> 15 << 11);
}

template <>
inline uint64_t HashMapHash<ecs::ComponentTypeIndex>(ecs::ComponentTypeIndex const& v)
{
	auto h0 = ((uint64_t)v & 0x7FFF) + ((uint64_t)v >> 15 << 11);
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
	__int64 FastLock;
	__int64 FastLock2;
	CRITICAL_SECTION field_10;
	Array<void*> Pages[2];
};

struct ECBFrameAllocator
{
	FrameAllocator* Allocator;
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
	uint8_t OptionalStart;
	uint8_t OptionalEnd;
	uint8_t FrameOptionalsStart;
	uint8_t FrameOptionalsEnd;
	uint8_t FilterOrEnd;
	uint8_t FilterAndEnd;
	EntityStorageData* Storage;
	uint16_t StorageId;

	uint8_t GetComponentIndex(uint8_t idx) const
	{
		assert(idx < FilterAndEnd);
		if (FilterAndEnd > 8) {
			return ComponentIndices[idx];
		} else {
			return InlineComponentIndices[idx];
		}
	}
};

struct QueryDescription : public ProtectedGameObject<QueryDescription>
{
	// TypeList template parameters to ecs::query::spec::Spec<...>
	HashSet<int32_t> StorageFrameIDs;
	StaticArray<StorageComponentMap> EntityClasses;
#if 0
	STDString Name;
#endif
	uint64_t* ComponentIndices;
	uint32_t Flags;
	uint8_t IncludeEnd;
	uint8_t IncludeAndEnd;
	uint8_t ExcludeEnd;
	uint8_t AddOrEnd;
	uint8_t FrameStart;
	uint8_t FrameOptionalStart;
	uint8_t OptionalStart;
	uint8_t OptionalEnd;
	uint8_t WriteStart;
	uint8_t WriteEnd;

	void* GetFirstMatchingComponent(std::size_t componentSize, bool isProxy);
	Array<void*> GetAllMatchingComponents(std::size_t componentSize, bool isProxy);
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
	Array<ComponentTypeEntry> Types;

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

struct EntityHandleGenerator : public ProtectedGameObject<EntityHandleGenerator>
{
	struct alignas(64) ThreadState : public ProtectedGameObject<ThreadState>
	{
		struct Entry
		{
			uint32_t Index;
			uint32_t Salt;
		};

		PagedArray<Entry> Salts;
		uint32_t NextFreeSlotIndex;
		uint32_t HighestIndex;
		uint32_t NumFreeSlots;

		uint64_t Add();
		void Grow();
	};

	std::array<ThreadState, 0x40> ThreadStates;

	EntityHandle Add(uint32_t classIndex);
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
		uint16_t PageIndex;
		uint16_t EntryIndex;
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

	struct SaltMap : public ProtectedGameObject<SaltMap>
	{
		std::array<PagedArray<TypeSalt>, 0x40> Buckets;
		uint32_t Size;
	};

	Array<EntityStorageData*> Entities;
	HashMap<uint64_t, uint16_t> TypeHashToEntityTypeIndex;
	SaltMap Salts;
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

struct ComponentCallbackParams
{
	EntityHandle Entity;
	EntityWorld* World;
};

struct ComponentCallbackHandler
{
	using CallProc = void (ComponentCallbackHandler const& self, ComponentCallbackParams const& params, void* component);
	using UserCallProc = void (void* object, ComponentCallbackParams const& params, void* component);
	using CopyProc = ComponentCallbackHandler* (void* dummy, ComponentCallbackHandler const& src, ComponentCallbackHandler* dst);
	using MoveProc = ComponentCallbackHandler* (void* dummy, ComponentCallbackHandler& src, ComponentCallbackHandler* dst);

	static void DefaultCall(ComponentCallbackHandler const& self, ComponentCallbackParams const& arg, void* component);
	static ComponentCallbackHandler* DefaultCopy(void* dummy, ComponentCallbackHandler const& src, ComponentCallbackHandler* dst);
	static ComponentCallbackHandler* DefaultMoveDtor(void* dummy, ComponentCallbackHandler& src, ComponentCallbackHandler* dst);

	ComponentCallbackHandler();
	ComponentCallbackHandler(UserCallProc* handler, void* context);

	CallProc* Call;
	CopyProc* Copy;
	MoveProc* MoveDtor;
	void* Object;
	UserCallProc* UserHandler;
};


struct ComponentCallback
{
	ComponentCallback();
	~ComponentCallback();
	ComponentCallback(ComponentCallbackHandler const& handler, uint64_t index);
	ComponentCallback(ComponentCallback const&);
	ComponentCallback(ComponentCallback&&) noexcept;

	ComponentCallback& operator = (ComponentCallback const&);
	ComponentCallback& operator = (ComponentCallback&&) noexcept;

	ComponentCallbackHandler* pHandler{ nullptr };
	ComponentCallbackHandler Handler;
	uint64_t Unused1{ 0 };
	uint64_t Unused2{ 0 };
	uint64_t RegistrantIndex{ 0 };
};

struct ComponentCallbackList : public ProtectedGameObject<ComponentCallbackList>
{
	uint64_t NextRegistrantId;
	Array<ComponentCallback> Callbacks;

	uint64_t Add(ComponentCallbackHandler const& handler);
	bool Remove(uint64_t registrantIndex);
};

struct ComponentCallbacks : public ProtectedGameObject<ComponentCallbacks>
{
	void* VMT;
	ComponentCallbackList OnConstruct;
	ComponentCallbackList OnDestroy;
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
	PagedArray<ECBEntityComponentChange, ECBFrameAllocator> Store;
	uint64_t X;
	uint64_t Y;
	EntityChangeFlags Flags;
	__int16 field_2A;
};

struct ComponentFrameStorage
{
	PagedArray<void*, ECBFrameAllocator> Components;
	uint32_t NumComponents;
	uint16_t ComponentSizeInBytes;
	ComponentTypeIndex ComponentTypeId;
	void* DestructorProc;
};

struct ImmediateWorldCache : public ProtectedGameObject<ImmediateWorldCache>
{
	struct ComponentChange
	{
		void* Ptr;
		EntityStorageData::EntityStorageIndex StorageIndex;
	};

	struct ComponentChanges
	{
		PagedHashMap<EntityHandle, ComponentChange, ECBFrameAllocator> Components;
		ComponentFrameStorage FrameStorage;
		void* field_70;
		void* field_78;
	};

	struct Changes
	{
		ComponentTypeMask AvailableComponentTypes;
		ComponentChanges* ComponentsByType;
		uint64_t Unknown;

		void* GetChange(EntityHandle entityHandle, ComponentTypeIndex type) const;
	};

	Changes WriteChanges;
	Changes ReadChanges;
	Array<ComponentCallbacks*>* Callbacks;
	FrameAllocator* Allocator;
	EntityWorld* EntityWorld;
	EntityHandleGenerator* HandleGenerator;
	__int64 field_158;
};

struct ECBData : public ProtectedGameObject<ECBData>
{
	PagedHashMap<EntityHandle, ECBEntityChangeSet, ECBFrameAllocator> EntityChanges;
	DoubleIndexedPagedArray<ComponentTypeIndex, ComponentFrameStorage, ECBFrameAllocator> ComponentPools;
	bool field_A0;
};

struct EntityCommandBuffer : public ProtectedGameObject<EntityCommandBuffer>
{
	EntityHandleGenerator* HandleGenerator;
	FrameAllocator* Allocator;
	ECBData Data;
	uint32_t ThreadId;
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
	int field_160;
	Array<EntityCommandBuffer> CommandBuffers;
	Array<ComponentCallbacks*> ComponentCallbacks;
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
	void* ECBExecutor;
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
};

END_NS()
