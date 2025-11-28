#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_NS(ecs)

enum class TypeIdContext
{
    Replication,
    Component,
    OneFrameComponent,
    System,
    ImmutableData
};

struct IndexSymbolInfo
{
    char const* name;
    int32_t* context;
};

enum class RuntimeCheckLevel
{
    // No property map validation
    None,
    // Validate each property map on the first mapped object
    Once,
    // Validate each mapped object separately
    Always,
    // Validate changed ECS components even if no Lua mapping is done
    FullECS
};

struct PerECSComponentData
{
    STDString const* Name{ nullptr };
    std::optional<ExtComponentType> ExtType;
    ReplicationTypeIndex ReplicationType{ UndefinedReplicationComponent };
};

struct PerECSReplicationData
{
    STDString const* Name{ nullptr };
    std::optional<ExtComponentType> ExtType;
    ComponentTypeIndex ComponentType{ UndefinedComponent };
};

class ECSComponentDataMap
{
public:
    ECSComponentDataMap();

    PerECSComponentData const& Get(ComponentTypeIndex type) const;
    PerECSComponentData& GetOrAdd(ComponentTypeIndex type);
    
    PerECSReplicationData const& Get(ReplicationTypeIndex type) const;
    PerECSReplicationData& GetOrAdd(ReplicationTypeIndex type);

    void Clear();

private:
    std::vector<PerECSComponentData> componentData_;
    std::vector<PerECSReplicationData> replicationData_;
    PerECSComponentData nullComponentData_;
    PerECSReplicationData nullReplicationData_;
};

struct ECSComponentLog
{
    ComponentTypeIndex ComponentType{ 0 };
    ComponentChangeFlags Flags{ 0 };

    StringView GetName() const;
    std::optional<ExtComponentType> GetType() const;
};

struct ECSEntityLog
{
    // Keep the key untyped on purpose to allow Lua serialization
    HashMap<uint16_t, ECSComponentLog> Components;
    EntityHandle Entity;
    EntityChangeFlags Flags{ 0 };
};

struct ECSChangeLog
{
    HashMap<EntityHandle, ECSEntityLog> Entities;

    void Clear();
    void AddEntityChange(EntityHandle entity, EntityChangeFlags flags);
    void AddComponentChange(EntityWorld* world, EntityHandle entity, ComponentTypeIndex type, ComponentChangeFlags flags);
};

class EntitySystemHelpersBase : public Noncopyable<EntitySystemHelpersBase>
{
public:
    using SystemHookProc = void(EntitySystemHelpersBase*, BaseSystem*, GameTime const&, SystemTypeIndex system);

    static RuntimeCheckLevel CheckLevel;

    struct PerComponentData
    {
        ComponentTypeIndex ComponentIndex{ UndefinedComponent };
        ReplicationTypeIndex ReplicationIndex{ UndefinedReplicationComponent };
        // ID of ECS query that only returns this single component;
        // this is used to avoid brute-forcing the ECS when looking for all entities with a particular component
        QueryIndex SingleComponentQuery{ UndefinedQuery };
        uint16_t Size{ 0 };
        lua::GenericPropertyMap* Properties{ nullptr };
        bool IsProxy{ false };
        bool OneFrame{ false };
    };

    EntitySystemHelpersBase();

    inline std::optional<ComponentTypeIndex> GetComponentIndex(STDString const& type) const
    {
        auto it = componentNameToIndexMappings_.find(type);
        if (it != componentNameToIndexMappings_.end() && it->second.ComponentIndex != UndefinedComponent) {
            return it->second.ComponentIndex;
        } else {
            return {};
        }
    }

    inline STDString const* GetComponentName(ComponentTypeIndex index) const
    {
        return ecsComponentData_.Get(index).Name;
    }

    inline STDString const* GetComponentName(ReplicationTypeIndex index) const
    {
        return ecsComponentData_.Get(index).Name;
    }

    inline std::optional<ExtComponentType> GetComponentType(ComponentTypeIndex index) const
    {
        return ecsComponentData_.Get(index).ExtType;
    }

    inline std::optional<ExtComponentType> GetComponentType(ReplicationTypeIndex index) const
    {
        return ecsComponentData_.Get(index).ExtType;
    }

