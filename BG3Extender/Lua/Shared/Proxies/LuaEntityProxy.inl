#include <Lua/Shared/Proxies/LuaEntityProxy.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/All.h>
#include <Extender/ScriptExtender.h>

namespace bg3se::lua
{
	char const* const EntityProxy::MetatableName = "EntityProxy";


	ecs::EntitySystemHelpersBase* EntityProxy::GetEntitySystem(lua_State* L)
	{
		return State::FromLua(L)->GetEntitySystemHelpers();
	}

	EntityProxy::EntityProxy(EntityHandle const& handle)
		: handle_(handle)
	{}

	template <class T>
	void PushComponentType(lua_State* L, ecs::EntitySystemHelpersBase* helpers, EntityHandle const& handle,
		LifetimeHandle const& lifetime)
	{
		auto component = helpers->GetComponent<T>(handle);
		if (component) {
			MakeDirectObjectRef(L, component, lifetime);
		} else {
			push(L, nullptr);
		}
	}

#define T(cls) \
	case cls::ComponentType: \
	{ \
		PushComponentType<cls>(L, helpers, handle, lifetime); \
		break; \
	}

	void PushComponent(lua_State* L, ecs::EntitySystemHelpersBase* helpers, EntityHandle const& handle, ExtComponentType componentType,
		LifetimeHandle const& lifetime)
	{
		switch (componentType) {

#include <GameDefinitions/Components/AllComponentTypes.inl>

		default:
			OsiError("Don't know how to push component type: " << componentType);
			break;
		}
	}

#undef T

#define T(cls) case cls::ComponentType: MakeDirectObjectRef(L, reinterpret_cast<cls*>(rawComponent), lifetime); break;

	void PushComponent(lua_State* L, void* rawComponent, ExtComponentType componentType, LifetimeHandle const& lifetime)
	{
		switch (componentType) {

#include <GameDefinitions/Components/AllComponentTypes.inl>

		default:
			OsiError("Don't know how to push component type: " << componentType);
			break;
		}
	}

#undef T

