#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/EntitySystemHelpers.h>

BEGIN_NS(lua)

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

class EntityProxyMetatable : public LightCppObjectMetatable<EntityProxyMetatable>,
	public Indexable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::Entity;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, EntityHandle const& handle)
	{
		lua_push_cppobject(L, MetatableTag::Entity, 0, reinterpret_cast<void*>(handle.Handle), LifetimeHandle{});
	}

	static EntityHandle Get(lua_State* L, int index);
	static EntityHelper GetHelper(lua_State* L, int index);

	inline static EntityHandle GetHandle(CppObjectMetadata& self)
	{
		return EntityHandle(reinterpret_cast<uint64_t>(self.Ptr));
	}

	static int Index(lua_State* L, CppObjectMetadata& self);
	static int ToString(lua_State* L, CppObjectMetadata& self);
	static bool IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other);
	static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);

private:
	static int CreateComponent(lua_State* L);
	static int GetComponent(lua_State* L);
	static int GetAllComponents(lua_State* L);
	static int GetAllComponentNames(lua_State* L);
	static int GetEntityType(lua_State* L);
	static int GetSalt(lua_State* L);
	static int GetIndex(lua_State* L);
	static int IsAlive(lua_State* L);
	static int Replicate(lua_State* L);
	static int SetReplicationFlags(lua_State* L);
	static int GetReplicationFlags(lua_State* L);
};

END_NS()