    std::optional<ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const;

    inline std::optional<ComponentTypeIndex> GetComponentIndexUnchecked(ExtComponentType type) const
    {
        auto idx = components_[(unsigned)type].ComponentIndex;
        if (idx != UndefinedComponent) {
            return idx;
        } else {
            return {};
        }
    }

    inline PerComponentData const* GetComponentMeta(ComponentTypeIndex type) const
    {
        auto extType = ecsComponentData_.Get(type).ExtType;
        return extType ? &components_[(unsigned)*extType] : nullptr;
    }

    inline PerComponentData const& GetComponentMeta(ExtComponentType type) const
    {
        return components_[(unsigned)type];
    }

    inline std::size_t GetComponentSize(ExtComponentType type) const
    {
        return components_[(unsigned)type].Size;
    }

    std::optional<ReplicationTypeIndex> GetReplicationIndex(ExtComponentType type) const
    {
        auto idx = components_[(unsigned)type].ReplicationIndex;
        if (idx != UndefinedReplicationComponent) {
            return idx;
        } else {
            return {};
        }
    }

    lua::GenericPropertyMap* GetPropertyMap(ExtComponentType type) const
    {
        return components_[(unsigned)type].Properties;
    }

    void BindPropertyMap(ExtComponentType type, lua::GenericPropertyMap* pm)
    {
        se_assert(components_[(unsigned)type].Properties == nullptr);
        components_[(unsigned)type].Properties = pm;
    }

    template <class T>
    T* GetComponent(FixedString const& guid)
    {
        return reinterpret_cast<T*>(GetRawComponent(guid, T::ComponentType));
    }

    template <class T>
    T* GetComponent(Guid const& guid)
    {
        return reinterpret_cast<T*>(GetRawComponent(guid, T::ComponentType));
    }

    template <class T>
    T* GetComponent(EntityHandle entityHandle)
    {
        return reinterpret_cast<T*>(GetRawComponent(entityHandle, T::ComponentType));
    }

    template <class T>
    ComponentOps* GetComponentOps()
    {
        auto const& meta = GetComponentMeta(T::ComponentType);
        if (meta.ComponentIndex != UndefinedComponent) {
            return GetEntityWorld()->ComponentOps.Get(meta.ComponentIndex);
        } else {
            return nullptr;
        }
    }

    template <class T>
    T* GetSystem()
    {
        return reinterpret_cast<T*>(GetRawSystem(T::SystemType));
    }

    virtual EntityWorld* GetEntityWorld() const = 0;
    virtual ExtensionStateBase* GetExtensionState() const = 0;

    virtual std::optional<EntityHandle> NetIdToEntity(NetId netId) const = 0;
    virtual std::optional<NetId> EntityToNetId(EntityHandle entity) const = 0;

    resource::GuidResourceBankBase* GetRawResourceManager(ExtResourceManagerType type);

    template <class T>
    std::optional<resource::GuidResourceBank<T>*> GetResourceManager()
    {
        auto mgr = GetRawResourceManager(T::ResourceManagerType);
        if (mgr) {
            return static_cast<resource::GuidResourceBank<T>*>(mgr);
        } else {
            return {};
        }
    }

    inline void EnableLogging(bool enable)
    {
        logging_ = enable;
    }

    inline ECSChangeLog& GetLog()
    {
        return log_;
    }

    BitSet<> * GetReplicationFlags(EntityHandle const& entity, ExtComponentType type);
    BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ExtComponentType type);
    BitSet<> * GetReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
    BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
    void NotifyReplicationFlagsDirtied();

    void* GetRawComponent(EntityHandle entityHandle, ExtComponentType type);
    void* GetRawSingleton(ExtComponentType type);
    EntityHandle GetSingletonEntity(ExtComponentType type);
    ecs::SystemTypeEntry* GetSystemEntry(ExtSystemType type);
    void* GetRawSystem(ExtSystemType type);
    EntityHandle GetEntityHandle(FixedString const& guidString);
    EntityHandle GetEntityHandle(Guid const& uuid);
    
    template <class T>
    inline T* GetSingleton()
    {
        auto p = GetRawSingleton(T::ComponentType);
        if (p != nullptr) {
            return reinterpret_cast<T*>(p);
        } else {
            return nullptr;
        }
    }
    
    template <class T>
    inline EntityHandle GetSingletonEntity()
    {
        return GetSingletonEntity(T::ComponentType);
    }

    void Update();
    void PostUpdate();
    void OnFlushECBs();
    void OnInit();
    void OnDestroy();

    bool SetSystemUpdateHook(SystemTypeIndex system, std::function<SystemHookProc> preUpdate, std::function<SystemHookProc> postUpdate);

