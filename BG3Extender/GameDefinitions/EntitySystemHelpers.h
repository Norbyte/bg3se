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
	ComponentTypeIndex ComponentType;
	ComponentChangeFlags Flags{ 0 };

	StringView GetName() const;
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
		assert(components_[(unsigned)type].Properties == nullptr);
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
	T* GetSystem()
	{
		return reinterpret_cast<T*>(GetRawSystem(T::SystemType));
	}

	virtual EntityWorld* GetEntityWorld() const = 0;
	virtual ExtensionStateBase* GetExtensionState() const = 0;

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

	inline std::optional<QueryIndex> GetQueryIndex(STDString const& type) const
	{
		auto it = queryMappings_.find(type);
		if (it != queryMappings_.end() && it->second != UndefinedQuery) {
			return it->second;
		} else {
			return {};
		}
	}

	QueryDescription* GetQuery(ExtQueryType type);

	BitSet<> * GetReplicationFlags(EntityHandle const& entity, ExtComponentType type);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ExtComponentType type);
	BitSet<> * GetReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
	void NotifyReplicationFlagsDirtied();

	void* GetRawComponent(EntityHandle entityHandle, ExtComponentType type);
	void* GetRawSystem(ExtSystemType type);
	EntityHandle GetEntityHandle(FixedString const& guidString);
	EntityHandle GetEntityHandle(Guid const& uuid);
	UuidToHandleMappingComponent* GetUuidMappings();

	void Update();
	void PostUpdate();
	void OnFlushECBs();

protected:
	void MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size, bool isProxy);
	void MapQueryIndex(char const* name, ExtQueryType type);
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

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	ECSComponentDataMap ecsComponentData_;
	std::unordered_map<STDString, SystemTypeIndex> systemIndexMappings_;
	std::vector<STDString const*> systemTypeIdToName_;
	std::unordered_map<STDString, QueryIndex> queryMappings_;
	std::vector<STDString const*> queryTypeIdToName_;
	std::unordered_map<STDString, resource::StaticDataTypeIndex> staticDataMappings_;
	std::vector<STDString const*> staticDataIdToName_;

	bool initialized_{ false };
	bool queryCacheInitialized_{ false };
	bool validated_{ false };
	bool logging_{ false };

	std::array<PerComponentData, (size_t)ExtComponentType::Max> components_;
	std::array<QueryIndex, (size_t)ExtQueryType::Max> queryIndices_;
	std::array<resource::StaticDataTypeIndex, (size_t)ExtResourceManagerType::Max> staticDataIndices_;
	std::array<SystemTypeIndex, (size_t)ExtSystemType::Max> systemIndices_;

	ECSChangeLog log_;

	void BindSystem(std::string_view name, SystemTypeIndex id);
	void BindQuery(std::string_view name, QueryIndex id);
	void BindStaticData(std::string_view name, resource::StaticDataTypeIndex id);
	void BindComponent(std::string_view name, ComponentTypeIndex id);
	void BindReplication(std::string_view name, ReplicationTypeIndex id);
	void* GetRawComponent(Guid const& guid, ExtComponentType type);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type);
	resource::GuidResourceBankBase* GetRawResourceManager(ExtResourceManagerType type);

	void DebugLogUpdateChanges();
	void DebugLogReplicationChanges();
	void DebugLogECBFlushChanges();
	void ThrowECBFlushEvents();
};

class ServerEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorld* GetEntityWorld() const override;
	ExtensionStateBase* GetExtensionState() const override;
};

class ClientEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorld* GetEntityWorld() const override;
	ExtensionStateBase* GetExtensionState() const override;
};

END_NS()
