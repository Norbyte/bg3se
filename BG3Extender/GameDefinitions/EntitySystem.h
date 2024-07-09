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

using ComponentTypeMask = std::array<uint64_t, 32>;
using QueryMask = std::array<uint64_t, 40>;
using EntityTypeMask = std::array<uint64_t, 4>;

// Component type index, registered statically during game startup
enum class ComponentTypeIndexTag {};
using ComponentTypeIndex = TypedIntegral<uint16_t, ComponentTypeIndexTag>;
// Replication component type index, registered statically during game startup
enum class ReplicationTypeIndexTag {};
using ReplicationTypeIndex = TypedIntegral<uint16_t, ReplicationTypeIndexTag>;

static constexpr ReplicationTypeIndex UndefinedReplicationComponent{ 0xffff };
static constexpr ComponentTypeIndex UndefinedComponent{ 0xffff };

END_NS()

BEGIN_SE()

template <>
inline uint64_t SparseHashMapHash<ecs::ComponentTypeIndex>(ecs::ComponentTypeIndex const& v)
{
	return (v.Value() & 0x7FFF) + (v.Value() >> 15 << 11);
}

template <>
inline uint64_t MultiHashMapHash<ecs::ComponentTypeIndex>(ecs::ComponentTypeIndex const& v)
{
	return ((v.Value() & 0x7FFF) + 0x780 * (v.Value() >> 15))
		| (uint64_t)(((v.Value() & 0x7FFF) + 0x780 * (v.Value() >> 15)) << 16);
}


template <class T>
struct PagedArray
{
	T** Buckets;
	uint16_t BitsPerBucket;
	uint16_t NumBuckets;
	uint32_t Used;

	void Resize(uint32_t newSize)
	{
		if (newSize) {
			auto bucketSize = 1u << BitsPerBucket;
			auto newNumBuckets = (~bucketSize & (bucketSize + newSize - 1)) >> BitsPerBucket;
			if (newNumBuckets != NumBuckets) {
				for (auto i = newNumBuckets; i < NumBuckets; i++) {
					GameFree(Buckets[i]);
				}

				auto newAllocBuckets = (newNumBuckets + 15) & 0xFFF0;
				if (newAllocBuckets < NumBuckets) {
					auto newBuckets = GameAllocArray<T*>(newAllocBuckets);
					if (Buckets) {
						if ((unsigned int)NumBuckets < NumBuckets)
							NumBuckets = NumBuckets;
						memcpy(newBuckets, Buckets, sizeof(T*) * std::min(newNumBuckets, (uint32_t)NumBuckets));
						GameFree(Buckets);
					}
					Buckets = newBuckets;
				}

				for (auto i = NumBuckets; i < newNumBuckets; i++) {
					Buckets[i] = GameAllocArray<T>(bucketSize);
				}

				NumBuckets = newNumBuckets;
			}
		} else {
			if (NumBuckets) {
				for (auto i = 0; i < NumBuckets; i++) {
					GameFree(Buckets[i]);
				}

				if (Buckets != nullptr) {
					GameFree(Buckets);
				}
			}

			NumBuckets = 0;
		}
	}

	inline T const& operator [] (uint32_t index) const
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}

	inline T& operator [] (uint32_t index)
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}
};

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

struct ComponentTypeEntry : public ProtectedGameObject<ComponentTypeEntry>
{
	ComponentTypeIndex TypeId;
	int NameId;
	uint8_t field_8;
	uint8_t field_9;
	uint8_t field_A;
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
	uint64_t field_50;
	uint32_t field_58;
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
	using ID = uint16_t;

	// TypeList template parameters to ecs::query::spec::Spec<...>
	MultiHashSet<int32_t> StorageFrameIDs;
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
	Array<QueryDescription::ID> PersistentQueries;
	Array<QueryDescription::ID> AliveQueries;
	Array<QueryDescription::ID> RemovedQueries;
	Array<QueryDescription::ID> ComponentTypes4;
	Array<QueryDescription::ID> ComponentTypes5;
	Array<QueryDescription::ID> FrameData;
};

