#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/CommonTypes.h>

BEGIN_SE()

struct BaseComponent
{
	// FIXME - not sure if this even exists anymore
	EntityHandle Entity;
};

END_SE()

BEGIN_NS(ecs)

struct EntityWorld;
struct Entity;

struct EntityRef
{
	EntityHandle Handle;
	EntityWorld* World{ nullptr };
};

using ComponentTypeMask = std::array<uint64_t, 30>;
using UnknownMask = std::array<uint64_t, 38>;
using EntityTypeMask = std::array<uint64_t, 4>;


// Handle type index, registered statically during game startup
// FIXME - delete all ComponentHandle logic!
enum class HandleTypeIndexTag {};
using HandleTypeIndex = TypedIntegral<uint16_t, HandleTypeIndexTag>;
// Component type index, registered statically during game startup
enum class ComponentTypeIndexTag {};
using ComponentTypeIndex = TypedIntegral<uint16_t, ComponentTypeIndexTag>;

static constexpr ComponentTypeIndex UndefinedComponent{ 0xffff };
static constexpr HandleTypeIndex UndefinedHandle{ 0xffff };

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

struct ComponentType
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

struct Query
{
	struct EntityMatch
	{
		EntityTypeMask ComponentTypeIdMask;
		void* field_20;
		uint8_t field_28;
		uint8_t field_29;
		uint8_t field_2A;
		uint8_t field_2B;
		uint8_t field_2C;
		uint8_t field_2D;
		uint8_t field_2E;
		uint8_t field_2F;
		Entity* Entity;
		void* field_38;
	};


	ComponentTypeMask ComponentTypeIdMask;
	ComponentTypeMask ComponentTypeIdMask2;
	ComponentTypeMask ComponentTypeIdMask3;
	UnknownMask field_2D0;
	UnknownMask field_400;
	UnknownMask field_530;
	int field_620;
	Array<EntityMatch*> Entities;
	Array<void*> field_638;
	uint16_t* ComponentTypeIds;
	uint8_t FirstComponentTypeIndex;
	uint8_t LastComponentTypeIndex;
	char field_652;
	char field_653;
	uint8_t ComponentTypesCapacity;
	void* field_658;
};


struct QueryManager
{
	Array<Query> Queries;
	Array<int16_t> field_10;
	Array<int16_t> field_20;
	Array<int16_t> field_30;
	Array<int16_t> field_40;
	Array<int16_t> field_50;
	Array<int16_t> field_60;
};

struct ComponentRegistry
{
	BitSet<> Bitmask;
	Array<ComponentType> Types;
};

struct SystemType
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

struct SystemRegistry
{
	void* VMT;
	Array<SystemType> Systems;
	uint32_t Unknown;
	uint32_t GrowSize;
};

struct ComponentReplication
{
	Array<MultiHashMap<EntityHandle, BitSet<>>> ComponentPools;
	bool Dirty;
};

struct EntityTypeSalts
{
	struct Entry
	{
		int Index;
		int Salt;
	};

	Entry** Buckets;
	uint32_t NumElements;
	uint16_t NumBuckets;
	uint16_t BitsPerBucket;
	__int64 field_10;
	int field_18;
	__int64 field_20;
	__int64 field_28;
	__int64 field_30;
	__int64 field_38;
};
		
struct Entity
{
	struct ComponentEntry
	{
		uint16_t Index;
		ComponentTypeIndex ComponentTypeId;
		char field_4;
		char field_5;
		void* DtorProc;
	};
			
	struct ComponentBucket
	{
		__int16 A;
		__int16 B;
	};
			
	struct ComponentPoolEntry
	{
		void** Components;
		void* B;
	};
			
	struct EntityComponentPool
	{
		ComponentPoolEntry Pool[256];
	};


	ComponentTypeMask field_0;
	uint64_t SomeEntityMask;
	uint16_t* ComponentIndexList; // List of indices into EntityComponentPool
	ComponentEntry* ComponentDtors;
	ComponentTypeIndex TypeId;
	__int16 field_10A;
	uint16_t ComponentIndexListSize;
	char field_10E;
	char field_10F;
	char field_110;
	Array<EntityComponentPool*> Components;
	Array<void*> field_128;
	BitSet<> field_138;
	int field_148;
	Array<void*> field_150;
	MultiHashMap<uint16_t, uint8_t> ComponentTypeToIndex;
	MultiHashMap<uint64_t, ComponentBucket> ComponentBuckets;
	MultiHashMap<uint64_t, uint64_t> field_1E0;
	MultiHashMap<uint64_t, uint64_t> field_220;
	Array<void*> field_260;
	Array<void*> field_270;
	MultiHashMap<uint16_t, MultiHashMap<uint16_t, void*>*> Components_u16_Unk;
	char field_2C0;
	__int64 field_2C8;
	EntityTypeMask ComponentMask; // Valid indices into Components pool
	Array<int16_t> field_2F0;
	Array<int16_t> field_300;
	UnknownMask field_310;
	Array<void*> field_440;
	UnknownMask field_450;

