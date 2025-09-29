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

void* QueryDescription::GetFirstMatchingComponent(std::size_t componentSize, bool isProxy)
{
    for (auto const& cls : EntityStorages.values()) {
        if (cls.Storage->InstanceToPageMap.size() > 0) {
            auto const& instPage = cls.Storage->InstanceToPageMap.values()[0];
            auto componentIdx = cls.GetComponentIndex(0);
            se_assert(cls.Storage->Components.size() >= 1);
            return cls.Storage->GetComponent(instPage, componentIdx, componentSize, isProxy);
        }
    }

    return {};
}

void PrintRange(EntitySystemHelpersBase& eh, char const* name, std::span<ComponentTypeIndex const> inds)
{
    if (!inds.empty()) std::cout << "\t" << name << ": ";
    for (auto i : inds) {
        std::cout << " " << *eh.GetComponentName(i);
    }
    if (!inds.empty()) std::cout << std::endl;
}

void QueryDescription::DebugPrint(QueryIndex index, EntitySystemHelpersBase& eh) const
{
    auto const& queries = eh.GetEntityWorld()->Queries;

    std::cout << "Query ";
    if ((Flags & QueryFlags::Modified) == QueryFlags::Modified) std::cout << " Modified";
    if ((Flags & QueryFlags::Added) == QueryFlags::Added) std::cout << " Added";
    if ((Flags & QueryFlags::Removed) == QueryFlags::Removed) std::cout << " Removed";
    if (queries.PersistentQueries.find(index) != queries.PersistentQueries.end()) std::cout << " Persistent";
    if (queries.AliveQueries.find(index) != queries.AliveQueries.end()) std::cout << " Alive";
    if (queries.RemovedQueries.find(index) != queries.RemovedQueries.end()) std::cout << " Removed2";
    if (queries.DeadQueries.find(index) != queries.DeadQueries.end()) std::cout << " Dead";
    if (queries.DeadOneFrameQueries.find(index) != queries.DeadOneFrameQueries.end()) std::cout << " DeadOneFrame";
    std::cout << std::endl;

    PrintRange(eh, "Includes", GetIncludes());
    PrintRange(eh, "IncludeAny", GetIncludeAny());
    PrintRange(eh, "Excludes", GetExcludes());
    PrintRange(eh, "AddOrs", GetAddOrs());
    PrintRange(eh, "AddAnds", GetAddAnds());
    PrintRange(eh, "OneFrames", GetOneFrames());
    PrintRange(eh, "OptionalOneFrames", GetOptionalOneFrames());
    PrintRange(eh, "Optionals", GetOptionals());
    PrintRange(eh, "Immediates", GetImmediates());
    PrintRange(eh, "Writes", GetWrites());

    std::cout << std::endl;
}

ComponentTypeEntry const* ComponentRegistry::Get(ComponentTypeIndex index) const
{
    auto idx = (uint32_t)SparseHashMapHash(index);
    if (Bitmask[idx]) {
        return &Types[idx];
    } else {
        return nullptr;
    }
}