struct ComponentRegistry : public ProtectedGameObject<ComponentRegistry>
{
	BitSet<> Bitmask;
	Array<ComponentTypeEntry> Types;
};

struct SystemTypeEntry : public ProtectedGameObject<SystemTypeEntry>
{
	using ID = uint32_t;

	void* System;
	int32_t SystemIndex0;
	int32_t SystemIndex1;
	__int16 field_10;
	bool Activated;
	void* UpdateProc;
	void* SomeProc2;
	void* SomeProc3;
	void* ECBFlushJob;
	MultiHashSet<SystemTypeEntry::ID> DependencySystems;
	MultiHashSet<SystemTypeEntry::ID> DependentSystems;
	MultiHashSet<uint32_t> HandleMappings2;
	MultiHashSet<uint32_t> HandleMappings;
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
	Array<MultiHashMap<EntityHandle, BitSet<>>> ComponentPools;
	bool Dirty;
};

struct EntityHandleGenerator : public ProtectedGameObject<EntityHandleGenerator>
{
	struct alignas(64) ThreadState : public ProtectedGameObject<ThreadState>
	{
		struct Entry
		{
			int Index;
			int Salt;
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
			
	struct ComponentPage
	{
		void* ComponentBuffer;
		void* B;
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
	Array<ComponentPage*> Components;
	Array<HandlePage*> Handles;
	Array<PageInfo> PageInfos;
	int16_t field_148;
	int16_t SmallPageIndex;
	Array<uint16_t> PageToComponentPoolIndex;
	MultiHashMap<ComponentTypeIndex, uint8_t> ComponentTypeToIndex;
	MultiHashMap<EntityHandle, EntityStorageIndex> InstanceToPageMap;
	MultiHashMap<uint64_t, uint16_t> AddedComponentFrameStorageIDs;
	MultiHashMap<uint64_t, uint16_t> RemovedComponentFrameStorageIDs2;
	Array<Array<EntityHandle>> ComponentAddedEntityMap;
	Array<Array<EntityHandle>> ComponentRemovedEntityMap;
	// FIXME - SparseArray<ComponentTypeEntry> instead?
	MultiHashMap<ComponentTypeIndex, MultiHashMap<uint16_t, ComponentPage*>*> ComponentPoolsByType;
	bool HasComponentPoolsByType;
	__int64 field_2C8;
	EntityTypeMask ComponentMask; // Valid indices into Components pool
	Array<uint16_t> RegisteredQueries;
	Array<uint16_t> AddComponentQueries;
	QueryMask AddComponentQueryMap;
	Array<uint16_t> RemoveComponentQueries;
	QueryMask RemoveComponentQueryMap;

