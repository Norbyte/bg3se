#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>
#include <GameDefinitions/Components/All.h>
#include <Extender/ScriptExtender.h>

#undef DEBUG_INDEX_MAPPINGS

#if defined(DEBUG_INDEX_MAPPINGS)
#define DEBUG_IDX(x) std::cout << x << std::endl
#else
#define DEBUG_IDX(x)
#endif

BEGIN_NS(ecs)

void* Query::GetFirstMatchingComponent(std::size_t componentSize, bool isProxy)
{
	for (auto const& cls : EntityClasses) {
		if (cls.EntityClass->InstanceToPageMap.size() > 0) {
			auto instPage = cls.EntityClass->InstanceToPageMap.Values[0];
			auto componentIdx = cls.GetComponentIndex(0);
			assert(cls.EntityClass->ComponentPools.size() >= 1);
			return cls.EntityClass->GetComponent(instPage, componentIdx, componentSize, isProxy);
		}
	}

	return {};
}

Array<void*> Query::GetAllMatchingComponents(std::size_t componentSize, bool isProxy)
{
	Array<void*> hits;

	for (auto const& cls : EntityClasses) {
		auto componentIdx = cls.GetComponentIndex(0);
		for (auto const& instance : cls.EntityClass->InstanceToPageMap) {
			auto component = cls.EntityClass->GetComponent(instance.Value(), componentIdx, componentSize, isProxy);
			hits.push_back(component);
		}
	}

	return hits;
}

uint64_t NewEntityPool::Add()
{
	if (NumFreeSlots < (1u << FreePool.BitsPerBucket) / 2) {
		Grow();
	}

	NumFreeSlots--;
	auto index = NextFreeSlotIndex & ((1 << FreePool.BitsPerBucket) - 1);
	auto& bucket = FreePool.Buckets[NextFreeSlotIndex >> FreePool.BitsPerBucket];
	auto prevFreeSlot = NextFreeSlotIndex;
	NextFreeSlotIndex = (uint32_t)(bucket[index] & 0xffffffffull);
	bucket[index] = (bucket[index] & 0xffffffff00000000ull) | prevFreeSlot;
	return bucket[index];
}

void NewEntityPool::Grow()
{
	auto newSize = FreePool.Used + (1 << FreePool.BitsPerBucket);
	if (((uint32_t)FreePool.NumBuckets << FreePool.BitsPerBucket) < newSize) {
		FreePool.Resize(newSize);
	}

	for (auto i = 0; i < (1 << FreePool.BitsPerBucket); i++) {
		auto entityIndex = FreePool.Used;
		FreePool[FreePool.Used++] = entityIndex | 0x100000000ull;

		if (NumFreeSlots > 0) {
			auto highestIndex = HighestIndex;
			FreePool[HighestIndex] = (FreePool[HighestIndex] & 0xffffffff00000000ull) | entityIndex;
		}

		FreePool[entityIndex] = entityIndex | 0x100000000ull;
		NumFreeSlots = this->NumFreeSlots;
		HighestIndex = entityIndex;
		if (NumFreeSlots == 0) {
			NextFreeSlotIndex = entityIndex;
		}

		NumFreeSlots++;
	}
}

EntityHandle NewEntityPools::Add(uint32_t classIndex)
{
	auto index = Pools[classIndex].Add();
	return EntityHandle(classIndex, index);
}

void* EntityClass::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
	auto ref = InstanceToPageMap.Find(entityHandle);
	if (ref) {
		return GetComponent(**ref, type, componentSize, isProxy);
	} else {
		return nullptr;
	}
}

void* EntityClass::GetComponent(InstanceComponentPointer const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
	auto compIndex = ComponentTypeToIndex.Find((uint16_t)type.Value());
	if (compIndex) {
		return GetComponent(entityPtr, **compIndex, componentSize, isProxy);
	} else {
		return nullptr;
	}
}