ComponentCallbacks* ComponentCallbackRegistry::Get(ComponentTypeIndex index)
{
    auto idx = (uint32_t)SparseHashMapHash(index);
    if (idx < Callbacks.size()) {
        return Callbacks[idx];
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

FieldTracker::~FieldTracker() {}


void* FrameAllocator::Allocate(uint16_t size)
{
    auto realSize = (uint16_t)(size + 63) & 0xFFC0u;

    auto curPage = CurrentPage;
    auto offset = (uint64_t)InterlockedExchangeAdd64(&curPage->Offset, realSize);
    while (offset + realSize > PageSize) {

        EnterCriticalSection(&CS);
        if (curPage == CurrentPage) {
            auto page = AllocPage();
            page->Offset = 0x40;
            CurrentPage = page;
        }
        LeaveCriticalSection(&CS);

        curPage = CurrentPage;
        offset = (uint64_t)InterlockedExchangeAdd64(&curPage->Offset, realSize);
    }

    return curPage + offset;
}

FrameAllocator::FrameBuffer* FrameAllocator::AllocPage()
{
    auto page = GameAllocRaw(PageSize);
    FrameBuffers[0].push_back(page);
    return static_cast<FrameBuffer*>(page);
}

void FrameAllocator::Free(void* ptr)
{
    // Per-frame data is batch released at the end of the frame
}

bool EntityHandleGenerator::IsEntityAlive(EntityHandle entity) const
{
    if (entity.GetThreadIndex() < ThreadStates.size()) {
        auto const& state = ThreadStates[entity.GetThreadIndex()];
        if (entity.GetIndex() < state.Entries.size()) {
            auto const& entry = state.Entries[entity.GetIndex()];
            return entry.Index == entity.GetIndex()
                && entry.Salt == entity.GetSalt();
        }
    }

    return false;
}

EntityHandle EntityHandleGenerator::Create()
{
    auto tid = ThreadRegistry::RequestThreadIndex();
    auto entry = ThreadStates[tid].Add();
    return EntityHandle(tid, entry->Index, entry->Salt);
}

EntityHandleGenerator::ThreadState::Entry* EntityHandleGenerator::ThreadState::Add()
{
    if (NumFreeSlots < Entries.bucket_size() / 2) {
        auto oldCapacity = Entries.capacity();
        if (Entries.capacity() < Entries.size() + Entries.bucket_size()) {
            Entries.resize(Entries.capacity() + Entries.bucket_size());
        }

        auto growSize = Entries.capacity() - oldCapacity;
        for (uint32_t i = 0; i < growSize; i++) {
            auto index = Entries.size();
            auto entry = Entries.add();
            *entry = Entry{
                .Index = index,
                .Salt = 1
            };
            if (NumFreeSlots > 0) {
                Entries[LastFreeSlotIndex].Index = index;
            } else {
                NextFreeSlotIndex = index;
            }

            LastFreeSlotIndex = index;
            NumFreeSlots++;
        }
    }

    auto index = NextFreeSlotIndex;
    NumFreeSlots--;
    auto entry = &Entries[index];
    NextFreeSlotIndex = entry->Index;
    entry->Index = index;
    return entry;
}

void* EntityStorageData::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
    auto ref = InstanceToPageMap.try_get(entityHandle);
    if (ref) {
        return GetComponent(*ref, type, componentSize, isProxy);
    } else {
        return nullptr;
    }
}

void* EntityStorageData::GetOneFrameComponent(EntityHandle entityHandle, ComponentTypeIndex type) const
{
    auto pool = OneFrameComponents.try_get(type);
    if (pool) {
        return pool->get_or_default(entityHandle);
    }

    return nullptr;
}

void* EntityStorageData::GetComponent(EntityStorageIndex const& entityPtr, ComponentTypeIndex type, std::size_t componentSize, bool isProxy) const
{
    auto compIndex = ComponentTypeToIndex.try_get(type);
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
    se_assert(buf != nullptr);
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
        auto change = ComponentsByType[typeIdx].Components.find(entityHandle);
        if (change) {
            return change->Ptr;
        }
    }

    return nullptr;
}

ImmediateWorldCache::ComponentChanges* ImmediateWorldCache::Changes::AddComponentChanges(ComponentTypeEntry const* type, FrameAllocator* allocator)
{
    auto typeIdx = (uint16_t)type->TypeId;
    auto components = ComponentsByType + typeIdx;
    if (!AvailableComponentTypes[typeIdx]) {
        AvailableComponentTypes.AtomicSet(typeIdx);
        new (components) ComponentChanges(allocator);
        components->FrameStorage.ComponentTypeId = type->TypeId;
        components->FrameStorage.ComponentSizeInBytes = type->InlineSize;
        components->FrameStorage.DestructorProc = type->DtorProc;
    }

    return components;
}

ImmediateWorldCache::ComponentChanges* ImmediateWorldCache::AddComponentChanges(ComponentTypeIndex type)
{
    auto typeIdx = (uint16_t)type;
    auto typeInfo = EntityWorld->ComponentRegistry_.Get(type);
    return WriteChanges.AddComponentChanges(typeInfo, Allocator);
}

bool ImmediateWorldCache::RemoveComponent(EntityHandle entity, ComponentTypeIndex type)
{
    auto changes = AddComponentChanges(type);
    auto change = changes->Components.find(entity);
    if (!change) {
        auto typeInfo = EntityWorld->ComponentRegistry_.Get(type);
        // TODO - different behavior for proxy objects?
        auto component = EntityWorld->GetRawComponent(entity, type, typeInfo->InlineSize, false);
        if (component) {
            auto& onDestroy = Callbacks->Get(type)->OnDestroy;
            EntityRef e{
                .Handle = entity,
                .World = EntityWorld
            };
            onDestroy.Invoke(&e, component);

            change = changes->Components.add_uninitialized(entity);
            // Default change means deletion
            new (change) ComponentChange();
            return true;
        } else {
            WARN("Tried to remove component [%d] that does not exist on entity [%08x]!", type, entity.Handle);
            return false;
        }
    } else {
        WARN("A change for component [%d] already exists on entity [%08x]!", type, entity.Handle);
        return false;
    }
}

ECSComponentDataMap::ECSComponentDataMap()
{
    nullComponentData_.Name = new STDString();
}

PerECSComponentData const& ECSComponentDataMap::Get(ComponentTypeIndex type) const
{
    auto idx = (uint32_t)SparseHashMapHash(type);
    if (idx < componentData_.size()) {
        return componentData_[idx];
    } else {
        return nullComponentData_;
    }
}

