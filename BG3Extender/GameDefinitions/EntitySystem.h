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
struct EntityClass;

using ComponentTypeMask = std::array<uint64_t, 32>;
using UnknownMask = std::array<uint64_t, 40>;
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

END_SE()

BEGIN_NS(ecs)

struct UpdateInfo
{
	__int64 field_50;
	__int64 field_58;
	__int64 field_60;
	__int64 field_68;
	__int64 field_70;
	__int64 field_78;
	__int64 field_80;
	__int64 field_88;
	__int64 field_90;
	__int64 field_98;
	__int64 field_A0;
	__int64 field_A8;
	__int64 field_B0;
	__int64 field_B8;
	__int64 field_C0;
	__int64 field_C8;
	__int64 field_D0;
	__int64 field_D8;
	__int64 field_E0;
	__int64 field_E8;
	__int64 field_F0;
	__int64 field_F8;
	__int64 field_100;
	__int64 field_108;
	__int64 field_110;
	__int64 field_118;
	__int64 field_120;
};

struct ComponentType : public ProtectedGameObject<ComponentType>
{
	__int16 field_0;
	int field_4;
	BYTE field_8;
	char field_9;
	char field_A;
	bool QueryFlags[4];
	uint16_t field_10;
	__int16 field_12;
	void* DtorProc;
	Array<int16_t> DependentComponentIndices;
	Array<int16_t> DependencyComponentIndices;
};

struct Query : public ProtectedGameObject<Query>
{
	struct EntityClassMatch : public ProtectedGameObject<EntityClassMatch>
	{
		EntityTypeMask ComponentTypeIdMask;
		union {
			uint8_t* ComponentIndices;
			std::array<uint8_t, 8> InlineComponentIndices;
		};
		uint8_t LastIndex1;
		uint8_t LastIndex2;
		uint8_t LastIndex3;
		uint8_t LastIndex4;
		uint8_t LastIndex5;
		uint8_t NumIndices;
		EntityClass* EntityClass;
		void* field_38;

		uint8_t GetComponentIndex(uint8_t idx) const
		{
			assert(idx < NumIndices);
			if (NumIndices > 8) {
				return ComponentIndices[idx];
			} else {
				return InlineComponentIndices[idx];
			}
		}
	};

	// TypeList template parameters to ecs::query::spec::Spec<...>
	StaticArray<int32_t> A1;
	StaticArray<int32_t> A2;
	StaticArray<uint64_t> A3;
	StaticArray<EntityClassMatch> EntityClasses;
	uint64_t* A5;
	uint32_t A6;
	uint8_t A7;
	uint8_t A8;
	uint8_t A9;
	uint8_t A10;
	uint8_t FirstComponentTypeIndex;
	uint8_t LastComponentTypeIndex;
	uint8_t field_652;
	uint8_t NextComponentTypeIndex;
	uint8_t ComponentTypesCapacity;
	uint8_t ComponentTypesCapacityUnk;

	void* GetFirstMatchingComponent(std::size_t componentSize, bool isProxy);
	Array<void*> GetAllMatchingComponents(std::size_t componentSize, bool isProxy);
};


struct QueryManager : public ProtectedGameObject<QueryManager>
{
	Array<Query> Queries;
	Array<int16_t> ComponentTypes1;
	Array<int16_t> ComponentTypes2;
	Array<int16_t> ComponentTypes3;
	Array<int16_t> ComponentTypes4;
	Array<int16_t> ComponentTypes5;
	Array<int16_t> ComponentTypes6;
};

struct ComponentRegistry : public ProtectedGameObject<ComponentRegistry>
{
	BitSet<> Bitmask;
	Array<ComponentType> Types;
};

struct SystemType : public ProtectedGameObject<SystemType>
{
	using ID = uint32_t;

	void* System;
	int32_t SystemIndex0;
	int32_t SystemIndex1;
	__int16 field_10;
	char field_12;
	void* SomeProc1;
	__int64 field_20;
	void* SomeProc2;
	MultiHashSet<SystemType::ID> DependencySystems;
	MultiHashSet<SystemType::ID> DependentSystems;
	MultiHashSet<uint32_t> HandleMappings2;
	MultiHashSet<uint32_t> HandleMappings;
};