	void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
	void* GetComponent(EntityStorageIndex const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
	void* GetComponent(EntityStorageIndex const& entityPtr, uint8_t componentSlot, std::size_t componentSize, bool isProxy) const;

	inline bool HasComponent(ComponentTypeIndex type) const
	{
		return (ComponentsInClass[(uint16_t)type >> 6] & (1ull << ((uint16_t)type & 0x3f))) != 0;
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
	MultiHashMap<uint64_t, uint16_t> TypeHashToEntityTypeIndex;
	SaltMap Salts;
	MultiHashMap<uint64_t, uint64_t> field_458;
	BitSet<> UsedFrameDataStorages;
	ComponentRegistry* ComponentRegistry;
	QueryRegistry* Queries;

	EntityStorageData* GetEntityStorage(EntityHandle entityHandle) const;
};

struct alignas(64) FrameAllocator : public ProtectedGameObject<FrameAllocator>
{
	__int64 FastLock;
	__int64 FastLock2;
	CRITICAL_SECTION field_10;
	Array<void*> Pages[2];
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

template <class T>
struct BucketedRawStaticArray
{
	T** Buckets;
	uint16_t BitsPerBucket;
	uint16_t NumBuckets;
	uint32_t Used;

	void Resize(uint32_t newSize)
	{
		if (newSize) {
			auto bucketSize = 1u << BitsPerBucket;
			auto newNumBuckets = (~bucketSize & (bucketSize + newSize - 1)) >> BitsPerBucket;
			if (newNumBuckets != NumBuckets) {
				for (auto i = newNumBuckets; i < NumBuckets; i++) {
					GameFree(Buckets[i]);
				}

				auto newAllocBuckets = (newNumBuckets + 15) & 0xFFF0;
				if (newAllocBuckets < NumBuckets) {
					auto newBuckets = GameAllocArray<T*>(newAllocBuckets);
					if (Buckets) {
						if ((unsigned int)NumBuckets < NumBuckets)
							NumBuckets = NumBuckets;
						memcpy(newBuckets, Buckets, sizeof(T*) * std::min(newNumBuckets, (uint32_t)NumBuckets));
						GameFree(Buckets);
					}
					Buckets = newBuckets;
				}

				for (auto i = NumBuckets; i < newNumBuckets; i++) {
					Buckets[i] = GameAllocArray<T>(bucketSize);
				}

				NumBuckets = newNumBuckets;
			}
		} else {
			if (NumBuckets) {
				for (auto i = 0; i < NumBuckets; i++) {
					GameFree(Buckets[i]);
				}

				if (Buckets != nullptr) {
					GameFree(Buckets);
				}
			}

			NumBuckets = 0;
		}
	}

	inline T const& operator [] (uint32_t index) const
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}

	inline T& operator [] (uint32_t index)
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}
};

template <class T>
struct BucketedStaticArray
{
	FrameAllocator* Store;
	T** Buckets;
	uint16_t BitsPerBucket;
	uint16_t NumBuckets;
	uint32_t Used;

	inline T const& operator [] (uint32_t index) const
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}

	inline T& operator [] (uint32_t index)
	{
		assert(index < ((uint32_t)NumBuckets << BitsPerBucket));
		return Buckets[index >> BitsPerBucket][index & ((1 << BitsPerBucket) - 1)];
	}
};

template <class TKey>
struct BucketedHashSet
{
	FrameAllocator* Store;
	int32_t** HashKeys;
	int32_t** NextIds;
	TKey** Keys;
	uint16_t BitsPerHashBucket;
	uint16_t NumHashBuckets;
	uint16_t BitsPerKeyBucket;
	uint16_t NumKeyBuckets;
	uint32_t HashTableSize;
	uint32_t KeysSize;

	TKey const& KeyAt(uint32_t index) const
	{
		auto keyTable = index >> BitsPerKeyBucket;
		auto keySlot = index & ((1 << BitsPerKeyBucket) - 1);
		return Keys[keyTable][keySlot];
	}

	int FindIndex(TKey const& key) const
	{
		if (HashTableSize == 0) return -1;

		auto hash = MultiHashMapHash(key) % HashTableSize;
		auto keyIndex = HashKeys[hash >> BitsPerHashBucket][hash & ((1 << BitsPerHashBucket) - 1)];
		while (keyIndex >= 0) {
			auto keyTable = keyIndex >> BitsPerKeyBucket;
			auto keySlot = keyIndex & ((1 << BitsPerKeyBucket) - 1);
			if (Keys[keyTable][keySlot] == key) return keyIndex;
			keyIndex = NextIds[keyTable][keySlot];
		}

		return -1;
	}
};

struct BucketedBitSet
{
	uint64_t** BitSet;
	uint16_t BitsPerBucket;
	uint16_t NumBuckets;
	uint32_t NumQwords;

	inline uint32_t Size() const
	{
		return NumQwords << 6;
	}

	inline bool Get(uint32_t index) const
	{
		if (index >= Size()) {
			return false;
		} else {
			auto qword = index >> 6;
			return _bittest64((int64_t*)&BitSet[qword >> BitsPerBucket][qword & ((1 << BitsPerBucket) - 1)], index & 0x3f);
		}
	}

	inline bool operator [] (uint32_t index) const
	{
		return Get(index);
	}

	inline void Set(uint32_t index)
	{
		assert(index < Size());
		auto qword = index >> 6;
		BitSet[qword >> BitsPerBucket][qword & ((1 << BitsPerBucket) - 1)] |= (1ull << (index & 0x3f));
	}