void* EntityClass::GetComponent(InstanceComponentPointer const& entityPtr, uint8_t componentSlot, std::size_t componentSize, bool isProxy) const
{
	auto& page = ComponentPools[entityPtr.PageIndex][componentSlot];
	auto buf = (uint8_t*)page.ComponentBuffer;
	assert(buf != nullptr);
	if (isProxy) {
		auto ptr = buf + sizeof(void*) * entityPtr.EntryIndex;
		return *(uint8_t**)ptr;
	} else {
		return buf + componentSize * entityPtr.EntryIndex;
	}
}
	
void* EntityWorld::GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy)
{
	auto entityClass = GetEntityClass(entityHandle);
	if (entityClass != nullptr) {
		auto component = entityClass->GetComponent(entityHandle, type, componentSize, isProxy);
		if (component != nullptr) {
			return component;
		}
	}

	auto compPool = Components->ComponentsByType.Find((uint16_t)type);
	if (compPool) {
		auto transientRef = compPool->Find(entityHandle);
		if (transientRef) {
			return *transientRef;
		}
	}

	auto compPool2 = Components->ComponentsByType2.Find((uint16_t)type);
	if (compPool2) {
		auto transientRef = compPool2->Find(entityHandle);
		if (transientRef) {
			return *transientRef;
		}
	}

	return nullptr;
}

bool EntityWorld::IsValid(EntityHandle entityHandle) const
{
	if (entityHandle.GetType() < 0x40) {
		auto& salts = (*EntitySalts)[entityHandle.GetType()];
		if (entityHandle.GetIndex() < salts.NumElements) {
			auto salt = salts.Buckets[entityHandle.GetIndex() >> salts.BitsPerBucket][entityHandle.GetIndex() & ((1 << salts.BitsPerBucket) - 1)];
			return salt.Salt == entityHandle.GetSalt() && salt.Index == entityHandle.GetIndex();
		}
	}

	return false;
}

EntityClass* EntityStore::GetEntityClass(EntityHandle entityHandle) const
{
	auto& componentSalts = Salts.Buckets[entityHandle.GetType()];
	if (entityHandle.GetIndex() < componentSalts.NumElements) {
		auto salt = componentSalts.Buckets[entityHandle.GetIndex() >> componentSalts.BitsPerBucket][entityHandle.GetIndex() & ((1 << componentSalts.BitsPerBucket) - 1)];
		if (salt.Salt == entityHandle.GetSalt()) {
			return EntityClasses[salt.EntityClassIndex];
		}
	}

	return nullptr;
}

EntityClass* EntityWorld::GetEntityClass(EntityHandle entityHandle) const
{
	if (!IsValid(entityHandle)) {
		return nullptr;
	}

	return EntityTypes->GetEntityClass(entityHandle);
}

#if defined(NDEBUG)
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::Once };
#else
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::FullECS };
#endif

EntitySystemHelpersBase::EntitySystemHelpersBase()
	: queryIndices_{ UndefinedIndex },
	staticDataIndices_{ UndefinedIndex }
{}

STDString SimplifyComponentName(StringView name)
{
	STDString key{ name };
	if (key.length() > 52 && strncmp(key.c_str(), "class ls::_StringView<char> __cdecl ls::GetTypeName<", 52) == 0) {
		key = key.substr(52);
	}

	if (key.length() > 6 && strncmp(key.c_str(), "class ", 6) == 0) {
		key = key.substr(6);
	}
	else if (key.length() > 7 && strncmp(key.c_str(), "struct ", 7) == 0) {
		key = key.substr(7);
	}

	if (key.length() > 7 && strncmp(key.c_str() + key.size() - 7, ">(void)", 7) == 0) {
		key = key.substr(0, key.size() - 7);
	}

	return key;
}

