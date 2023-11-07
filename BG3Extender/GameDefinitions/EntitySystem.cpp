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

void* Query::GetFirstMatchingComponent(std::size_t componentSize)
{
	for (auto const& cls : EntityClasses) {
		if (cls.EntityClass->InstanceToPageMap.size() > 0) {
			auto instPage = cls.EntityClass->InstanceToPageMap.Values[0];
			auto componentIdx = cls.GetComponentIndex(0);
			assert(cls.EntityClass->ComponentPools.size() >= 1);
			return cls.EntityClass->GetComponent(instPage, componentIdx, componentSize);
		}
	}

	return {};
}

Array<void*> Query::GetAllMatchingComponents(std::size_t componentSize)
{
	Array<void*> hits;

	for (auto const& cls : EntityClasses) {
		auto componentIdx = cls.GetComponentIndex(0);
		for (auto const& instance : cls.EntityClass->InstanceToPageMap) {
			auto component = cls.EntityClass->GetComponent(instance.Value(), componentIdx, componentSize);
			hits.push_back(component);
		}
	}

	return hits;
}

void* EntityClass::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize) const
{
	auto ref = InstanceToPageMap.Find(entityHandle);
	if (ref) {
		return GetComponent(**ref, type, componentSize);
	} else {
		return nullptr;
	}
}

void* EntityClass::GetComponent(InstanceComponentPointer const& entityPtr, ComponentTypeIndex type, std::size_t componentSize) const
{
	auto compIndex = ComponentTypeToIndex.Find((uint16_t)type.Value());
	if (compIndex) {
		return GetComponent(entityPtr, **compIndex, componentSize);
	} else {
		return nullptr;
	}
}

void* EntityClass::GetComponent(InstanceComponentPointer const& entityPtr, uint8_t componentSlot, std::size_t componentSize) const
{
	auto& page = ComponentPools[entityPtr.PageIndex][componentSlot];
	auto buf = (uint8_t*)page.ComponentBuffer;
	assert(buf != nullptr);
	return buf + componentSize * entityPtr.EntryIndex;
}

ComponentPool* EntityComponents::SparseHashMap::Get(ComponentTypeIndex index) const
{
	auto hash = ((index.Value() & 0x7FFF) + 0x780 * (index.Value() >> 15));
	if (hash < (int)SetValues.Size && SetValues[hash]) {
		auto idx = NextIds[hash];
		if (idx != -1 && Keys[idx] == index.Value()) {
			return Values[idx];
		}
	}

	return nullptr;
}
	
void* EntityWorld::GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize)
{
	auto entityClass = GetEntityClass(entityHandle);
	if (entityClass != nullptr) {
		auto component = entityClass->GetComponent(entityHandle, type, componentSize);
		if (component != nullptr) {
			return component;
		}
	}

	/*auto compPool = Components->ComponentsByType.Find((uint16_t)type);
	if (compPool) {
		auto transientRef = (*compPool)->Find(entityHandle.Handle);
		if (transientRef) {
			return **transientRef;
		}
	}

	auto compPool2 = Components->ComponentsByType2.Find((uint16_t)type);
	if (compPool2) {
		auto transientRef = (*compPool2)->Find(entityHandle.Handle);
		if (transientRef) {
			return **transientRef;
		}
	}*/

	return nullptr;
}
	
void* EntityWorld::GetRawComponent(char const* nameGuid, ComponentTypeIndex type, std::size_t componentSize)
{
	auto fs = NameGuidToFixedString(nameGuid);
	if (!fs) {
		OsiError("Could not map GUID '" << nameGuid << "' to FixedString");
		return nullptr;
	}

	return GetRawComponent(fs, type, componentSize);
}

void* EntityWorld::GetRawComponent(FixedString const& guid, ComponentTypeIndex type, std::size_t componentSize)
{
	ERR("FIXME!");
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

EntitySystemHelpersBase::EntitySystemHelpersBase()
	: componentIndices_{ UndefinedComponent }, 
	componentSizes_{ 0 },
	replicationIndices_{ UndefinedReplicationComponent },
	queryIndices_{ UndefinedIndex },
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
	if (replicationIndices_[(unsigned)type] == UndefinedReplicationComponent) {
		return nullptr;
	}

	return GetReplicationFlags(entity, replicationIndices_[(unsigned)type]);
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
	if (replicationIndices_[(unsigned)type] == UndefinedReplicationComponent) {
		return nullptr;
	}

	return GetOrCreateReplicationFlags(entity, replicationIndices_[(unsigned)type]);
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
	componentIndices_.fill(UndefinedComponent);
	componentSizes_.fill(0);
	replicationIndices_.fill(UndefinedReplicationComponent);
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

	#define T(cls) MapComponentIndices(cls::EngineClass, cls::ComponentType, sizeof(cls));
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

void EntitySystemHelpersBase::MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size)
{
	auto it = componentNameToIndexMappings_.find(componentName);
	if (it != componentNameToIndexMappings_.end()) {
		componentIndices_[(unsigned)type] = it->second.ComponentIndex;
		replicationIndices_[(unsigned)type] = it->second.ReplicationIndex;

		if (it->second.ComponentIndex != UndefinedComponent) {
			componentIndexToTypeMappings_.insert(std::make_pair(it->second.ComponentIndex, type));
		}

		if (it->second.ReplicationIndex != UndefinedReplicationComponent) {
			replicationIndexToTypeMappings_.insert(std::make_pair(it->second.ReplicationIndex, type));
		}

		componentSizes_[(unsigned)type] = size;
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

void* EntitySystemHelpersBase::GetRawComponent(char const* nameGuid, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetRawComponent(nameGuid, *componentIndex, componentSizes_[(unsigned)type]);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawComponent(FixedString const& guid, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetRawComponent(guid, *componentIndex, componentSizes_[(unsigned)type]);
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

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetRawComponent(entityHandle, *componentIndex, componentSizes_[(unsigned)type]);
	} else {
		return nullptr;
	}
}

UuidToHandleMappingComponent* EntitySystemHelpersBase::GetUuidMappings()
{
	auto query = GetQuery(ExtQueryType::UuidToHandleMapping);
	if (query) {
		return reinterpret_cast<UuidToHandleMappingComponent*>(query->GetFirstMatchingComponent(componentSizes_[(unsigned)ExtComponentType::UuidToHandleMapping]));
	} else {
		return nullptr;
	}
}

EntityHandle EntitySystemHelpersBase::GetEntityHandle(Guid uuid)
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