struct SystemRegistry : public ProtectedGameObject<SystemRegistry>
{
	void* VMT;
	Array<SystemType> Systems;
	uint32_t Unknown;
	uint32_t GrowSize;
};

struct ComponentReplication : public ProtectedGameObject<ComponentReplication>
{
	Array<MultiHashMap<EntityHandle, BitSet<>>> ComponentPools;
	bool Dirty;
};

struct EntityTypeSalts : public ProtectedGameObject<EntityTypeSalts>
{
	struct Entry
	{
		int Index;
		int Salt;
	};

	Entry** Buckets;
	uint16_t BitsPerBucket;
	uint16_t NumBuckets;
	uint32_t NumElements;
	uint32_t field_10;
	uint32_t field_14;
	uint32_t field_18;
	uint64_t field_20[4];
};
		
struct EntityClass : public ProtectedGameObject<EntityClass>
{
	static constexpr std::size_t PageSize = 64;

	struct ComponentEntry
	{
		uint16_t Index;
		ComponentTypeIndex ComponentTypeId;
		char field_4;
		char field_5;
		void* DtorProc;
	};
			
	struct InstanceComponentPointer
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


	ComponentTypeMask ComponentsInClass;
	uint64_t SomeEntityMask;
	ComponentTypeIndex* ComponentIndices; // List of indices into ComponentTypeToIndex/ComponentPoolsByType
	ComponentEntry* ComponentDtors;
	uint16_t EntityClassId;
	__int16 field_10A;
	uint16_t ComponentIndexListSize;
	char field_10E;
	char field_10F;
	char field_110;
	Array<ComponentPage*> ComponentPools;
	Array<HandlePage*> InstanceHandles;
	BitSet<> field_138;
	int field_148;
	Array<void*> field_150;
	MultiHashMap<ComponentTypeIndex, uint8_t> ComponentTypeToIndex;
	MultiHashMap<EntityHandle, InstanceComponentPointer> InstanceToPageMap;
	MultiHashMap<EntityHandle, uint64_t> field_1E0;
	MultiHashMap<EntityHandle, uint64_t> field_220;
	Array<void*> field_260;
	Array<void*> field_270;
	MultiHashMap<ComponentTypeIndex, MultiHashMap<uint16_t, ComponentPage*>*> ComponentPoolsByType;
	char field_2C0;
	__int64 field_2C8;
	EntityTypeMask ComponentMask; // Valid indices into Components pool
	Array<int16_t> field_2F0;
	Array<int16_t> field_300;
	UnknownMask field_310;
	Array<void*> field_440;
	UnknownMask field_450;

	void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
	void* GetComponent(InstanceComponentPointer const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const;
	void* GetComponent(InstanceComponentPointer const& entityPtr, uint8_t componentSlot, std::size_t componentSize, bool isProxy) const;

	inline bool HasComponent(ComponentTypeIndex type) const
	{
		return (ComponentsInClass[(uint16_t)type >> 6] & (1ull << ((uint16_t)type & 0x3f))) != 0;
	}
};


struct EntityStore : public ProtectedGameObject<EntityStore>
{
	struct TypeSalts : public ProtectedGameObject<TypeSalts>
	{
		struct Entry
		{
			int Salt;
			uint16_t EntityClassIndex;
		};

		Entry** Buckets;
		uint16_t BitsPerBucket;
		uint16_t NumBuckets;
		uint32_t NumElements;
	};

	struct SaltMap : public ProtectedGameObject<SaltMap>
	{
		std::array<TypeSalts, 0x40> Buckets;
		uint32_t Size;
	};

	Array<EntityClass*> EntityClasses;
	MultiHashMap<uint64_t, uint16_t> TypeHashToClassIndex;
	SaltMap Salts;
	MultiHashMap<uint64_t, uint64_t> field_458;
	BitSet<> UsedTypeIndices;
	ComponentRegistry* ComponentRegistry;
	QueryManager* Queries;