PerECSComponentData& ECSComponentDataMap::GetOrAdd(ComponentTypeIndex type)
{
    auto idx = (uint32_t)SparseHashMapHash(type);
    if (idx > componentData_.size()) {
        componentData_.resize(idx + 1);
    }

    return componentData_[idx];
}

PerECSReplicationData const& ECSComponentDataMap::Get(ReplicationTypeIndex type) const
{
    auto idx = (unsigned)type;
    if (idx < replicationData_.size()) {
        return replicationData_[idx];
    } else {
        return nullReplicationData_;
    }
}

PerECSReplicationData& ECSComponentDataMap::GetOrAdd(ReplicationTypeIndex type)
{
    auto idx = (unsigned)type;
    if (idx >= replicationData_.size()) {
        replicationData_.resize(idx + 1);
    }

    return replicationData_[idx];
}

void ECSComponentDataMap::Clear()
{
    componentData_.clear();
    replicationData_.clear();
}

ComponentOps* ComponentOpsRegistry::Get(ComponentTypeIndex id) const
{
    auto idx = (uint32_t)SparseHashMapHash(id);
    return idx < Ops.size() ? Ops[idx] : nullptr;
}

ECBEntityChangeSet const* ECBData::GetEntityChange(EntityHandle const& entity) const
{
    return EntityChanges.find(entity);
}

ECBEntityChangeSet* ECBData::GetOrAddEntityChange(EntityHandle const& entity)
{
    auto changes = EntityChanges.find(entity);
    if (changes == nullptr) {
        changes = EntityChanges.add_uninitialized(entity);
        new (changes) ECBEntityChangeSet(EntityChanges.allocator().Allocator);
    }

    return changes;
}

EntityHandle EntityCommandBuffer::CreateEntity()
{
    auto entity = HandleGenerator->Create();
    auto change = Data.GetOrAddEntityChange(entity);
    change->Flags |= EntityChangeFlags::Create;
    return entity;
}

EntityHandle EntityCommandBuffer::CreateEntityImmediate()
{
    auto entity = HandleGenerator->Create();
    auto change = Data.GetOrAddEntityChange(entity);
    change->Flags |= EntityChangeFlags::Create | EntityChangeFlags::Immediate;
    return entity;
}

bool EntityCommandBuffer::DestroyEntity(EntityHandle entity)
{
    if (HandleGenerator->IsEntityAlive(entity)) {
        auto change = Data.GetOrAddEntityChange(entity);
        if ((change->Flags & EntityChangeFlags::Create) == EntityChangeFlags::Create) {
            ERR("Cannot create and destroy an entity in the same frame");
        } else {
            change->Flags |= EntityChangeFlags::Destroy;
            return true;
        }
    }

    return false;
}

void* EntityCommandBuffer::GetComponentChange(ComponentTypeIndex type, EntityStorageData::EntityStorageIndex const& index) const
{
    auto pool = Data.ComponentPools.Find(type);
    if (index && pool) {
        return pool->GetComponent(index);
    }

    return nullptr;
}

void* EntityWorld::GetRawComponent(EntityHandle entityHandle, ComponentTypeIndex type, std::size_t componentSize, bool isProxy)
{
    auto storage = GetEntityStorage(entityHandle);
    if (IsOneFrame(type)) {
        if (storage != nullptr) {
            return storage->GetOneFrameComponent(entityHandle, type);
        }
    } else {
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
    }

    return nullptr;
}

bool EntityWorld::IsValid(EntityHandle entityHandle) const
{
    if (entityHandle.GetThreadIndex() < std::size(HandleGenerator->ThreadStates)) {
        auto& state = HandleGenerator->ThreadStates[entityHandle.GetThreadIndex()];
        if (entityHandle.GetIndex() < state.Entries.size()) {
            auto const& salt = state.Entries[entityHandle.GetIndex()];
            return salt.Salt == entityHandle.GetSalt() && salt.Index == entityHandle.GetIndex();
        }
    }

    return false;
}

EntityCommandBuffer* EntityWorld::Deferred()
{
    return &CommandBuffers[ThreadRegistry::RequestThreadIndex()];
}

EntityStorageData* EntityStorageContainer::GetEntityStorage(EntityHandle entityHandle) const
{
    if (entityHandle.GetThreadIndex() >= Salts.Buckets.size()) {
        return nullptr;
    }

    auto& componentSalts = Salts.Buckets[entityHandle.GetThreadIndex()];
    if (entityHandle.GetIndex() < componentSalts.size()) {
        auto const& salt = componentSalts[entityHandle.GetIndex()];
        if (salt.Salt == entityHandle.GetSalt()) {
            return Entities[salt.EntityClassIndex];
        }
    }

    return nullptr;
}

EntityStorageData* EntityWorld::GetEntityStorage(EntityHandle entityHandle) const
{
    return Storage->GetEntityStorage(entityHandle);
}


StringView ECSComponentLog::GetName() const
{
    auto name = GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers()->GetComponentName(ComponentType);
    return name ? *name : StringView{};
}