BitSet<>* EntitySystemHelpersBase::GetReplicationFlags(EntityHandle const& entity, ExtComponentType type)
{
	if (components_[(unsigned)type].ReplicationIndex == UndefinedReplicationComponent) {
		return nullptr;
	}

	return GetReplicationFlags(entity, components_[(unsigned)type].ReplicationIndex);
}

BitSet<>* EntitySystemHelpersBase::GetReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType)
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return nullptr;

	auto& pools = world->Replication->ComponentPools;
	auto typeId = (uint16_t)replicationType;
	if (typeId >= (int)pools.Size()) {
		OsiError("Attempted to fetch replication list for component " << typeId << ", but replication pool size is " << pools.Size() << "!");
		return nullptr;
	}

	auto& pool = pools[typeId];
	auto syncFlags = pool.Find(entity);
	if (syncFlags) {
		return *syncFlags;
	} else {
		return nullptr;
	}
}

BitSet<>* EntitySystemHelpersBase::GetOrCreateReplicationFlags(EntityHandle const& entity, ExtComponentType type)
{
	if (components_[(unsigned)type].ReplicationIndex == UndefinedReplicationComponent) {
		return nullptr;
	}

	return GetOrCreateReplicationFlags(entity, components_[(unsigned)type].ReplicationIndex);
}

BitSet<>* EntitySystemHelpersBase::GetOrCreateReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType)
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return nullptr;

	auto& pools = world->Replication->ComponentPools;
	auto typeId = (uint16_t)replicationType;
	if (typeId >= (int)pools.Size()) {
		OsiError("Attempted to fetch replication list for component " << typeId << ", but replication pool size is " << pools.Size() << "!");
		return nullptr;
	}

	auto& pool = pools[typeId];
	auto syncFlags = pool.Find(entity);
	if (syncFlags) {
		return *syncFlags;
	}

	return pool.Set(entity, BitSet<>());
}

void EntitySystemHelpersBase::NotifyReplicationFlagsDirtied()
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return;

	world->Replication->Dirty = true;
}

void EntitySystemHelpersBase::BindSystem(StringView name, int32_t systemId)
{
	auto it = systemIndexMappings_.insert(std::make_pair(name, systemId));
	if (systemTypeIdToName_.size() <= systemId) {
		systemTypeIdToName_.resize(systemId + 1);
	}

	systemTypeIdToName_[systemId] = &it.first->first;
}

void EntitySystemHelpersBase::BindQuery(StringView name, int32_t queryId)
{
	auto it = queryMappings_.insert(std::make_pair(name, queryId));
	if (queryTypeIdToName_.size() <= queryId) {
		queryTypeIdToName_.resize(queryId + 1);
	}

	queryTypeIdToName_[queryId] = &it.first->first;
}

void EntitySystemHelpersBase::BindStaticData(StringView name, int32_t id)
{
	auto it = staticDataMappings_.insert(std::make_pair(name, id));
	if (staticDataIdToName_.size() <= id) {
		staticDataIdToName_.resize(id + 1);
	}

	staticDataIdToName_[id] = &it.first->first;
}

void EntitySystemHelpersBase::BindComponent(StringView name, int32_t id)
{
	STDString const* pName;
	auto it = componentNameToIndexMappings_.find(STDString(name));
	if (it == componentNameToIndexMappings_.end()) {
		auto iit = componentNameToIndexMappings_.insert(std::make_pair(name, IndexMappings{(ComponentTypeIndex)id, UndefinedReplicationComponent}));
		pName = &iit.first->first;
	} else {
		it->second.ComponentIndex = (ComponentTypeIndex)id;
		pName = &it->first;
	}

	componentIndexToNameMappings_.insert(std::make_pair(id, pName));
}

void EntitySystemHelpersBase::BindReplication(StringView name, int32_t id)
{
	auto it = componentNameToIndexMappings_.find(STDString(name));
	if (it == componentNameToIndexMappings_.end()) {
		componentNameToIndexMappings_.insert(std::make_pair(name, IndexMappings{UndefinedComponent, (ReplicationTypeIndex)id}));
	} else {
		it->second.ReplicationIndex = (ReplicationTypeIndex)id;
	}
}