	void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type) const;
};


struct EntityStore
{
	struct TypeSalts
	{
		struct Entry
		{
			int Salt;
			uint16_t EntityIndex;
		};

		Entry** Buckets;
		uint32_t NumElements;
		uint16_t NumBuckets;
		uint16_t BitsPerBucket;
	};

	struct SaltMap
	{
		std::array<TypeSalts, 0x40> Buckets;
		uint32_t Size;
	};

	Array<Entity*> Entities;
	MultiHashMap<uint64_t, uint16_t> TypeHashToEntityIndex;
	SaltMap Salts;
	MultiHashMap<uint64_t, uint64_t> field_458;
	BitSet<> UsedTypeIndices;
	ComponentRegistry* ComponentRegistry;
	QueryManager* Queries;

	Entity* GetEntity(EntityHandle entityHandle) const;
};

struct ComponentDataStore
{
	__int64 FastLock;
	__int64 FastLock2;
	CRITICAL_SECTION field_10;
	Array<void*> Pages[2];
};

struct ComponentOps
{
	__int64 VMT;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	__int64 field_20;
	__int16 TypeId;
};

struct ComponentPool
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

struct ComponentRegistryEntry1
{
	void* VMT;
	__int64 field_8;
	Array<void*> field_10;
	__int64 field_20;
	Array<void*> field_28;
};

struct EntityComponents
{
	struct SparseHashMap
	{
		BitSet<> SetValues;
		Array<int16_t> NextIds;
		Array<int16_t> Keys;
		Array<ComponentPool*> Values;
	};

	struct EntityEntry
	{
		ComponentTypeMask ComponentTypeIdMask;
		ComponentTypeMask ComponentUpdateFlags1;
		ComponentTypeMask ComponentUpdateFlags2;
		uint64_t field_2D0[8];
		__int64 field_310;
		__int64 field_318;
		Array<uint64_t> ComponentHandles;
		Array<uint64_t> field_330;
		int Flags;
		int field_344;
		__int64 field_348;
	};

	MultiHashMap<EntityHandle, EntityEntry*> Entities;
	SparseHashMap ComponentPools;
	char field_80;
	MultiHashMap<uint16_t, MultiHashMap<uint64_t, void*>> ComponentsByType;
	MultiHashMap<uint16_t, MultiHashMap<uint64_t, void*>> ComponentsByType2;
	void* field_108;
	void* field_110;
	Array<void*>* ComponentTypes;
	ComponentDataStore* Store;
	EntityWorld* EntityWorld;
	EntityTypeSalts* Salts;
	__int64 field_128;
};

struct EntityWorld : public ProtectedGameObject<EntityWorld>
{
	ComponentReplication* Replication;
	ComponentRegistry ComponentRegistry_;
	SystemRegistry Systems;
	__int64 field_48;
	UpdateInfo UpdateInfos;
	__int64 field_128;
	__int64 field_130;
	__int64 field_138;
	uint64_t GameTime[3];
	void*  ECSUpdateBatch;
	int field_160;
	__int64 field_168;
	Array<void*> field_170;
	Array<ComponentRegistryEntry1*> ComponentTypes;
	bool field_190;
	bool NeedsUpdate;
	bool field_192;
	bool field_193;
	QueryManager Queries;
	std::array<EntityTypeSalts, 0x40>* EntitySalts;
	EntityStore* Entities;
	ComponentDataStore ComponentData;
	__int64 field_270;
	MultiHashMap<uint64_t, uint64_t> field_278;
	Array<void*> field_2B8;
	Array<ComponentOps*> ComponentOpsList;
	ScratchString Scratch;
	void* UpdateBatches;
	CriticalSection CS;
	int field_338;
	EntityComponents* Components;
	__int64 field_348;
	__int64 field_350;
	__int64 field_358;
	__int64 field_360;
	__int64 field_368;
	__int64 field_370;
	__int64 field_378;

	void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type);
	void* GetComponent(char const* nameGuid, ComponentTypeIndex type);
	void* GetComponent(FixedString const& guid, ComponentTypeIndex type);

	Entity* GetEntity(EntityHandle entityHandle) const;
	bool IsValid(EntityHandle entityHandle) const;
};

END_NS()
