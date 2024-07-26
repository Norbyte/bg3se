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

void ComponentCallbackHandler::DefaultCall(ComponentCallbackHandler const& self, ComponentCallbackParams const& arg, void* component)
{
	self.UserHandler(self.Object, arg, component);
}

ComponentCallbackHandler* ComponentCallbackHandler::DefaultCopy(void* dummy, ComponentCallbackHandler const& src, ComponentCallbackHandler* dst)
{
	*dst = src;
	return dst;
}

ComponentCallbackHandler* ComponentCallbackHandler::DefaultMoveDtor(void* dummy, ComponentCallbackHandler& src, ComponentCallbackHandler* dst)
{
	if (dst == nullptr) return nullptr;
	*dst = src;
	return dst;
}

ComponentCallbackHandler::ComponentCallbackHandler()
	: Call(&DefaultCall),
	Copy(&DefaultCopy),
	MoveDtor(&DefaultMoveDtor),
	Object(nullptr),
	UserHandler(nullptr)
{}

ComponentCallbackHandler::ComponentCallbackHandler(UserCallProc* handler, void* context)
	: Call(&DefaultCall),
	Copy(&DefaultCopy),
	MoveDtor(&DefaultMoveDtor),
	Object(context),
	UserHandler(handler)
{}


ComponentCallback::ComponentCallback() {}

ComponentCallback::~ComponentCallback()
{
	Handler.MoveDtor(nullptr, Handler, nullptr);
}

ComponentCallback::ComponentCallback(ComponentCallbackHandler const& handler, uint64_t index)
	: Handler(handler),
	RegistrantIndex(index)
{}

ComponentCallback::ComponentCallback(ComponentCallback const& o)
	: pHandler(&Handler),
	Handler(),
	Unused1(o.Unused1),
	Unused2(o.Unused2),
	RegistrantIndex(o.RegistrantIndex)
{
	o.Handler.Copy(nullptr, o.Handler, &Handler);
}

ComponentCallback::ComponentCallback(ComponentCallback&& o) noexcept
	: pHandler(&Handler),
	Handler(),
	Unused1(o.Unused1),
	Unused2(o.Unused2),
	RegistrantIndex(o.RegistrantIndex)
{
	o.Handler.MoveDtor(nullptr, o.Handler, &Handler);
}

ComponentCallback& ComponentCallback::operator = (ComponentCallback const& o)
{
	pHandler = &Handler;
	Unused1 = o.Unused1;
	Unused2 = o.Unused2;
	RegistrantIndex = o.RegistrantIndex;
	o.Handler.Copy(nullptr, o.Handler, &Handler);
	return *this;
}

ComponentCallback& ComponentCallback::operator = (ComponentCallback&& o) noexcept
{
	pHandler = &Handler;
	Unused1 = o.Unused1;
	Unused2 = o.Unused2;
	RegistrantIndex = o.RegistrantIndex;
	o.Handler.MoveDtor(nullptr, o.Handler, &Handler);
	return *this;
}


uint64_t ComponentCallbackList::Add(ComponentCallbackHandler const& handler)
{
	auto index = NextRegistrantId++;
	Callbacks.push_back(ComponentCallback{ handler, index });
	return index;
}

bool ComponentCallbackList::Remove(uint64_t registrantIndex)
{
	for (uint32_t i = 0; i < Callbacks.size(); i++) {
		if (Callbacks[i].RegistrantIndex == registrantIndex) {
			Callbacks.remove_at(i);
			return true;
		}
	}

	return false;
}


void* QueryDescription::GetFirstMatchingComponent(std::size_t componentSize, bool isProxy)
{
	for (auto const& cls : EntityClasses) {
		if (cls.Storage->InstanceToPageMap.size() > 0) {
			auto const& instPage = cls.Storage->InstanceToPageMap.values()[0];
			auto componentIdx = cls.GetComponentIndex(0);
			assert(cls.Storage->Components.size() >= 1);
			return cls.Storage->GetComponent(instPage, componentIdx, componentSize, isProxy);
		}
	}

	return {};
}

