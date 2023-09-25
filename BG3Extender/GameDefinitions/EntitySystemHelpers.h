#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_NS(ecs)

enum class IndexSymbolType
{
	None,
	Replication,
	Handle,
	Component,
	EventComponent
};

struct IndexSymbolInfo
{
	char const* name;
	IndexSymbolType type;
};

class EntitySystemHelpersBase : public Noncopyable<EntitySystemHelpersBase>
{
public:
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

	inline std::optional<STDString const*> GetComponentName(HandleTypeIndex index) const
	{
		auto it = handleIndexToNameMappings_.find(index);
		if (it != handleIndexToNameMappings_.end()) {
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

	inline std::optional<ExtComponentType> GetComponentType(HandleTypeIndex index) const
	{
		auto it = handleIndexToTypeMappings_.find(index);
		if (it != handleIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ComponentTypeIndex> GetComponentIndex(HandleTypeIndex index) const
	{
		auto it = handleIndexToComponentMappings_.find(index);
		if (it != handleIndexToComponentMappings_.end()) {
			return ComponentTypeIndex(it->second);
		} else {
			return {};
		}
	}

	inline std::optional<ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const
	{
		auto idx = componentIndices_[(unsigned)type];
		if (idx != UndefinedIndex) {
			return idx;
		} else {
			return {};
		}
	}

	inline std::size_t GetComponentSize(ExtComponentType type) const
	{
		return componentSizes_[(unsigned)type];
	}

	std::optional<HandleTypeIndex> GetHandleIndex(ExtComponentType type) const
	{
		auto idx = handleIndices_[(unsigned)type];
		if (idx != -1) {
			return idx;
		}
		else {
			return {};
		}
	}

	template <class T>
	T* GetComponent(char const* nameGuid)
	{
		return reinterpret_cast<T*>(GetRawComponent(nameGuid, T::ComponentType));
	}

	template <class T>
	T* GetComponent(FixedString const& guid)
	{
		return reinterpret_cast<T*>(GetRawComponent(guid, T::ComponentType));
	}

	template <class T>
	T* GetComponent(EntityHandle entityHandle)
	{
		return reinterpret_cast<T*>(GetRawComponent(entityHandle, T::ComponentType));
	}

	template <class T>
	T* GetComponent(NetId netId)
	{
		ERR("FIXME");
		return nullptr;
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
	EntityHandle GetEntityHandle(Guid uuid);
	UuidToHandleMappingComponent* GetUuidMappings();

protected:
	static constexpr int32_t UndefinedIndex{ -1 };

	void MapComponentIndices(char const* componentName, ExtComponentType type, std::size_t size);
	void MapQueryIndex(char const* name, ExtQueryType type);
	void MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type);
	void UpdateComponentMappings();

private:
	struct ComponentIndexMappings
	{
		int32_t ReplicationIndex{ -1 };
		int32_t HandleIndex{ -1 };
		int32_t ComponentIndex{ -1 };
		int32_t EventComponentIndex{ -1 };
		std::array<int32_t, 5> Indices;
		std::size_t NumIndices{ 0 };

		void Add(int32_t index, IndexSymbolType type);
	};
	
	struct IndexMappings
	{
		HandleTypeIndex HandleIndex{ UndefinedHandle };
		ComponentTypeIndex ComponentIndex{ UndefinedComponent };
		ReplicationTypeIndex ReplicationIndex{ UndefinedReplicationComponent };
	};

	std::array<ComponentTypeIndex, (size_t)ExtComponentType::Max> componentIndices_;
	std::array<std::size_t, (size_t)ExtComponentType::Max> componentSizes_;
	std::array<ReplicationTypeIndex, (size_t)ExtComponentType::Max> replicationIndices_;
	std::array<HandleTypeIndex, (size_t)ExtComponentType::Max> handleIndices_;
	std::array<int32_t, (size_t)ExtQueryType::Max> queryIndices_;
	std::array<int32_t, (size_t)ExtResourceManagerType::Max> resourceManagerIndices_;

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	std::unordered_map<ComponentTypeIndex, STDString const*> componentIndexToNameMappings_;
	std::unordered_map<HandleTypeIndex, STDString const*> handleIndexToNameMappings_;
	std::unordered_map<ComponentTypeIndex, ExtComponentType> componentIndexToTypeMappings_;
	std::unordered_map<HandleTypeIndex, ExtComponentType> handleIndexToTypeMappings_;
	std::unordered_map<HandleTypeIndex, ComponentTypeIndex> handleIndexToComponentMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::vector<STDString const*> systemTypeIdToName_;
	std::unordered_map<STDString, int32_t> queryMappings_;
	std::vector<STDString const*> queryTypeIdToName_;

	bool initialized_{ false };

	bool TryUpdateSystemMapping(std::string_view name, ComponentIndexMappings& mapping);
	void TryUpdateComponentMapping(std::string_view name, ComponentIndexMappings& mapping);
	void BindSystemName(std::string_view name, int32_t systemId);
	void BindQueryName(std::string_view name, int32_t systemId);
	void* GetRawComponent(char const* nameGuid, ExtComponentType type);
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