void EntitySystemHelpersBase::UpdateComponentMappings()
{
	if (initialized_) return;

	componentNameToIndexMappings_.clear();
	componentIndexToNameMappings_.clear();
	componentIndexToTypeMappings_.clear();
	replicationIndexToTypeMappings_.clear();
	components_.fill(PerComponentData{});
	queryIndices_.fill(UndefinedIndex);
	staticDataIndices_.fill(UndefinedIndex);

	std::unordered_map<int32_t*, TypeIdContext> contexts;
	for (auto const& context : GetStaticSymbols().IndexSymbolToContextMaps) {
		auto name = ecs::SimplifyComponentName(context.second);
		if (name == "ecs::OneFrameComponentTypeIdContext") {
			contexts.insert(std::make_pair(context.first, TypeIdContext::OneFrameComponent));
		} else if (name == "ecs::ComponentTypeIdContext") {
			contexts.insert(std::make_pair(context.first, TypeIdContext::Component));
		} else if (name == "ecs::EntityWorld::SystemsContext") {
			contexts.insert(std::make_pair(context.first, TypeIdContext::System));
		} else if (name == "ecs::sync::ReplicatedTypeContext") {
			contexts.insert(std::make_pair(context.first, TypeIdContext::Replication));
		} else if (name == "ls::ImmutableDataHeadmaster") {
			contexts.insert(std::make_pair(context.first, TypeIdContext::ImmutableData));
		}
	}

	auto const& symbolMaps = GetStaticSymbols().IndexSymbolToNameMaps;
	for (auto const& mapping : symbolMaps) {
		auto name = SimplifyComponentName(mapping.second.name);
		if (name.starts_with("ecs::query::spec::Spec<")) {
			BindQuery(name, *mapping.first);
		}
		else {
			auto contextIt = contexts.find(mapping.second.context);
			if (contextIt != contexts.end()) {
				switch (contextIt->second) {
				case TypeIdContext::System:
					BindSystem(name, *mapping.first);
					break;

				case TypeIdContext::ImmutableData:
					BindStaticData(name, *mapping.first);
					break;

				case TypeIdContext::Component:
					BindComponent(name, *mapping.first);
					break;

				case TypeIdContext::Replication:
					BindReplication(name, *mapping.first);
					break;
				}
			}
		}
	}

#if defined(DEBUG_INDEX_MAPPINGS)
	DEBUG_IDX("COMPONENT MAPPINGS:");

	for (auto const& map : componentNameToIndexMappings_) {
		DEBUG_IDX("\t" << map.first << ": Handle " << map.second.HandleIndex << ", Component " << map.second.ComponentIndex);
	}

	DEBUG_IDX("-------------------------------------------------------");
#endif

	#define T(cls) MapComponentIndices(cls::EngineClass, cls::ComponentType, sizeof(cls), std::is_base_of_v<BaseProxyComponent, cls>);
	#include <GameDefinitions/Components/AllComponentTypes.inl>
	#undef T

	MapQueryIndex("ecs::query::spec::Spec<struct ls::TypeList<struct ls::uuid::ToHandleMappingComponent>,struct ls::TypeList<>,struct ls::TypeList<>,struct ls::TypeList<>,struct ls::TypeList<>,struct ls::TypeList<>,struct ecs::QueryTypePersistentTag,struct ecs::QueryTypeAliveTag>", ExtQueryType::UuidToHandleMapping);

	MapResourceManagerIndex("ls::TagManager", ExtResourceManagerType::Tag);
	MapResourceManagerIndex("eoc::FactionContainer", ExtResourceManagerType::Faction);
	MapResourceManagerIndex("eoc::RaceManager", ExtResourceManagerType::Race);
	MapResourceManagerIndex("eoc::AbilityDistributionPresetManager", ExtResourceManagerType::AbilityDistributionPreset);
	MapResourceManagerIndex("eoc::CompanionPresetManager", ExtResourceManagerType::CompanionPreset);
	MapResourceManagerIndex("eoc::OriginManager", ExtResourceManagerType::Origin);
	MapResourceManagerIndex("eoc::BackgroundManager", ExtResourceManagerType::Background);
	MapResourceManagerIndex("eoc::GodManager", ExtResourceManagerType::God);
	MapResourceManagerIndex("eoc::AbilityListManager", ExtResourceManagerType::AbilityList);
	MapResourceManagerIndex("eoc::SkillListManager", ExtResourceManagerType::SkillList);
	MapResourceManagerIndex("eoc::SpellListManager", ExtResourceManagerType::SpellList);
	MapResourceManagerIndex("eoc::PassiveListManager", ExtResourceManagerType::PassiveList);
	MapResourceManagerIndex("eoc::ProgressionManager", ExtResourceManagerType::Progression);
	MapResourceManagerIndex("eoc::ProgressionDescriptionManager", ExtResourceManagerType::ProgressionDescription);
	MapResourceManagerIndex("eoc::GossipContainer", ExtResourceManagerType::Gossip);
	MapResourceManagerIndex("eoc::ActionResourceTypes", ExtResourceManagerType::ActionResource);
	MapResourceManagerIndex("eoc::ActionResourceGroupManager", ExtResourceManagerType::ActionResourceGroup);
	MapResourceManagerIndex("eoc::EquipmentTypes", ExtResourceManagerType::EquipmentType);
	MapResourceManagerIndex("eoc::VFXContainer", ExtResourceManagerType::VFX);
	MapResourceManagerIndex("eoc::CharacterCreationPresetManager", ExtResourceManagerType::CharacterCreationPreset);
	MapResourceManagerIndex("eoc::CharacterCreationSkinColorManager", ExtResourceManagerType::CharacterCreationSkinColor);
	MapResourceManagerIndex("eoc::CharacterCreationEyeColorManager", ExtResourceManagerType::CharacterCreationEyeColor);
	MapResourceManagerIndex("eoc::CharacterCreationHairColorManager", ExtResourceManagerType::CharacterCreationHairColor);
	MapResourceManagerIndex("eoc::CharacterCreationAccessorySetManager", ExtResourceManagerType::CharacterCreationAccessorySet);
	MapResourceManagerIndex("eoc::CharacterCreationEquipmentIconsManager", ExtResourceManagerType::CharacterCreationEquipmentIcons);
	MapResourceManagerIndex("eoc::CharacterCreationIconSettingsManager", ExtResourceManagerType::CharacterCreationIconSettings);
	MapResourceManagerIndex("eoc::CharacterCreationMaterialOverrideManager", ExtResourceManagerType::CharacterCreationMaterialOverride);
	MapResourceManagerIndex("eoc::CharacterCreationAppearanceMaterialManager", ExtResourceManagerType::CharacterCreationAppearanceMaterial);
	MapResourceManagerIndex("eoc::CharacterCreationPassiveAppearanceManager", ExtResourceManagerType::CharacterCreationPassiveAppearance);
	MapResourceManagerIndex("eoc::CharacterCreationAppearanceVisualManager", ExtResourceManagerType::CharacterCreationAppearanceVisual);
	MapResourceManagerIndex("eoc::CharacterCreationSharedVisualManager", ExtResourceManagerType::CharacterCreationSharedVisual);
	MapResourceManagerIndex("eoc::tutorial::EntriesManager", ExtResourceManagerType::TutorialEntries);
	MapResourceManagerIndex("eoc::FeatManager", ExtResourceManagerType::Feat);
	MapResourceManagerIndex("eoc::FeatDescriptionManager", ExtResourceManagerType::FeatDescription);
	MapResourceManagerIndex("eoc::tutorial::ModalEntriesManager", ExtResourceManagerType::TutorialModalEntries);
	MapResourceManagerIndex("eoc::ClassDescriptions", ExtResourceManagerType::ClassDescription);
	MapResourceManagerIndex("eoc::ColorDefinitions", ExtResourceManagerType::ColorDefinition);
	MapResourceManagerIndex("ls::FlagManager", ExtResourceManagerType::Flag);
	MapResourceManagerIndex("eoc::LevelMapValues", ExtResourceManagerType::LevelMap);
	
	initialized_ = true;
}