	int EntityProxy::GetComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		auto componentType = get<ExtComponentType>(L, 2);
		auto ecs = GetEntitySystem(L);
		PushComponent(L, ecs, self->handle_, componentType, GetCurrentLifetime(L));
		return 1;
	}

	void EntityProxy::GetComponentByType(lua_State* L, ExtComponentType componentType) const
	{
		StackCheck _(L, 1);
		auto ecs = GetEntitySystem(L);
		PushComponent(L, ecs, handle_, componentType, GetCurrentLifetime(L));
	}

	int EntityProxy::GetAllComponents(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);

		bool warnOnMissing = false;
		if (lua_gettop(L) >= 2) {
			warnOnMissing = get<bool>(L, 2);
		}

		lua_newtable(L);

		auto ecs = GetEntitySystem(L);
		auto world = ecs->GetEntityWorld();
		auto entityClass = world->GetEntityClass(self->handle_);
		if (entityClass != nullptr) {
			auto componentPtr = entityClass->InstanceToPageMap.Find(self->handle_);
			if (componentPtr) {
				for (auto typeInfo : entityClass->ComponentTypeToIndex) {
					auto extType = ecs->GetComponentType(typeInfo.Key());
					if (extType) {
						auto componentSize = ecs->GetComponentSize(*extType);
						auto component = entityClass->GetComponent(**componentPtr, typeInfo.Value(), componentSize);

						push(L, *extType);
						PushComponent(L, component, *extType, GetCurrentLifetime(L));
						lua_rawset(L, -3);
					} else if (warnOnMissing) {
						auto name = ecs->GetComponentName(typeInfo.Key());
						if (name) {
							OsiWarn("No model found for component: " << **name);
						} else {
							OsiWarn("No model found for component ID: " << typeInfo.Key().Value());
						}
					}
				}
			}
		}

		return 1;
	}

	Array<ExtComponentType> EntityProxy::GetAllComponentTypes(ecs::EntitySystemHelpersBase* ecs) const
	{
		Array<ExtComponentType> types;

		auto world = ecs->GetEntityWorld();
		auto entityClass = world->GetEntityClass(handle_);
		if (entityClass != nullptr) {
			for (auto typeInfo : entityClass->ComponentTypeToIndex) {
				auto extType = ecs->GetComponentType(typeInfo.Key());
				if (extType) {
					types.push_back(*extType);
				}
			}
		}

		return types;
	}

	int EntityProxy::GetAllComponentNames(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);

		lua_newtable(L);

		auto ecs = GetEntitySystem(L);
		auto world = ecs->GetEntityWorld();
		auto entityClass = world->GetEntityClass(self->handle_);
		if (entityClass != nullptr) {
			for (auto componentIdx : entityClass->ComponentTypeToIndex.Keys) {
				auto name = ecs->GetComponentName(componentIdx);
				if (name) {
					push(L, componentIdx.Value());
					push(L, **name);
					lua_rawset(L, -3);
				}
			}
		}

		return 1;
	}

	int EntityProxy::GetEntityType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int EntityProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int EntityProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	int EntityProxy::IsAlive(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		auto world = GetEntitySystem(L)->GetEntityWorld();
		bool alive{ false };
		auto entityClass = world->GetEntityClass(self->handle_);
		if (entityClass != nullptr) {
			alive = (bool)entityClass->InstanceToPageMap.Find(self->handle_);
		}

		push(L, alive);
		return 1;
	}


	int EntityProxy::GetReplicationFlags(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		auto component = get<ExtComponentType>(L, 2);

		uint32_t qword{ 0 };
		if (lua_gettop(L) > 2) {
			qword = get<uint32_t>(L, 3);
		}

		auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
		auto flagMask = ecs->GetReplicationFlags(self->handle_, component);
		uint64_t flags{ 0 };
		if (flagMask && qword < flagMask->NumQwords()) {
			flags = flagMask->GetBuf()[qword];
		}

		push(L, flags);
		return 1;
	}

	void ReplicateComponent(lua_State* L, EntityHandle entity, ExtComponentType component, uint32_t qword, uint64_t flags)
	{
		auto ecs = gExtender->GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
		if (!ecs->GetEntityWorld()->Replication) {
			OsiError("Replication system is unavailable");
			return;
		}

		auto replicationFlags = ecs->GetOrCreateReplicationFlags(entity, component);
		if (replicationFlags) {
			replicationFlags->EnsureSize((qword + 1) * 64);
			bool changed = (replicationFlags->GetBuf()[qword] & flags) != flags;
			replicationFlags->GetBuf()[qword] |= flags;
			if (changed) {
				ecs->NotifyReplicationFlagsDirtied();
			}
		} else {
			OsiError("Unable to replicate; this component type cannot be replicated or the replication ID is not mapped");
		}
	}

	int EntityProxy::SetReplicationFlags(lua_State* L)
	{
		StackCheck _(L);
		auto self = get<EntityProxy*>(L, 1);
		auto component = get<ExtComponentType>(L, 2);

		uint64_t flags{ 0 };
		if (lua_gettop(L) > 2) {
			flags = get<uint64_t>(L, 3);
		}

		uint32_t qword{ 0 };
		if (lua_gettop(L) > 3) {
			qword = get<uint32_t>(L, 4);
		}

		ReplicateComponent(L, self->handle_, component, qword, flags);
		return 0;
	}

	int EntityProxy::Replicate(lua_State* L)
	{
		StackCheck _(L);
		auto self = get<EntityProxy*>(L, 1);
		auto component = get<ExtComponentType>(L, 2);

		ReplicateComponent(L, self->handle_, component, 0, 0xffffffffffffffffull);
		return 0;
	}

	int EntityProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<EntityProxy*>(L, 1);
		auto key = get<FixedString>(L, 2);

		if (key == GFS.strGetComponent) {
			push(L, &EntityProxy::GetComponent);
			return 1;
		}

		if (key == GFS.strGetAllComponents) {
			push(L, &EntityProxy::GetAllComponents);
			return 1;
		}

		if (key == GFS.strGetAllComponentNames) {
			push(L, &EntityProxy::GetAllComponentNames);
			return 1;
		}

		if (key == GFS.strGetEntityType) {
			push(L, &EntityProxy::GetEntityType);
			return 1;
		}

		if (key == GFS.strGetSalt) {
			push(L, &EntityProxy::GetSalt);
			return 1;
		}

		if (key == GFS.strGetIndex) {
			push(L, &EntityProxy::GetIndex);
			return 1;
		}

		if (key == GFS.strIsAlive) {
			push(L, &EntityProxy::IsAlive);
			return 1;
		}

		if (key == GFS.strGetReplicationFlags) {
			push(L, &EntityProxy::GetReplicationFlags);
			return 1;
		}

		if (key == GFS.strSetReplicationFlags) {
			push(L, &EntityProxy::SetReplicationFlags);
			return 1;
		}

		if (key == GFS.strReplicate) {
			push(L, &EntityProxy::Replicate);
			return 1;
		}

		auto componentType = EnumInfo<ExtComponentType>::Find(key);
		if (componentType) {
			auto ecs = GetEntitySystem(L);
			auto rawComponent = ecs->GetRawComponent(self->handle_, *componentType);
			if (rawComponent != nullptr) {
				PushComponent(L, GetEntitySystem(L), self->handle_, *componentType, GetCurrentLifetime(L));
			} else {
				push(L, nullptr);
			}
		} else {
			auto componentTypeName = get<char const*>(L, 2);
			luaL_error(L, "Not a valid EntityProxy method or component type: %s", componentTypeName);
		}

		return 1;
	}

	int EntityProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[100];
		sprintf_s(entityName, "Entity (%016llx)", handle_.Handle);
		push(L, entityName);
		return 1;
	}

	EntityHandle do_get(lua_State* L, int index, Overload<EntityHandle>)
	{
		if (lua_type(L, index) == LUA_TNIL) {
			return NullEntityHandle;
		} else {
			return EntityProxy::CheckUserData(L, index)->Handle();
		}
	}

	ecs::EntityRef do_get(lua_State* L, int index, Overload<ecs::EntityRef>)
	{
		if (lua_type(L, index) == LUA_TNIL) {
			return ecs::EntityRef{ NullEntityHandle, State::FromLua(L)->GetEntityWorld() };
		} else {
			return ecs::EntityRef{ EntityProxy::CheckUserData(L, index)->Handle(), State::FromLua(L)->GetEntityWorld() };
		}
	}

	TypeInformationRef do_get(lua_State* L, int index, Overload<TypeInformationRef>)
	{
		luaL_error(L, "TypeInformationRef is an engine-only type");
		return {};
	}

	EntityProxy* do_get(lua_State* L, int index, Overload<EntityProxy*>)
	{
		luaL_checktype(L, index, LUA_TUSERDATA);
		return EntityProxy::CheckUserData(L, index);
	}

	void RegisterEntityProxy(lua_State* L)
	{
		EntityProxy::RegisterMetatable(L);
	}
}
