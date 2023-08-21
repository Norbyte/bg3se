#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_SE()

enum class IndexSymbolType
{
	None,
	Replication,
	Handle,
	Component
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

	inline std::optional<EntityWorld::ComponentTypeIndex> GetComponentIndex(STDString const& type) const
	{
		auto it = componentNameToIndexMappings_.find(type);
		if (it != componentNameToIndexMappings_.end() && it->second.ComponentIndex != UndefinedIndex) {
			return it->second.ComponentIndex;
		} else {
			return {};
		}
	}

	inline std::optional<STDString> GetComponentName(EntityWorld::ComponentTypeIndex index) const
	{
		auto it = componentIndexToNameMappings_.find((int16_t)index);
		if (it != componentIndexToNameMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<STDString> GetComponentName(EntityWorld::HandleTypeIndex index) const
	{
		auto it = handleIndexToNameMappings_.find((int16_t)index);
		if (it != handleIndexToNameMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(EntityWorld::ComponentTypeIndex index) const
	{
		auto it = componentIndexToTypeMappings_.find((int16_t)index);
		if (it != componentIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(EntityWorld::HandleTypeIndex index) const
	{
		auto it = handleIndexToTypeMappings_.find((int16_t)index);
		if (it != handleIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<EntityWorld::ComponentTypeIndex> GetComponentIndex(EntityWorld::HandleTypeIndex index) const
	{
		auto it = handleIndexToComponentMappings_.find((int16_t)index);
		if (it != handleIndexToComponentMappings_.end()) {
			return EntityWorld::ComponentTypeIndex(it->second);
		} else {
			return {};
		}
	}

	inline std::optional<EntityWorld::ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const
	{
		auto idx = componentIndices_[(unsigned)type];
		if (idx != UndefinedIndex) {
			return idx;
		} else {
			return {};
		}
	}

	std::optional<EntityWorld::HandleTypeIndex> GetHandleIndex(ExtComponentType type) const
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
	std::optional<GuidResourceDefinitionManager<T>*> GetResourceManager()
	{
		auto mgr = GetRawResourceManager(T::ResourceManagerType);
		if (mgr) {
			return static_cast<GuidResourceDefinitionManager<T>*>(mgr);
		} else {
			return {};
		}
	}

	BitSet<> * GetReplicationFlags(EntityHandle const& entity, EntityWorld::ComponentTypeIndex replicationType);
	BitSet<> * GetOrCreateReplicationFlags(EntityHandle const& entity, EntityWorld::ComponentTypeIndex replicationType);
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
		std::array<int32_t, 4> Indices;
		std::size_t NumIndices{ 0 };

		void Add(int32_t index, IndexSymbolType type);
	};
	
	struct IndexMappings
	{
		EntityWorld::HandleTypeIndex HandleIndex;
		EntityWorld::ComponentTypeIndex ComponentIndex;
	};

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	std::unordered_map<int16_t, STDString> componentIndexToNameMappings_;
	std::unordered_map<int16_t, STDString> handleIndexToNameMappings_;
	std::unordered_map<int16_t, ExtComponentType> componentIndexToTypeMappings_;
	std::unordered_map<int16_t, ExtComponentType> handleIndexToTypeMappings_;
	std::unordered_map<int16_t, int16_t> handleIndexToComponentMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::array<EntityWorld::ComponentTypeIndex, (int)ExtComponentType::Max> componentIndices_;
	std::array<EntityWorld::HandleTypeIndex, (int)ExtComponentType::Max> handleIndices_;
	std::array<int32_t, (int)ExtResourceManagerType::Max> resourceManagerIndices_;
	bool initialized_{ false };

	void UpdateComponentMapping(char const* name, ComponentIndexMappings& mapping);
	void* GetRawComponent(char const* nameGuid, ExtComponentType type);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type);
	void* GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type);
	GuidResourceDefinitionManagerBase* GetRawResourceManager(ExtResourceManagerType type);
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

END_SE()