void EntitySystemHelpersBase::MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size, bool isProxy)
{
	auto it = componentNameToIndexMappings_.find(componentName);
	if (it != componentNameToIndexMappings_.end()) {
		components_[(unsigned)type].ComponentIndex = it->second.ComponentIndex;
		components_[(unsigned)type].ReplicationIndex = it->second.ReplicationIndex;

		if (it->second.ComponentIndex != UndefinedComponent) {
			componentIndexToTypeMappings_.insert(std::make_pair(it->second.ComponentIndex, type));
		}

		if (it->second.ReplicationIndex != UndefinedReplicationComponent) {
			replicationIndexToTypeMappings_.insert(std::make_pair(it->second.ReplicationIndex, type));
		}

		components_[(unsigned)type].Size = size;
		components_[(unsigned)type].IsProxy = isProxy;
	} else {
		OsiWarn("Could not find index for component: " << componentName);
	}
}

void EntitySystemHelpersBase::MapQueryIndex(char const* name, ExtQueryType type)
{
	auto it = queryMappings_.find(name);
	if (it != queryMappings_.end()) {
		queryIndices_[(unsigned)type] = it->second;
	} else {
		OsiWarn("Could not find index for query: " << name);
	}
}

void EntitySystemHelpersBase::MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type)
{
	auto it = staticDataMappings_.find(componentName);
	if (it != staticDataMappings_.end()) {
		staticDataIndices_[(unsigned)type] = it->second;
	} else {
		OsiWarn("Could not find index for resource manager: " << componentName);
	}
}

