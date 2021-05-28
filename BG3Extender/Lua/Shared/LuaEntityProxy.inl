#include <Lua/Shared/LuaEntityProxy.h>

namespace bg3se::lua
{
	char const* const EntityProxy::MetatableName = "EntityProxy";

	EntityProxy::EntityProxy(EntityHandle const& handle, EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int EntityProxy::HasComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto componentType = checked_get<char const*>(L, 2);
		auto componentIdx = self->entitySystem_->GetComponentIndex(componentType);
		if (componentIdx) {
			auto world = self->entitySystem_->GetEntityWorld();
			auto component = world->GetEntityComponentHandle(self->handle_, *componentIdx, false);
			push(L, (bool)component);
		}
		else {
			push(L, false);
		}

		return 1;
	}

	int EntityProxy::GetEntityType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int EntityProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int EntityProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	int EntityProxy::IsAlive(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto world = self->entitySystem_->GetEntityWorld();
		auto entity = world->GetEntity(self->handle_, false);
		push(L, entity != nullptr);
		return 1;
	}

	void EntityProxy::PopulateMetatable(lua_State* L)
	{
		lua_newtable(L);

		push(L, &EntityProxy::HasComponent);
		lua_setfield(L, -2, "HasComponent");

		push(L, &EntityProxy::GetEntityType);
		lua_setfield(L, -2, "GetEntityType");

		push(L, &EntityProxy::GetSalt);
		lua_setfield(L, -2, "GetSalt");

		push(L, &EntityProxy::GetIndex);
		lua_setfield(L, -2, "GetIndex");

		push(L, &EntityProxy::IsAlive);
		lua_setfield(L, -2, "IsAlive");

		lua_setfield(L, -2, "__index");
	}

	int EntityProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);

		OsiError("Should not get here!");
		push(L, nullptr);
		return 1;
	}


	char const* const ObjectHandleProxy::MetatableName = "ObjectHandleProxy";

	ObjectHandleProxy::ObjectHandleProxy(ObjectHandle const& handle, EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int ObjectHandleProxy::GetType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int ObjectHandleProxy::GetTypeName(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		auto name = self->entitySystem_->GetComponentName(EntityWorldBase::HandleTypeIndex(self->handle_.GetType()));
		push(L, name);
		return 1;
	}

	int ObjectHandleProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int ObjectHandleProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	void ObjectHandleProxy::PopulateMetatable(lua_State* L)
	{
		lua_newtable(L);

		push(L, &ObjectHandleProxy::GetType);
		lua_setfield(L, -2, "GetType");

		push(L, &ObjectHandleProxy::GetTypeName);
		lua_setfield(L, -2, "GetTypeName");

		push(L, &ObjectHandleProxy::GetSalt);
		lua_setfield(L, -2, "GetSalt");

		push(L, &ObjectHandleProxy::GetIndex);
		lua_setfield(L, -2, "GetIndex");

		lua_setfield(L, -2, "__index");
	}

	int ObjectHandleProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);

		OsiError("Should not get here!");
		push(L, nullptr);
		return 1;
	}


	void RegisterEntityProxy(lua_State* L)
	{
		EntityProxy::RegisterMetatable(L);
		ObjectHandleProxy::RegisterMetatable(L);
	}
}
