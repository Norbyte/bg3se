#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/EntitySystemHelpers.h>

BEGIN_NS(lua)

void PushComponent(lua_State* L, ecs::EntitySystemHelpersBase* helpers, EntityHandle const& handle, 
	ExtComponentType componentType, LifetimeHandle const& lifetime);
void PushComponent(lua_State* L, void* rawComponent, ExtComponentType componentType, LifetimeHandle const& lifetime);

class EntityHelper
{
public:
	inline EntityHelper(EntityHandle const& handle, ecs::EntitySystemHelpersBase* ecs)
		: handle_(handle), ecs_(ecs)
	{}

	template <class T>
	bool HasComponent()
	{
		return ecs_->GetComponent<T>(handle_) != nullptr;
	}

	template <class T>
	T* GetComponent()
	{
		return ecs_->GetComponent<T>(handle_);
	}

	Array<ExtComponentType> GetAllComponentTypes() const;
	void PushComponentByType(lua_State* L, ExtComponentType componentType) const;

	inline EntityHandle const& Handle() const
	{
		return handle_;
	}

private:
	EntityHandle handle_;
	ecs::EntitySystemHelpersBase* ecs_;
};

class EntityProxyMetatable : public LightCppValueMetatable<EntityProxyMetatable>,
	public Indexable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::Entity;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, EntityHandle const& handle)
	{
		lua_push_cppvalue(L, MetatableTag::Entity, 0, handle.Handle);
	}

	static EntityHandle Get(lua_State* L, int index);
	static EntityHelper GetHelper(lua_State* L, int index);

	inline static EntityHandle GetHandle(CppValueMetadata& self)
	{
		return EntityHandle(self.Value);
	}

	static int Index(lua_State* L, CppValueMetadata& self);
	static int ToString(lua_State* L, CppValueMetadata& self);
	static bool IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex);
	static char const* GetTypeName(lua_State* L, CppValueMetadata& self);
	static void StaticInitialize();

private:
	static UserReturn CreateComponent(lua_State* L, EntityHandle entity, ExtComponentType component);
	static UserReturn GetComponent(lua_State* L, EntityHandle entity, ExtComponentType component);
	static bool HasRawComponent(lua_State* L, EntityHandle entity, STDString componentName);
	static UserReturn GetAllComponents(lua_State* L, EntityHandle entity, std::optional<bool> warnOnMissing);
	static Array<STDString> GetAllComponentNames(lua_State* L, EntityHandle entity, std::optional<bool> requireMapped);
	static uint32_t GetEntityType(EntityHandle entity);
	static uint32_t GetSalt(EntityHandle entity);
	static uint32_t GetIndex(EntityHandle entity);
	static bool IsAlive(lua_State* L, EntityHandle entity);
	static void Replicate(lua_State* L, EntityHandle entity, ExtComponentType component);
	static void SetReplicationFlags(lua_State* L, EntityHandle entity, ExtComponentType component, uint64_t flags, std::optional<uint32_t> qword);
	static uint64_t GetReplicationFlags(lua_State* L, EntityHandle entity, ExtComponentType component, std::optional<uint32_t> qword);

	static HashMap<FixedString, lua_CFunction> functions_;
};

END_NS()
