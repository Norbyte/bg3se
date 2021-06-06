#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>

BEGIN_SE()

class EntitySystemHelpersBase : public Noncopyable<EntitySystemHelpersBase>
{
public:
	EntitySystemHelpersBase();

	inline std::optional<EntityWorldBase::ComponentTypeIndex> GetComponentIndex(STDString const& type) const
	{
		auto it = componentNameToIndexMappings_.find(type);
		if (it != componentNameToIndexMappings_.end() && it->second.ComponentIndex != UndefinedIndex) {
			return it->second.ComponentIndex;
		} else {
			return {};
		}
	}

	inline std::optional<STDString> GetComponentName(EntityWorldBase::ComponentTypeIndex index) const
	{
		auto it = componentIndexToNameMappings_.find((int32_t)index);
		if (it != componentIndexToNameMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<STDString> GetComponentName(EntityWorldBase::HandleTypeIndex index) const
	{
		auto it = handleIndexToNameMappings_.find((int32_t)index);
		if (it != handleIndexToNameMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(EntityWorldBase::ComponentTypeIndex index) const
	{
		auto it = componentIndexToTypeMappings_.find((int32_t)index);
		if (it != componentIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<ExtComponentType> GetComponentType(EntityWorldBase::HandleTypeIndex index) const
	{
		auto it = handleIndexToTypeMappings_.find((int32_t)index);
		if (it != handleIndexToTypeMappings_.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	inline std::optional<EntityWorldBase::ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const
	{
		auto idx = componentIndices_[(unsigned)type];
		if (idx != UndefinedIndex) {
			return idx;
		} else {
			return {};
		}
	}

	std::optional<EntityWorldBase::HandleTypeIndex> GetHandleIndex(ExtComponentType type) const
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
		if constexpr (std::is_same_v<T, esv::Character> || std::is_same_v<T, esv::Item>) {
			return reinterpret_cast<T*>((uintptr_t)ptr - 8);
		}

		return reinterpret_cast<T*>(ptr);
	}

	template <class T>
	T* GetComponent(ObjectHandle componentHandle, bool logError = true)
	{
		auto component = GetRawComponent(componentHandle, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(char const* nameGuid, bool logError = true)
	{
		auto component = GetRawComponent(nameGuid, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(FixedString const& guid, bool logError = true)
	{
		auto component = GetRawComponent(guid, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(NetId netId, bool logError = true)
	{
		auto component = GetRawComponent(netId, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetEntityComponent(EntityHandle entityHandle, bool logError = true)
	{
		auto component = GetRawEntityComponent(entityHandle, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
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
			auto component = pool->GetComponentByIndex(i);
			if (component) {
				fun(RawComponentPtrToComponent<T>(component));
			}
		}
	}

	virtual EntityWorldBase* GetEntityWorld() = 0;

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

protected:
	static constexpr int32_t UndefinedIndex{ -1 };

	void MapComponentIndices(char const* componentName, ExtComponentType type);
	void MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type);
	void UpdateComponentMappings();

private:
	struct ComponentIndexMappings
	{
		std::array<int32_t, 4> Indices;
		std::size_t NumIndices{ 0 };

		void Add(int32_t index);
	};
	
	struct IndexMappings
	{
		EntityWorldBase::HandleTypeIndex HandleIndex;
		EntityWorldBase::ComponentTypeIndex ComponentIndex;
	};

	std::unordered_map<STDString, IndexMappings> componentNameToIndexMappings_;
	std::unordered_map<int32_t, STDString> componentIndexToNameMappings_;
	std::unordered_map<int32_t, STDString> handleIndexToNameMappings_;
	std::unordered_map<int32_t, ExtComponentType> componentIndexToTypeMappings_;
	std::unordered_map<int32_t, ExtComponentType> handleIndexToTypeMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::array<EntityWorldBase::ComponentTypeIndex, (int)ExtComponentType::Max> componentIndices_;
	std::array<EntityWorldBase::HandleTypeIndex, (int)ExtComponentType::Max> handleIndices_;
	std::array<int32_t, (int)ExtResourceManagerType::Max> resourceManagerIndices_;
	bool initialized_{ false };

	void* GetRawComponent(NetId netId, ExtComponentType type, bool logError);
	void* GetRawComponent(ObjectHandle componentHandle, ExtComponentType type, bool logError);
	void* GetRawComponent(char const* nameGuid, ExtComponentType type, bool logError);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type, bool logError);
	void* GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type, bool logError);
	GuidResourceDefinitionManagerBase* GetRawResourceManager(ExtResourceManagerType type);
};

class ServerEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorldBase* GetEntityWorld() override;
};

class ClientEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorldBase* GetEntityWorld() override;
};

END_SE()
