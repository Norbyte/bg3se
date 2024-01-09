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

class EntitySystemHelpersBase : public Noncopyable<EntitySystemHelpersBase>
{
public:
	static RuntimeCheckLevel CheckLevel;

	struct PerComponentData
	{
		ComponentTypeIndex ComponentIndex{ UndefinedComponent };
		ReplicationTypeIndex ReplicationIndex{ UndefinedReplicationComponent };
		std::size_t Size{ 0 };
		lua::GenericPropertyMap* Properties{ nullptr };
		bool IsProxy{ false };
	};

	EntitySystemHelpersBase();

	inline std::optional<ComponentTypeIndex> GetComponentIndex(STDString const& type) const
	{
		auto it = componentNameToIndexMappings_.find(type);
		if (it != componentNameToIndexMappings_.end() && it->second.ComponentIndex != UndefinedIndex) {
			return it->second.ComponentIndex;
		} else {
			return {};
		}
	}

	inline std::optional<STDString const*> GetComponentName(ComponentTypeIndex index) const
	{
		auto it = componentIndexToNameMappings_.find(index);
		if (it != componentIndexToNameMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(ComponentTypeIndex index) const
	{
		auto it = componentIndexToTypeMappings_.find(index);
		if (it != componentIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(ReplicationTypeIndex index) const
	{
		auto it = replicationIndexToTypeMappings_.find(index);
		if (it != replicationIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const
	{
		auto idx = components_[(unsigned)type].ComponentIndex;
		if (idx != UndefinedIndex) {
			return idx;
		} else {
			return {};
		}
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
		if (idx != -1) {
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

	virtual EntityWorld* GetEntityWorld() = 0;

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

	inline std::optional<int32_t> GetQueryIndex(STDString const& type) const
	{
		auto it = queryMappings_.find(type);
		if (it != queryMappings_.end() && it->second != UndefinedIndex) {
			return it->second;
		} else {
			return {};
		}
	}

	Query* GetQuery(ExtQueryType type);

	BitSet<> * GetReplicationFlags(EntityHandle const& entity, ExtComponentType type);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ExtComponentType type);
	BitSet<> * GetReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ReplicationTypeIndex replicationType);
	void NotifyReplicationFlagsDirtied();

	void* GetRawComponent(EntityHandle entityHandle, ExtComponentType type);
	EntityHandle GetEntityHandle(FixedString const& guidString);
	EntityHandle GetEntityHandle(Guid const& uuid);
	UuidToHandleMappingComponent* GetUuidMappings();

	void Update();
	void PostUpdate();

protected:
	static constexpr int32_t UndefinedIndex{ -1 };

	void MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size, bool isProxy);
	void MapQueryIndex(char const* name, ExtQueryType type);
	void MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type);
	void UpdateComponentMappings();

private:
	struct IndexMappings
	{
		ComponentTypeIndex ComponentIndex{ UndefinedComponent };
		ReplicationTypeIndex ReplicationIndex{ UndefinedReplicationComponent };
	};

	std::array<PerComponentData, (size_t)ExtComponentType::Max> components_;
	std::array<int32_t, (size_t)ExtQueryType::Max> queryIndices_;
	std::array<int32_t, (size_t)ExtResourceManagerType::Max> staticDataIndices_;

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	std::unordered_map<ComponentTypeIndex, STDString const*> componentIndexToNameMappings_;
	std::unordered_map<ComponentTypeIndex, ExtComponentType> componentIndexToTypeMappings_;
	std::unordered_map<ReplicationTypeIndex, ExtComponentType> replicationIndexToTypeMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::vector<STDString const*> systemTypeIdToName_;
	std::unordered_map<STDString, int32_t> queryMappings_;
	std::vector<STDString const*> queryTypeIdToName_;
	std::unordered_map<STDString, int32_t> staticDataMappings_;
	std::vector<STDString const*> staticDataIdToName_;

	bool initialized_{ false };

	void BindSystem(std::string_view name, int32_t id);
	void BindQuery(std::string_view name, int32_t id);
	void BindStaticData(std::string_view name, int32_t id);
	void BindComponent(std::string_view name, int32_t id);
	void BindReplication(std::string_view name, int32_t id);
	void* GetRawComponent(Guid const& guid, ExtComponentType type);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type);
	resource::GuidResourceBankBase* GetRawResourceManager(ExtResourceManagerType type);
};

class ServerEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorld* GetEntityWorld() override;
};

class ClientEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorld* GetEntityWorld() override;
};

END_NS()
