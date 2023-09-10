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
	T* RawComponentPtrToComponent(void* ptr)
	{
		/*if constexpr (std::is_same_v<T, esv::Character> || std::is_same_v<T, esv::Item>) {
			return reinterpret_cast<T*>((uintptr_t)ptr - 8);
		}*/

		return reinterpret_cast<T*>(ptr);
	}

	template <class T>
	T* GetComponent(char const* nameGuid)
	{
		auto component = GetRawComponent(nameGuid, T::ComponentType);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(FixedString const& guid)
	{
		auto component = GetRawComponent(guid, T::ComponentType);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(EntityHandle entityHandle)
	{
		auto component = GetRawEntityComponent(entityHandle, T::ComponentType);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(NetId netId)
	{
		ERR("FIXME");
		return nullptr;
	}

	template <class T, class Fun>
	void IterateComponents(Fun fun)
	{
		auto world = GetEntityWorld();
		if (!world) {
			return;
		}

		auto componentIndex = GetComponentIndex(T::ComponentType);
		if (!componentIndex) {
			return;
		}

		auto pool = world->Components.Types[(int)*componentIndex].Pool;
		auto size = pool->GetSize();
		for (auto i = 0; i < size; i++) {
			auto component = pool->Factory.FindByIndex(i);
			if (component) {
				fun(RawComponentPtrToComponent<T>(component));
			}
		}
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

	BitSet<> * GetReplicationFlags(EntityHandle const& entity, ComponentTypeIndex replicationType);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, ComponentTypeIndex replicationType);
	void NotifyReplicationFlagsDirtied();

protected:
	static constexpr int32_t UndefinedIndex{ -1 };

	void MapComponentIndices(char const* componentName, ExtComponentType type);
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
		HandleTypeIndex HandleIndex;
		ComponentTypeIndex ComponentIndex;
	};

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	std::unordered_map<ComponentTypeIndex, STDString const*> componentIndexToNameMappings_;
	std::unordered_map<HandleTypeIndex, STDString const*> handleIndexToNameMappings_;
	std::unordered_map<ComponentTypeIndex, ExtComponentType> componentIndexToTypeMappings_;
	std::unordered_map<HandleTypeIndex, ExtComponentType> handleIndexToTypeMappings_;
	std::unordered_map<HandleTypeIndex, ComponentTypeIndex> handleIndexToComponentMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::vector<STDString const*> systemIndices_;
	std::unordered_map<STDString, int32_t> queryMappings_;
	std::vector<STDString const*> queryIndices_;
	std::array<ComponentTypeIndex, (int)ExtComponentType::Max> componentIndices_;
	std::array<HandleTypeIndex, (int)ExtComponentType::Max> handleIndices_;
	std::array<int32_t, (int)ExtResourceManagerType::Max> resourceManagerIndices_;
	bool initialized_{ false };

	bool TryUpdateSystemMapping(std::string_view name, ComponentIndexMappings& mapping);
	void TryUpdateComponentMapping(std::string_view name, ComponentIndexMappings& mapping);
	void BindSystemName(std::string_view name, int32_t systemId);
	void BindQueryName(std::string_view name, int32_t systemId);
	void* GetRawComponent(char const* nameGuid, ExtComponentType type);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type);
	void* GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type);
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