	inline void Clear(uint32_t index)
	{
		assert(index < Size());
		auto qword = index >> 6;
		BitSet[qword >> BitsPerBucket][qword & ((1 << BitsPerBucket) - 1)] &= ~(1ull << (index & 0x3f));
	}
};

template <class TKey, class TValue>
struct BucketedSparseHashSet
{
	FrameAllocator* Store;
	BucketedBitSet Bitmap;

	BucketedRawStaticArray<uint16_t> LookupTable;
	BucketedStaticArray<TKey> Keys;
	BucketedStaticArray<TValue> Values;

	int FindIndex(TKey const& key) const
	{
		auto hash = (uint32_t)SparseHashMapHash(key);
		if (!Bitmap[hash]) return -1;

		auto keyIndex = LookupTable[hash];
		if (keyIndex == 0xFFFF || Keys[keyIndex] != key) return -1;
		return keyIndex;
	}

	TValue const* Find(TKey const& key) const
	{
		auto index = FindIndex(key);
		if (index < 0) return nullptr;

		return &Values[index];
	}
};

template <class TKey, class TValue>
struct BucketedHashMap : public BucketedHashSet<TKey>
{
	BucketedStaticArray<TValue> Values;

	TValue* Find(TKey const& key)
	{
		auto index = this->FindIndex(key);
		if (index < 0) return nullptr;

		return &Values[index];
	}

	TValue const* Find(TKey const& key) const
	{
		auto index = this->FindIndex(key);
		if (index < 0) return nullptr;

		return &Values[index];
	}
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

struct ECBEntityChangeSet
{
	struct ComponentRef
	{
		// -1 = removed
		int32_t PoolIndex;
		uint16_t field_4;
		uint16_t ComponentTypeId;
	};

	BucketedStaticArray<ComponentRef> Store;
	uint64_t X;
	uint64_t Y;
	EntityChangeFlags Flags;
	__int16 field_2A;
};

struct ComponentFrameStorage
{
	BucketedStaticArray<void*> Components;
	uint32_t NumComponents;
	uint16_t ComponentSizeInBytes;
	uint16_t ComponentTypeId;
	void* DestructorProc;
};

struct ImmediateWorldCache : public ProtectedGameObject<ImmediateWorldCache>
{
	struct ComponentRef
	{
		int32_t PoolIndex;
		uint16_t field_4;
		uint16_t ComponentTypeId;
	};

	struct EntityComponentMap
	{
		BucketedStaticArray<ComponentRef> Store;
		uint64_t X;
		uint64_t Y;
		uint16_t Flags;
		__int16 field_2A;
	};

	struct ComponentPtr
	{
		void* Ptr;
		uint64_t Unknown;
	};

	struct ComponentChanges
	{
		BucketedHashMap<EntityHandle, ComponentPtr> Components;
		ComponentFrameStorage FrameStorage;
		void* field_70;
		void* field_78;
	};

	struct Changes
	{
		ComponentTypeMask AvailableComponentTypes;
		ComponentChanges* ComponentsByType;
		uint64_t Unknown;
	};

	Changes WriteChanges;
	Changes ReadChanges;
	Array<ComponentCallbacks*>* Callbacks;
	FrameAllocator* Allocator;
	EntityWorld* EntityWorld;
	EntityHandleGenerator* HandleGenerator;
	__int64 field_158;
};

struct EntityCommandBuffer : public ProtectedGameObject<EntityCommandBuffer>
{
	EntityHandleGenerator* HandleGenerator;
	FrameAllocator* Allocator;
	BucketedHashMap<EntityHandle, ECBEntityChangeSet> EntityChanges;
	BucketedSparseHashSet<ComponentTypeIndex, ComponentFrameStorage> ComponentPools;
	bool field_A0;
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

struct EntityWorld : public ProtectedGameObject<EntityWorld>
{
	using UpdateProc = void (EntityWorld* self, GameTime const& time);

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
	Array<EntityCommandBuffer*> CommandBuffers;
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
	Array<ComponentOps*> ComponentOps;
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