Array<void*> QueryDescription::GetAllMatchingComponents(std::size_t componentSize, bool isProxy)
{
	Array<void*> hits;

	for (auto const& cls : EntityClasses) {
		auto componentIdx = cls.GetComponentIndex(0);
		for (auto const& instance : cls.Storage->InstanceToPageMap) {
			auto component = cls.Storage->GetComponent(instance.Value(), componentIdx, componentSize, isProxy);
			hits.push_back(component);
		}
	}

	return hits;
}

ComponentTypeEntry const* ComponentRegistry::Get(ComponentTypeIndex index) const
{
	if (Bitmask[index.Value()]) {
		return &Types[index.Value()];
	} else {
		return nullptr;
	}
}

/*
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
}*/

void* EntityStorageData::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
	auto ref = InstanceToPageMap.try_get(entityHandle);
	if (ref) {
		return GetComponent(*ref, type, componentSize, isProxy);
	} else {
		return nullptr;
	}
}

void* EntityStorageData::GetComponent(EntityStorageIndex const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
	auto compIndex = ComponentTypeToIndex.try_get((uint16_t)type.Value());
	if (compIndex) {
		return GetComponent(entityPtr, *compIndex, componentSize, isProxy);
	} else {
		return nullptr;
	}
}

void* EntityStorageData::GetComponent(EntityStorageIndex const& entityPtr, uint8_t componentSlot, std::size_t componentSize, bool isProxy) const
{
	auto& page = Components[entityPtr.PageIndex]->Components[componentSlot];
	auto buf = (uint8_t*)page.ComponentBuffer;
	assert(buf != nullptr);
	if (isProxy) {
		auto ptr = buf + sizeof(void*) * entityPtr.EntryIndex;
		return *(uint8_t**)ptr;
	} else {
		return buf + componentSize * entityPtr.EntryIndex;
	}
}

void* ImmediateWorldCache::Changes::GetChange(EntityHandle entityHandle, ComponentTypeIndex type) const
{
	auto typeIdx = (uint16_t)type;
	if (AvailableComponentTypes[typeIdx]) {
		auto change = ComponentsByType[typeIdx].Components.Find(entityHandle);
		if (change) {
			return change->Ptr;
		}
	}

	return nullptr;
}
	
void* EntityWorld::GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy)
{
	auto storage = GetEntityStorage(entityHandle);
	if (storage != nullptr) {
		auto component = storage->GetComponent(entityHandle, type, componentSize, isProxy);
		if (component != nullptr) {
			return component;
		}
	}

	auto change = Cache->WriteChanges.GetChange(entityHandle, type);
	if (change != nullptr) {
		return change;
	}

	change = Cache->ReadChanges.GetChange(entityHandle, type);
	if (change != nullptr) {
		return change;
	}

	return nullptr;
}

bool EntityWorld::IsValid(EntityHandle entityHandle) const
{
	if (entityHandle.GetType() < 0x40) {
		auto& state = HandleGenerator->ThreadStates[entityHandle.GetType()];
		if (entityHandle.GetIndex() < state.Salts.Used) {
			auto const& salt = state.Salts[entityHandle.GetIndex()];
			return salt.Salt == entityHandle.GetSalt() && salt.Index == entityHandle.GetIndex();
		}
	}

	return false;
}

EntityStorageData* EntityStorageContainer::GetEntityStorage(EntityHandle entityHandle) const
{
	auto& componentSalts = Salts.Buckets[entityHandle.GetType()];
	if (entityHandle.GetIndex() < componentSalts.Used) {
		auto const& salt = componentSalts[entityHandle.GetIndex()];
		if (salt.Salt == entityHandle.GetSalt()) {
			return Entities[salt.EntityClassIndex];
		}
	}

	return nullptr;
}

EntityStorageData* EntityWorld::GetEntityStorage(EntityHandle entityHandle) const
{
	if (!IsValid(entityHandle)) {
		return nullptr;
	}

	return Storage->GetEntityStorage(entityHandle);
}