protected:
    void MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size, bool isProxy);
    void MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type);
    void MapSystemIndex(char const* systemName, ExtSystemType type);
    void UpdateComponentMappings();
    void ValidateReplication();
    void ValidateMappedComponentSizes();
    bool ValidateMappedComponentSize(ecs::EntityWorld* world, ComponentTypeIndex typeId, ExtComponentType extType);
    void ValidateECBFlushChanges();
    void ValidateEntityChanges();
    void ValidateEntityChanges(ImmediateWorldCache::Changes& changes);
    void UpdateQueryCache();
    void MapSingleComponentQuery(QueryIndex query, ComponentTypeIndex component);

private:
    struct IndexMappings
    {
        ComponentTypeIndex ComponentIndex{ UndefinedComponent };
        ReplicationTypeIndex ReplicationIndex{ UndefinedReplicationComponent };
    };
    
    struct SystemHook
    {
        SystemTypeEntry::UpdateProcType* OriginalUpdateProc{ nullptr };
        std::function<SystemHookProc> PreUpdate;
        std::function<SystemHookProc> PostUpdate;
    };

    std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
    ECSComponentDataMap ecsComponentData_;
    std::unordered_map<STDString, SystemTypeIndex> systemIndexMappings_;
    std::vector<STDString const*> systemTypeIdToName_;
    std::unordered_map<STDString, resource::StaticDataTypeIndex> staticDataMappings_;
    std::vector<STDString const*> staticDataIdToName_;

    bool initialized_{ false };
    bool queryCacheInitialized_{ false };
    bool validated_{ false };
    bool logging_{ false };

    std::array<PerComponentData, (size_t)ExtComponentType::Max> components_;
    std::array<resource::StaticDataTypeIndex, (size_t)ExtResourceManagerType::Max> staticDataIndices_;
    std::array<SystemTypeIndex, (size_t)ExtSystemType::Max> systemIndices_;

    std::unordered_map<BaseSystem*, SystemTypeIndex> systemToId_;
    std::vector<SystemHook> systemHooks_;

    ECSChangeLog log_;

    void BindSystem(std::string_view name, SystemTypeIndex id);
    void BindStaticData(std::string_view name, resource::StaticDataTypeIndex id);
    void BindComponent(std::string_view name, ComponentTypeIndex id);
    void BindReplication(std::string_view name, ReplicationTypeIndex id);
    void* GetRawComponent(Guid const& guid, ExtComponentType type);
    void* GetRawComponent(FixedString const& guid, ExtComponentType type);

    void DebugLogUpdateChanges();
    void DebugLogReplicationChanges();
    void DebugLogECBFlushChanges();
    void ThrowECBFlushEvents();

    void InitSystemUpdateHooks();
    void ClearSystemUpdateHooks();

    void SystemUpdateHook(BaseSystem*, EntityWorld&, GameTime const&);
    static void StaticSystemUpdateHook(BaseSystem*, EntityWorld&, GameTime const&);
};

class ServerEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
    void Setup();

    EntityWorld* GetEntityWorld() const override;
    ExtensionStateBase* GetExtensionState() const override;
    std::optional<EntityHandle> NetIdToEntity(NetId netId) const override;
    std::optional<NetId> EntityToNetId(EntityHandle entity) const override;
};

class ClientEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
    void Setup();

    EntityWorld* GetEntityWorld() const override;
    ExtensionStateBase* GetExtensionState() const override;
    std::optional<EntityHandle> NetIdToEntity(NetId netId) const override;
    std::optional<NetId> EntityToNetId(EntityHandle entity) const override;
};

END_NS()
