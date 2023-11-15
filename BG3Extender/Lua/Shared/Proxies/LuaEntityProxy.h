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