#if defined(NDEBUG)
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::Once };
#else
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::Always };
#endif

EntitySystemHelpersBase::EntitySystemHelpersBase()
	: queryIndices_{ UndefinedIndex },
	staticDataIndices_{ UndefinedIndex },
	systemIndices_{ UndefinedIndex }
{}

std::optional<ComponentTypeIndex> EntitySystemHelpersBase::GetComponentIndex(ExtComponentType type) const
{
	auto idx = components_[(unsigned)type].ComponentIndex;
	if (idx != UndefinedIndex && GetEntityWorld()->ComponentOps[idx.Value()] != nullptr) {
		return idx;
	} else {
		return {};
	}
}

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

	return pools[typeId].try_get(entity);
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
	auto syncFlags = pool.try_get(entity);
	if (syncFlags) {
		return syncFlags;
	} else {
		return pool.add_key(entity);
	}
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
	systemIndices_.fill(UndefinedIndex);

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
	MapSystemIndex("ecl::UISystem", ExtSystemType::UISystem);
	MapSystemIndex("ecl::PickingHelperManager", ExtSystemType::PickingHelperManager);

#define FOR_RESOURCE_TYPE(cls) MapResourceManagerIndex(resource::cls::EngineClass, resource::cls::ResourceManagerType);
	FOR_EACH_GUID_RESOURCE_TYPE()
#undef FOR_RESOURCE_TYPE

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

void EntitySystemHelpersBase::MapSystemIndex(char const* systemName, ExtSystemType type)
{
	auto it = systemIndexMappings_.find(systemName);
	if (it != systemIndexMappings_.end()) {
		systemIndices_[(unsigned)type] = it->second;
	} else {
		OsiWarn("Could not find index for system: " << systemName);
	}
}

