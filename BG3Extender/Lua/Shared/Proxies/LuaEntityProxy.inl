#include <Lua/Shared/Proxies/LuaEntityProxy.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Projectile.h>
#include <GameDefinitions/Components/Stats.h>
#include <GameDefinitions/Components/Boosts.h>
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
						lua_settable(L, -3);
					} else if (warnOnMissing) {
						auto name = ecs->GetComponentName(typeInfo.Key());
						if (name) {
							OsiWarn("No model found for component: " << *name);
						}
					}
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


	char const* const ComponentHandleProxy::MetatableName = "ComponentHandleProxy";

	ComponentHandleProxy::ComponentHandleProxy(ComponentHandle const& handle, ecs::EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int ComponentHandleProxy::GetType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ComponentHandleProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int ComponentHandleProxy::GetTypeName(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ComponentHandleProxy*>(L, 1);
		auto name = self->entitySystem_->GetComponentName(ecs::HandleTypeIndex(self->handle_.GetType()));
		push(L, name);
		return 1;
	}

	int ComponentHandleProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ComponentHandleProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int ComponentHandleProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ComponentHandleProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	int ComponentHandleProxy::GetComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = get<ComponentHandleProxy*>(L, 1);
		auto componentType = self->entitySystem_->GetComponentType((ecs::HandleTypeIndex)self->handle_.GetType());
		if (componentType) {
			// PushComponent(L, self->entitySystem_, self->handle_, *componentType, GetCurrentLifetime(L));
			ERR("FIXME");
			push(L, nullptr);
		} else {
			OsiError("No component model exists for this component type");
			push(L, nullptr);
		}

		return 1;
	}

	void ComponentHandleProxy::PopulateMetatable(lua_State* L)
	{
		lua_newtable(L);

		push(L, &ComponentHandleProxy::GetType);
		lua_setfield(L, -2, "GetType");

		push(L, &ComponentHandleProxy::GetTypeName);
		lua_setfield(L, -2, "GetTypeName");

		push(L, &ComponentHandleProxy::GetSalt);
		lua_setfield(L, -2, "GetSalt");

		push(L, &ComponentHandleProxy::GetIndex);
		lua_setfield(L, -2, "GetIndex");

		push(L, &ComponentHandleProxy::GetComponent);
		lua_setfield(L, -2, "Get");

		lua_setfield(L, -2, "__index");
	}

	int ComponentHandleProxy::Index(lua_State* L)
	{
		OsiError("Should not get here!");
		return 0;
	}

	int ComponentHandleProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);

		std::optional<STDString const*> componentName;
		if (gExtender->GetServer().IsInServerThread()) {
			componentName = gExtender->GetServer().GetEntityHelpers().GetComponentName((ecs::HandleTypeIndex)handle_.GetType());
		} else {
			componentName = gExtender->GetClient().GetEntityHelpers().GetComponentName((ecs::HandleTypeIndex)handle_.GetType());
		}

		char entityName[200];
		if (componentName) {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s Object (%016llx)", (*componentName)->c_str(), handle_.Handle);
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Object (%016llx)", handle_.Handle);
		}

		push(L, entityName);
		return 1;
	}

	ComponentHandle do_get(lua_State* L, int index, Overload<ComponentHandle>)
	{
		if (lua_type(L, index) == LUA_TNIL) {
			return ComponentHandle{ ComponentHandle::NullHandle };
		} else {
			return ComponentHandleProxy::CheckUserData(L, index)->Handle();
		}
	}

	ComponentHandleProxy* do_get(lua_State* L, int index, Overload<ComponentHandleProxy*>)
	{
		luaL_checktype(L, index, LUA_TUSERDATA);
		return ComponentHandleProxy::CheckUserData(L, index);
	}

	void RegisterEntityProxy(lua_State* L)
	{
		EntityProxy::RegisterMetatable(L);
		ComponentHandleProxy::RegisterMetatable(L);
	}
}