std::optional<ExtComponentType> ECSComponentLog::GetType() const
{
    return GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers()->GetComponentType(ComponentType);
}

void ECSChangeLog::Clear()
{
    Entities.clear();
}

void ECSChangeLog::AddEntityChange(EntityHandle entity, EntityChangeFlags flags)
{
    auto entry = Entities.get_or_add(entity);
    entry->Flags = entry->Flags | flags;
}

void ECSChangeLog::AddComponentChange(EntityWorld* world, EntityHandle entity, ComponentTypeIndex type, ComponentChangeFlags flags)
{
    auto componentInfo = world->ComponentRegistry_.Get(type);

    if (componentInfo->OneFrame) {
        flags |= ComponentChangeFlags::OneFrame;
    }
    
    if (componentInfo->Replicated) {
        flags |= ComponentChangeFlags::ReplicatedComponent;
    }

    auto entry = Entities.get_or_add(entity);
    auto componentEntry = entry->Components.get_or_add((uint16_t)type);
    componentEntry->ComponentType = type;
    componentEntry->Flags = componentEntry->Flags | flags;
}


#if defined(NDEBUG)
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::Once };
#else
RuntimeCheckLevel EntitySystemHelpersBase::CheckLevel{ RuntimeCheckLevel::FullECS };
#endif

EntitySystemHelpersBase::EntitySystemHelpersBase()
    : staticDataIndices_{ resource::UndefinedStaticDataType },
    systemIndices_{ UndefinedSystem }
{}