void* EntitySystemHelpersBase::GetRawComponent(Guid const& guid, ExtComponentType type)
{
	auto handle = GetEntityHandle(guid);
	if (handle) {
		return GetRawComponent(handle, type);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawComponent(FixedString const& guid, ExtComponentType type)
{
	auto handle = GetEntityHandle(guid);
	if (handle) {
		return GetRawComponent(handle, type);
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

void* EntitySystemHelpersBase::GetRawSystem(ExtSystemType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto index = systemIndices_[(unsigned)type];
	if (index != UndefinedComponent.Value()) {
		return world->Systems.Systems[index].System;
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
	if (CheckLevel == RuntimeCheckLevel::FullECS) {
		ValidateEntityChanges();
	}
}

void EntitySystemHelpersBase::PostUpdate()
{
	if (!validated_ && GetEntityWorld() != nullptr) {
		ValidateMappedComponentSizes();
		validated_ = true;
	}

	if (CheckLevel == RuntimeCheckLevel::FullECS) {
		ValidateReplication();
	}
}

void EntitySystemHelpersBase::ValidateReplication()
{
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

void EntitySystemHelpersBase::ValidateMappedComponentSizes()
{
	auto world = GetEntityWorld();

	Array<ComponentTypeIndex> deletions;
	for (auto const& mapping : componentIndexToTypeMappings_) {
		if (!ValidateMappedComponentSize(world, mapping.first, mapping.second)) {
			deletions.push_back(mapping.first);
		}
	}

	for (auto typeId : deletions) {
		auto name = componentIndexToNameMappings_[typeId.Value()];
		ERR("[ECS INTEGRITY CHECK] Force unmap component %s", name->c_str());
		auto it = componentIndexToTypeMappings_.find(typeId);
		components_[(unsigned)it->second].ComponentIndex = UndefinedComponent;
		componentIndexToTypeMappings_.erase(it);
	}
}

bool EntitySystemHelpersBase::ValidateMappedComponentSize(ecs::EntityWorld* world, ComponentTypeIndex typeId, ExtComponentType extType)
{
	auto mapped = world->ComponentRegistry_.Get(typeId);
	auto const& local = components_[(unsigned)extType];
	auto name = componentIndexToNameMappings_[typeId.Value()];
	if (mapped != nullptr) {
		if (local.IsProxy) {
			if (mapped->InlineSize != sizeof(void*)) {
				ERR("[ECS INTEGRITY CHECK] '%s' marked as proxy, but entity only has inline data (%d)", 
					name->c_str(), mapped->InlineSize);
				return false;
			}

			if (mapped->InlineSize != sizeof(void*) || mapped->ComponentSize != local.Size) {
				ERR("[ECS INTEGRITY CHECK] '%s' OOB size mismatch: local %d, ECS %d", 
					name->c_str(), local.Size, mapped->ComponentSize);
				// Don't unmap since OOB mismatch doesn't invalidate component store layout
				return true;
			}
		} else {
			if (mapped->InlineSize != mapped->ComponentSize) {
				ERR("[ECS INTEGRITY CHECK] '%s' marked as non-proxy, but entity has OOB data (inline %d, OOB %d)",
					name->c_str(), mapped->InlineSize, mapped->ComponentSize);
				return false;
			}

			if (mapped->ComponentSize != local.Size) {
				ERR("[ECS INTEGRITY CHECK] '%s' size mismatch: local %d, ECS %d",
					name->c_str(), local.Size, mapped->ComponentSize);
				return false;
			}
		}
	}

	return true;
}

void EntitySystemHelpersBase::ValidateEntityChanges()
{
	auto world = GetEntityWorld();
	ValidateEntityChanges(world->Cache->WriteChanges);
	ValidateEntityChanges(world->Cache->ReadChanges);
}

void EntitySystemHelpersBase::ValidateEntityChanges(ImmediateWorldCache::Changes& changes)
{
	for (auto i = 0; i < components_.size(); i++) {
		auto const& componentInfo = components_[i];
		if (componentInfo.ComponentIndex != UndefinedComponent) {
			if (changes.AvailableComponentTypes[componentInfo.ComponentIndex.Value()]) {
				auto const& pool = changes.ComponentsByType[componentInfo.ComponentIndex.Value()];
				auto name = componentIndexToNameMappings_[componentInfo.ComponentIndex];
				auto componentSize = componentInfo.IsProxy ? sizeof(void*) : componentInfo.Size;
				if (pool.FrameStorage.ComponentSizeInBytes != componentSize) {
					ERR("[ECS INTEGRITY CHECK] Component size mismatch (%s): local %d, ECS %d", name->c_str(), componentSize, pool.FrameStorage.ComponentSizeInBytes);
				}
			}
		}
	}

	for (uint32_t componentId = 0; componentId < changes.AvailableComponentTypes.size(); componentId++) {
		if (changes.AvailableComponentTypes[componentId]) {
			auto const& components = changes.ComponentsByType[componentId].Components;
			auto componentType = GetComponentType(ComponentTypeIndex(componentId));
			if (componentType) {
				auto pm = GetPropertyMap(*componentType);
				if (pm != nullptr && components.Values.Used > 0) {
					for (uint32_t j = 0; j < components.Values.Used; j++) {
						auto const& component = components.Values[j];
						if (component.Ptr != nullptr) {
							pm->ValidateObject(component.Ptr);
						}
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
		auto handle = entityMap->Mappings.try_get(uuid);
		if (handle) {
			return *handle;
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

	auto res = (*defns)->Definitions.try_get(index);
	if (!res) {
		OsiError("Resource manager missing for " << type);
		return {};
	}

	return *res;
}

QueryDescription* EntitySystemHelpersBase::GetQuery(ExtQueryType type)
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



EntityWorld* ServerEntitySystemHelpers::GetEntityWorld() const
{
	return GetStaticSymbols().GetServerEntityWorld();
}

EntityWorld* ClientEntitySystemHelpers::GetEntityWorld() const
{
	return GetStaticSymbols().GetClientEntityWorld();
}

END_NS()