void* EntitySystemHelpersBase::GetRawComponent(Guid const& guid, ExtComponentType type)
{
	auto handle = GetEntityHandle(guid);
	if (handle) {
		return GetRawComponent(guid, type);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawComponent(FixedString const& guid, ExtComponentType type)
{
	auto handle = GetEntityHandle(guid);
	if (handle) {
		return GetRawComponent(guid, type);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawComponent(EntityHandle entityHandle, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto const& meta = GetComponentMeta(type);
	if (meta.ComponentIndex != UndefinedComponent) {
		return world->GetRawComponent(entityHandle, meta.ComponentIndex, meta.Size, meta.IsProxy);
	} else {
		return nullptr;
	}
}

UuidToHandleMappingComponent* EntitySystemHelpersBase::GetUuidMappings()
{
	auto query = GetQuery(ExtQueryType::UuidToHandleMapping);
	if (query) {
		auto const& meta = GetComponentMeta(ExtComponentType::UuidToHandleMapping);
		return reinterpret_cast<UuidToHandleMappingComponent*>(query->GetFirstMatchingComponent(meta.Size, meta.IsProxy));
	} else {
		return nullptr;
	}
}

void EntitySystemHelpersBase::Update()
{
	if (CheckLevel != RuntimeCheckLevel::FullECS) return;

	auto world = GetEntityWorld();
	auto pools = world->Components->ComponentPools;
	for (auto i = 0; i < components_.size(); i++) {
		auto const& componentInfo = components_[i];
		if (componentInfo.ComponentIndex != UndefinedComponent) {
			auto pool = pools.Find(componentInfo.ComponentIndex);
			if (pool != nullptr) {
				auto name = componentIndexToNameMappings_[componentInfo.ComponentIndex];
				if (pool->ComponentSizeInBytes != componentInfo.Size) {
					ERR("[ECS INTEGRITY CHECK] Component size mismatch (%s): local %d, ECS %d", name->c_str(), componentInfo.Size, pool->ComponentSizeInBytes);
				}
			}
		}
	}

	for (uint32_t i = 0; i < world->Components->ComponentsByType.Values.Used; i++) {
		auto componentId = world->Components->ComponentsByType.KeyAt(i);
		auto const& components = world->Components->ComponentsByType.Values[i];
		auto componentType = GetComponentType(componentId);
		if (componentType) {
			auto pm = GetPropertyMap(*componentType);
			if (pm != nullptr && components.Values.Used > 0) {
				for (uint32_t j = 0; j < components.Values.Used; j++) {
					auto component = components.Values[j];
					if (component != nullptr) {
						pm->ValidateObject(component);
					}
				}
			}
		}
	}
}

void EntitySystemHelpersBase::PostUpdate()
{
	if (CheckLevel != RuntimeCheckLevel::FullECS) return;

	auto world = GetEntityWorld();
	if (!world->Replication || !world->Replication->Dirty) return;

	for (unsigned i = 0; i < world->Replication->ComponentPools.size(); i++) {
		auto const& pool = world->Replication->ComponentPools[i];
		auto componentType = GetComponentType(ReplicationTypeIndex(i));
		if (componentType && pool.size() > 0) {
			auto pm = GetPropertyMap(*componentType);
			if (pm != nullptr) {
				for (auto const& entity : pool) {
					auto component = GetRawComponent(entity.Key(), *componentType);
					if (component) {
						pm->ValidateObject(component);
					}
				}
			}
		}
	}
}

EntityHandle EntitySystemHelpersBase::GetEntityHandle(FixedString const& guidString)
{
	auto guid = Guid::ParseGuidString(guidString.GetStringView());
	if (guid) {
		return GetEntityHandle(*guid);
	} else {
		return {};
	}
}

EntityHandle EntitySystemHelpersBase::GetEntityHandle(Guid const& uuid)
{
	auto entityMap = GetUuidMappings();
	if (entityMap) {
		auto handle = entityMap->Mappings.Find(uuid);
		if (handle) {
			return **handle;
		}
	}

	return {};
}

resource::GuidResourceBankBase* EntitySystemHelpersBase::GetRawResourceManager(ExtResourceManagerType type)
{
	auto index = staticDataIndices_[(unsigned)type];
	if (index == UndefinedIndex) {
		OsiError("No resource manager index mapping registered for " << type);
		return {};
	}

	auto defns = GetStaticSymbols().eoc__gGuidResourceManager;
	if (!defns || !*defns) {
		OsiError("Resource definition manager not available yet!");
		return {};
	}

	auto res = (*defns)->Definitions.Find(index);
	if (!res) {
		OsiError("Resource manager missing for " << type);
		return {};
	}

	return **res;
}

Query* EntitySystemHelpersBase::GetQuery(ExtQueryType type)
{
	auto index = queryIndices_[(unsigned)type];
	if (index == UndefinedIndex) {
		OsiError("No query index mapping registered for " << type);
		return {};
	}

	auto world = GetEntityWorld();
	if (!world) {
		return {};
	}

	return &world->Queries.Queries[index];
}

void ServerEntitySystemHelpers::Setup()
{
	UpdateComponentMappings();
}

void ClientEntitySystemHelpers::Setup()
{
	UpdateComponentMappings();
}



EntityWorld* ServerEntitySystemHelpers::GetEntityWorld()
{
	return GetStaticSymbols().GetServerEntityWorld();
}

EntityWorld* ClientEntitySystemHelpers::GetEntityWorld()
{
	return GetStaticSymbols().GetClientEntityWorld();
}

END_NS()