	EntityClass* GetEntityClass(EntityHandle entityHandle) const;
};

struct ComponentDataStore : public ProtectedGameObject<ComponentDataStore>
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
	ComponentDataStore* Store;
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
	ComponentDataStore* Store;
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
	ComponentDataStore* Store;
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
	ComponentDataStore* Store;
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

struct EntityComponents : public ProtectedGameObject<EntityComponents>
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

	struct ComponentPoolInfo
	{
		BucketedStaticArray<void*> Components;
		uint32_t NumComponents;
		uint16_t ComponentSizeInBytes;
		uint16_t ComponentTypeId;
		void* DestructorProc;
	};

	struct ComponentMap
	{
		BucketedHashMap<EntityHandle, void*> Components;
		ComponentPoolInfo Pool;
		void* field_70;
		void* field_78;
	};

	struct TransientComponents
	{
		BitArray<uint64_t, 32> AvailableComponentTypes;
		ComponentMap* ComponentsByType;
		uint64_t Unknown;
	};

	TransientComponents Components;
	TransientComponents Components2;
	Array<ComponentCallbacks*>* Callbacks;
	ComponentDataStore* Store;
	EntityWorld* EntityWorld;
	EntityTypeSalts* Salts;
	__int64 field_158;
};

struct NewEntityPool : public ProtectedGameObject<NewEntityPool>
{
	BucketedRawStaticArray<uint64_t> FreePool;
	uint32_t NextFreeSlotIndex;
	uint32_t HighestIndex;
	uint32_t NumFreeSlots;
	__int64 field_20;
	__int64 field_28;
	__int64 field_30;
	__int64 field_38;

	uint64_t Add();
	void Grow();
};

struct NewEntityPools : public ProtectedGameObject<NewEntityPools>
{
	std::array<NewEntityPool, 0x40> Pools;

	EntityHandle Add(uint32_t classIndex);
};

struct ComponentUpdates : public ProtectedGameObject<ComponentUpdates>
{
	NewEntityPools* NewEntities;
	ComponentDataStore* Store;
	BucketedHashMap<EntityHandle, EntityComponents::EntityComponentMap> Entities;
	BucketedSparseHashSet<ComponentTypeIndex, EntityComponents::ComponentPoolInfo> ComponentPools;
	bool field_A0;
	uint64_t field_B8;
};

struct EntityWorld : public ProtectedGameObject<EntityWorld>
{
	using UpdateProc = void (EntityWorld* self, GameTime const& time);

	ComponentReplication* Replication;
	ComponentRegistry ComponentRegistry_;
	SystemRegistry Systems;
	__int64 field_48;
	UpdateInfo UpdateInfos;
	__int64 field_128;
	__int64 field_130;
	__int64 field_138;
	GameTime Time;
	void* ECSUpdateBatch;
	int field_160;
	Array<ComponentUpdates*> ComponentUpdates;
	Array<ComponentCallbacks*> ComponentCallbacks;
	bool field_190;
	bool NeedsUpdate;
	bool field_192;
	bool field_193;
	QueryManager Queries;
	std::array<EntityTypeSalts, 0x40>* EntitySalts;
	EntityStore* EntityTypes;
	__int64 field_218[6];
	ComponentDataStore ComponentData;
	__int64 field_298;
	__int64 field_2A0;
	__int64 field_2A8;
	__int64 field_2B0;
	__int64 field_2B8;
	Array<void*> field_2C0;
	Array<ComponentOps*> ComponentOpsList;
	Array<void*> field_2E0;
	void* field_2F0;
	bool field_2F8;
	bool field_2F9;
	void* UpdateBatches;
	CriticalSection CS;
	EntityComponents* Components;
	uint64_t InitFlags;
	uint32_t InitFlags2;

	void* GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy);

	EntityClass* GetEntityClass(EntityHandle entityHandle) const;
	bool IsValid(EntityHandle entityHandle) const;
};

END_NS()