std::optional<ComponentTypeIndex> EntitySystemHelpersBase::GetComponentIndex(ExtComponentType type) const
{
    auto idx = components_[(unsigned)type].ComponentIndex;
    if (idx != UndefinedComponent && GetEntityWorld()->ComponentOps.Get(idx) != nullptr) {
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

void EntitySystemHelpersBase::BindSystem(StringView name, SystemTypeIndex systemId)
{
    auto it = systemIndexMappings_.insert(std::make_pair(name, systemId));
    if (systemTypeIdToName_.size() <= (unsigned)systemId) {
        systemTypeIdToName_.resize((unsigned)systemId + 1);
    }

    systemTypeIdToName_[(unsigned)systemId] = &it.first->first;
}

void EntitySystemHelpersBase::BindStaticData(StringView name, resource::StaticDataTypeIndex id)
{
    auto it = staticDataMappings_.insert(std::make_pair(name, id));
    if (staticDataIdToName_.size() <= (unsigned)id) {
        staticDataIdToName_.resize((unsigned)id + 1);
    }

    staticDataIdToName_[(unsigned)id] = &it.first->first;
}

void EntitySystemHelpersBase::BindComponent(StringView name, ComponentTypeIndex id)
{
    STDString const* pName;
    auto it = componentNameToIndexMappings_.find(STDString(name));
    if (it == componentNameToIndexMappings_.end()) {
        auto iit = componentNameToIndexMappings_.insert(std::make_pair(name, IndexMappings{id, UndefinedReplicationComponent}));
        pName = &iit.first->first;
    } else {
        it->second.ComponentIndex = id;
        pName = &it->first;
    }

    auto& binding = ecsComponentData_.GetOrAdd(id);
    binding.Name = pName;
}

void EntitySystemHelpersBase::BindReplication(StringView name, ReplicationTypeIndex id)
{
    auto it = componentNameToIndexMappings_.find(STDString(name));
    if (it == componentNameToIndexMappings_.end()) {
        componentNameToIndexMappings_.insert(std::make_pair(name, IndexMappings{UndefinedComponent, id}));
    } else {
        it->second.ReplicationIndex = id;
    }
}

void EntitySystemHelpersBase::UpdateComponentMappings()
{
    if (initialized_) return;

    componentNameToIndexMappings_.clear();
    ecsComponentData_.Clear();
    components_.fill(PerComponentData{});
    staticDataIndices_.fill(resource::UndefinedStaticDataType);
    systemIndices_.fill(UndefinedSystem);

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
            // Queries ignored
        } else {
            auto contextIt = contexts.find(mapping.second.context);
            if (contextIt != contexts.end()) {
                switch (contextIt->second) {
                case TypeIdContext::System:
                    BindSystem(name, SystemTypeIndex(*mapping.first));
                    break;

                case TypeIdContext::ImmutableData:
                    BindStaticData(name, resource::StaticDataTypeIndex(*mapping.first));
                    break;

                case TypeIdContext::Component:
                    BindComponent(name, ComponentTypeIndex(*mapping.first));
                    break;

                case TypeIdContext::OneFrameComponent:
                    BindComponent(name, ComponentTypeIndex(*mapping.first | 0x8000));
                    break;

                case TypeIdContext::Replication:
                    BindReplication(name, ReplicationTypeIndex(*mapping.first));
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

    #define T(cls) MapComponentIndices(cls::EngineClass, cls::ComponentType, sizeof(cls), std::is_base_of_v<BaseProxyComponent, cls> || cls::ForceProxy);
    #include <GameDefinitions/Components/AllComponentTypes.inl>
    #undef T

    #define T(cls) MapSystemIndex(cls::EngineClass, cls::SystemType);
    #include <GameDefinitions/Components/AllSystemTypes.inl>
    #undef T

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
            auto& binding = ecsComponentData_.GetOrAdd(it->second.ComponentIndex);
            binding.Name = &it->first;
            binding.ExtType = type;
            binding.ReplicationType = it->second.ReplicationIndex;
        }

        if (it->second.ReplicationIndex != UndefinedReplicationComponent) {
            auto& binding = ecsComponentData_.GetOrAdd(it->second.ReplicationIndex);
            binding.Name = &it->first;
            binding.ExtType = type;
            binding.ComponentType = it->second.ComponentIndex;
        }

        se_assert(size < 0x10000);
        components_[(unsigned)type].Size = (uint16_t)size;
        components_[(unsigned)type].IsProxy = isProxy;
    } else {
        OsiWarn("Could not find index for component: " << componentName);
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

void* EntitySystemHelpersBase::GetRawSingleton(ExtComponentType type)
{
    auto& meta = GetComponentMeta(type);
    if (meta.SingleComponentQuery == ecs::UndefinedQuery) {
        WARN("No query defined for singleton %s?", GetComponentName(meta.ComponentIndex)->c_str());
        return nullptr;
    }

    auto world = GetEntityWorld();
    auto const& query = world->Queries.Queries[(unsigned)meta.SingleComponentQuery];
    if (query.EntityStorages.empty()) {
        return nullptr;
    }

    auto const& storage = query.EntityStorages.values()[0];
    if (storage.Storage->InstanceToPageMap.empty()) {
        return nullptr;
    }

    auto page = storage.Storage->InstanceToPageMap.values()[0];
    return storage.Storage->GetComponent(page, storage.GetComponentIndex(0), meta.Size, meta.IsProxy);
}

SystemTypeEntry* EntitySystemHelpersBase::GetSystemEntry(ExtSystemType type)
{
    auto world = GetEntityWorld();
    if (!world) {
        return nullptr;
    }

    auto index = systemIndices_[(unsigned)type];
    if (index != UndefinedSystem) {
        return &world->Systems.Systems[(unsigned)index];
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
    if (index != UndefinedSystem) {
        return world->Systems.Systems[(unsigned)index].System;
    } else {
        return nullptr;
    }
}

void EntitySystemHelpersBase::Update()
{
    OPTICK_EVENT();
    if (CheckLevel == RuntimeCheckLevel::FullECS) {
        ValidateECBFlushChanges();
        ValidateEntityChanges();
    }

    ThrowECBFlushEvents();

    if (logging_) {
        DebugLogECBFlushChanges();
        DebugLogUpdateChanges();
    }
}

void EntitySystemHelpersBase::DebugLogUpdateChanges()
{
    OPTICK_EVENT(Optick::Category::Debug);
    auto world = GetEntityWorld();

    auto const& changes = world->Cache->WriteChanges;
    for (unsigned i = 0; i < changes.AvailableComponentTypes.NumBits; i++) {
        if (changes.AvailableComponentTypes[i]) {
            auto const& changeSet = changes.ComponentsByType[i];
            for (unsigned j = 0; j < changeSet.Components.size(); j++) {
                auto entityHandle = changeSet.Components.key_at(j);
                auto const& change = changeSet.Components.Values[j];

                log_.AddComponentChange(world, entityHandle, ComponentTypeIndex{ (uint16_t)i }, change.Ptr ? ComponentChangeFlags::Create : ComponentChangeFlags::Destroy);
            }
        }
    }
}

void EntitySystemHelpersBase::PostUpdate()
{
    OPTICK_EVENT();
    if (!validated_ && GetEntityWorld() != nullptr) {
        ValidateMappedComponentSizes();
        UpdateQueryCache();
        validated_ = true;
    }

    if (CheckLevel == RuntimeCheckLevel::FullECS) {
        ValidateReplication();
    }

    if (logging_) {
        DebugLogReplicationChanges();
    }
}

void EntitySystemHelpersBase::OnInit()
{
    InitSystemUpdateHooks();
}

void EntitySystemHelpersBase::OnDestroy()
{
    ClearSystemUpdateHooks();
}

void EntitySystemHelpersBase::DebugLogReplicationChanges()
{
    auto world = GetEntityWorld();
    if (!logging_) return;

    ecs::SyncBuffers* buffers = world->Replication;
    if (!buffers) {
        buffers = *GetStaticSymbols().GetEoCClient()->GameClient->ReplicationPeer.Buffers;
    }

    if (!buffers) return;

    OPTICK_EVENT(Optick::Category::Debug);
    for (unsigned i = 0; i < buffers->ComponentPools.size(); i++) {
        auto const& pool = buffers->ComponentPools[i];
        if (pool.size() > 0) {
            for (auto const& entity : pool) {
                auto type = ecsComponentData_.Get(ReplicationTypeIndex{ (uint16_t)i }).ComponentType;

                if (type != UndefinedComponent) {
                    log_.AddComponentChange(world, entity.Key(), type, ComponentChangeFlags::Replicate);
                }
            }
        }
    }
}

void EntitySystemHelpersBase::OnFlushECBs()
{
    if (CheckLevel == RuntimeCheckLevel::FullECS) {
        ValidateECBFlushChanges();
    }

    ThrowECBFlushEvents();

    if (logging_) {
        DebugLogECBFlushChanges();
    }
}

void EntitySystemHelpersBase::DebugLogECBFlushChanges()
{
    OPTICK_EVENT(Optick::Category::Debug);
    auto world = GetEntityWorld();

    for (auto& ecb : world->CommandBuffers) {
        for (unsigned i = 0; i < ecb.Data.EntityChanges.size(); i++) {
            auto entityHandle = ecb.Data.EntityChanges.key_at(i);
            auto const& entityChanges = ecb.Data.EntityChanges.Values[i];

            log_.AddEntityChange(entityHandle, entityChanges.Flags);

            for (unsigned j = 0; j < entityChanges.Store.size(); j++) {
                auto const& upd = entityChanges.Store[j];

                log_.AddComponentChange(world, entityHandle, upd.ComponentTypeId, 
                    (upd.PoolIndex.PageIndex != 0xffff) ? ComponentChangeFlags::Create : ComponentChangeFlags::Destroy);
            }
        }
    }
}

void EntitySystemHelpersBase::ThrowECBFlushEvents()
{
    auto state = GetExtensionState();
    if (state == nullptr || !state->GetLua()) return;

    OPTICK_EVENT();
    auto world = GetEntityWorld();
    auto& hooks = state->GetLua()->GetComponentEventHooks();

    for (auto& ecb : world->CommandBuffers) {
        for (unsigned i = 0; i < ecb.Data.EntityChanges.size(); i++) {
            auto entity = ecb.Data.EntityChanges.key_at(i);
            auto const& entityChanges = ecb.Data.EntityChanges.Values[i];

            for (unsigned j = 0; j < entityChanges.Store.size(); j++) {
                auto const& upd = entityChanges.Store[j];
                if (upd.PoolIndex.PageIndex != 0xffff) {
                    auto typeInfo = GetComponentMeta(upd.ComponentTypeId);
                    if (typeInfo && typeInfo->OneFrame) {
                        hooks.OnEntityEvent(*world, entity, upd.ComponentTypeId, lua::EntityComponentEvent::Create, nullptr);
                    }
                }
            }
        }
    }
}

void EntitySystemHelpersBase::ValidateReplication()
{
    auto world = GetEntityWorld();
    if (!world->Replication || !world->Replication->Dirty) return;

    OPTICK_EVENT(Optick::Category::Debug);
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
    OPTICK_EVENT(Optick::Category::Debug);
    auto world = GetEntityWorld();

    Array<ComponentTypeIndex> deletions;
    for (unsigned componentIdx = 0; componentIdx < components_.size(); componentIdx++) {
        auto componentType = components_[componentIdx].ComponentIndex;
        if (componentType != UndefinedComponent) {
            if (!ValidateMappedComponentSize(world, componentType, ExtComponentType{ componentIdx })) {
                deletions.push_back(componentType);
            }

            auto registryEntry = world->ComponentRegistry_.Get(componentType);
            if (registryEntry) {
                components_[componentIdx].OneFrame = registryEntry->OneFrame;
            }
        }
    }

    for (auto typeId : deletions) {
        auto& mapping = ecsComponentData_.GetOrAdd(typeId);
        if (mapping.ExtType) {
            ERR("[ECS INTEGRITY CHECK] Force unmap component %s", mapping.Name->c_str());
            components_[(unsigned)*mapping.ExtType].ComponentIndex = UndefinedComponent;
            mapping.ExtType = {};
        }
    }
}

bool EntitySystemHelpersBase::ValidateMappedComponentSize(ecs::EntityWorld* world, ComponentTypeIndex typeId, ExtComponentType extType)
{
    auto mapped = world->ComponentRegistry_.Get(typeId);
    auto const& local = components_[(unsigned)extType];
    auto name = ecsComponentData_.Get(typeId).Name;
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

void EntitySystemHelpersBase::ValidateECBFlushChanges()
{
    OPTICK_EVENT(Optick::Category::Debug);
    auto world = GetEntityWorld();
    EntityHandle last{};

    for (auto& ecb : world->CommandBuffers) {
        for (unsigned i = 0; i < ecb.Data.EntityChanges.size(); i++) {
            auto entityHandle = ecb.Data.EntityChanges.key_at(i);
            auto const& entityChanges = ecb.Data.EntityChanges.Values[i];

            for (unsigned j = 0; j < entityChanges.Store.size(); j++) {
                auto const& change = entityChanges.Store[j];

                if (change.PoolIndex.PageIndex != 0xffff) {
                    auto componentType = GetComponentType(change.ComponentTypeId);
                    if (componentType) {
                        auto const& meta = GetComponentMeta(*componentType);
                        auto pm = GetPropertyMap(*componentType);
                        if (pm != nullptr) {
                            auto component = ecb.GetComponentChange(change.ComponentTypeId, change.PoolIndex);
                            if (component) {
                                if (meta.IsProxy) {
                                    pm->ValidateObject(*(void**)component);
                                } else {
                                    pm->ValidateObject(component);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void EntitySystemHelpersBase::ValidateEntityChanges()
{
    OPTICK_EVENT(Optick::Category::Debug);
    auto world = GetEntityWorld();
    ValidateEntityChanges(world->Cache->WriteChanges);
}

void EntitySystemHelpersBase::ValidateEntityChanges(ImmediateWorldCache::Changes& changes)
{
    for (auto i = 0; i < components_.size(); i++) {
        auto const& componentInfo = components_[i];
        if (componentInfo.ComponentIndex != UndefinedComponent) {
            if (changes.AvailableComponentTypes[(unsigned)componentInfo.ComponentIndex]) {
                auto const& pool = changes.ComponentsByType[(unsigned)componentInfo.ComponentIndex];
                auto name = ecsComponentData_.Get(componentInfo.ComponentIndex).Name;
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
                if (pm != nullptr && components.Values.size() > 0) {
                    for (uint32_t j = 0; j < components.Values.size(); j++) {
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

void EntitySystemHelpersBase::MapSingleComponentQuery(QueryIndex queryIndex, ComponentTypeIndex component)
{
    auto extComponent = GetComponentType(component);
    if (!extComponent) return;

    auto& desc = components_[(unsigned)*extComponent];
    if (desc.SingleComponentQuery != ecs::UndefinedQuery) {
        auto const& currentQuery = GetEntityWorld()->Queries.Queries[(unsigned)desc.SingleComponentQuery];
        auto const& newQuery = GetEntityWorld()->Queries.Queries[(unsigned)queryIndex];

        // Only allow replacing the query if it is different from the previous and has less optionals/adds
        if (!(
            newQuery.GetOptionalOneFrames().size() < currentQuery.GetOptionalOneFrames().size()
            || newQuery.GetOptionals().size() < currentQuery.GetOptionals().size()
            || newQuery.GetIncludeAny().size() < currentQuery.GetIncludeAny().size()
            || newQuery.GetImmediates().size() < currentQuery.GetImmediates().size()
            ))
        {
            return;
        }
    }

    desc.SingleComponentQuery = queryIndex;
}

void EntitySystemHelpersBase::UpdateQueryCache()
{
    if (queryCacheInitialized_) return;

    OPTICK_EVENT();
    auto world = GetEntityWorld();

    BitSet<> persistentQueries;
    BitSet<> aliveQueries;

    for (auto queryIndex : world->Queries.PersistentQueries) {
        persistentQueries.Set((uint32_t)queryIndex);
    }

    for (auto queryIndex : world->Queries.AliveQueries) {
        aliveQueries.Set((uint32_t)queryIndex);
    }

    unsigned queryIndex{ 0 };
    for (auto const& query : world->Queries.Queries) {

        if (query.Flags == QueryFlags::Added
            && aliveQueries[queryIndex]
            && query.GetIncludes().empty()
            && query.GetIncludeAny().empty()
            && query.GetExcludes().empty()
            // && query.GetAddOrs().empty()
            // && query.GetOptionalOneFrames().empty()
            // && query.GetImmediates().empty()
            && query.GetAddAnds().size() == 1
            && query.GetOneFrames().size() == 1) {

            MapSingleComponentQuery((QueryIndex)queryIndex, query.GetOneFrames()[0]);
        }
        
        // Only include "normal" single-component include queries
        if ((unsigned)query.Flags == 0
            && persistentQueries[queryIndex]
            && (
                (query.GetIncludes().size() == 1 && query.GetIncludeAny().empty())
                || (query.GetIncludes().empty() && query.GetIncludeAny().size() == 1)
                )
            && query.GetExcludes().empty()) {

            if (query.GetIncludes().empty()) {
                MapSingleComponentQuery((QueryIndex)queryIndex, query.GetIncludes()[0]);
            } else {
                MapSingleComponentQuery((QueryIndex)queryIndex, query.GetIncludeAny()[0]);
            }
        }

        queryIndex++;
    }

    queryCacheInitialized_ = true;
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
    auto entityMap = GetSingleton<UuidToHandleMappingComponent>();
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
    if (index == resource::UndefinedStaticDataType || (unsigned)type >= std::size(staticDataIndices_)) {
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

bool EntitySystemHelpersBase::SetSystemUpdateHook(SystemTypeIndex systemType, std::function<SystemHookProc> preUpdate, std::function<SystemHookProc> postUpdate)
{
    assert((unsigned)systemType < systemTypeIdToName_.size());

    if ((unsigned)systemType >= systemHooks_.size()) return false;

    auto& hook = systemHooks_[(unsigned)systemType];
    if (hook.OriginalUpdateProc == nullptr) {
        auto& system = GetEntityWorld()->Systems.Systems[(unsigned)systemType];
        if (system.System == nullptr) {
            return false;
        }

        systemToId_.insert(std::make_pair(system.System, systemType));
        hook.OriginalUpdateProc = system.UpdateProc;
        system.UpdateProc = &StaticSystemUpdateHook;
    }

    hook.PreUpdate = preUpdate;
    hook.PostUpdate = postUpdate;
    return true;
}

void EntitySystemHelpersBase::SystemUpdateHook(BaseSystem* system, EntityWorld& world, GameTime const& time)
{
    auto systemId = systemToId_.find(system);
    assert(systemId != systemToId_.end());
    auto& hooks = systemHooks_[(unsigned)systemId->second];
    if (hooks.PreUpdate) {
        hooks.PreUpdate(this, system, time, systemId->second);
    }

    hooks.OriginalUpdateProc(system, world, time);

    if (hooks.PostUpdate) {
        hooks.PostUpdate(this, system, time, systemId->second);
    }
}

void EntitySystemHelpersBase::StaticSystemUpdateHook(BaseSystem* system, EntityWorld& world, GameTime const& time)
{
    auto helpers = (&world == gExtender->GetClient().GetEntityHelpers().GetEntityWorld())
        ? (EntitySystemHelpersBase*)&gExtender->GetClient().GetEntityHelpers()
        : (EntitySystemHelpersBase*)&gExtender->GetServer().GetEntityHelpers();

    helpers->SystemUpdateHook(system, world, time);
}

void EntitySystemHelpersBase::InitSystemUpdateHooks()
{
    systemHooks_.resize(systemTypeIdToName_.size());
}

void EntitySystemHelpersBase::ClearSystemUpdateHooks()
{
    for (unsigned i = 0; i < systemHooks_.size(); i++) {
        if (systemHooks_[i].OriginalUpdateProc != nullptr) {
            auto& system = GetEntityWorld()->Systems.Systems[i];
            system.UpdateProc = systemHooks_[i].OriginalUpdateProc;
        }
    }

    systemToId_.clear();
    systemHooks_.clear();
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

ExtensionStateBase* ServerEntitySystemHelpers::GetExtensionState() const
{
    return gExtender->GetServer().HasExtensionState() ? &gExtender->GetServer().GetExtensionState() : nullptr;
}

std::optional<EntityHandle> ServerEntitySystemHelpers::NetIdToEntity(NetId netId) const
{
    auto& repl = GetStaticSymbols().GetEoCServer()->GameServer->Replication;
    auto entity = repl.NetIdToEntity.try_get(netId);
    return entity ? *entity : std::optional<EntityHandle>{};
}

std::optional<NetId> ServerEntitySystemHelpers::EntityToNetId(EntityHandle entity) const
{
    auto& repl = GetStaticSymbols().GetEoCServer()->GameServer->Replication;
    auto netId = repl.EntityToNetId.try_get(entity);
    return netId ? *netId : std::optional<NetId>{};
}

EntityWorld* ClientEntitySystemHelpers::GetEntityWorld() const
{
    return GetStaticSymbols().GetClientEntityWorld();
}

ExtensionStateBase* ClientEntitySystemHelpers::GetExtensionState() const
{
    return gExtender->GetClient().HasExtensionState() ? &gExtender->GetClient().GetExtensionState() : nullptr;
}

std::optional<EntityHandle> ClientEntitySystemHelpers::NetIdToEntity(NetId netId) const
{
    auto client = GetStaticSymbols().GetEoCClient()->GameClient;
    if (client) {
        auto entity = client->ReplicationPeer.NetIdToEntity.try_get(netId);
        return entity ? *entity : std::optional<EntityHandle>{};
    } else {
        return {};
    }
}

std::optional<NetId> ClientEntitySystemHelpers::EntityToNetId(EntityHandle entity) const
{
    auto client = GetStaticSymbols().GetEoCClient()->GameClient;
    if (client) {
        auto netId = client->ReplicationPeer.EntityToNetId.try_get(entity);
        return netId ? *netId : std::optional<NetId>{};
    } else {
        return {};
    }
}

END_NS()
