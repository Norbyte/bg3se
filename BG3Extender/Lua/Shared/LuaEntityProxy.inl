#include <Lua/Shared/LuaEntityProxy.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Projectile.h>
#include <GameDefinitions/Components/Stats.h>
#include <GameDefinitions/Components/Boosts.h>

namespace bg3se::lua
{
	char const* const EntityProxy::MetatableName = "EntityProxy";

	EntityProxy::EntityProxy(EntityHandle const& handle, EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int EntityProxy::HasRawComponent(lua_State* L)
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

	int EntityProxy::GetAllRawComponents(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);

		lua_newtable(L);
		int idx{ 1 };

		auto entity = self->entitySystem_->GetEntityWorld()->GetEntity(self->handle_);
		if (entity) {
			for (unsigned i = 0; i < entity->ComponentHandles.Size(); i++) {
				auto componentHandle = entity->ComponentHandles[i];
				if (componentHandle) {
					auto componentIdx = entity->SlotIndexToComponentIdMap[i];
					auto name = self->entitySystem_->GetComponentName((EntityWorldBase::ComponentTypeIndex)componentIdx);

					push(L, idx++);
					lua_newtable(L);
					settable(L, "Type", name);
					settable(L, "Handle", componentHandle);
					lua_settable(L, -3);
				}
			}
		}

		return 1;
	}

	template <class T>
	void PushComponentType(lua_State* L, EntitySystemHelpersBase* helpers, EntityHandle const& handle, 
		LifetimeHolder const& lifetime, bool logError)
	{
		auto component = helpers->GetEntityComponent<T>(handle, false);
		if (component) {
			ObjectProxy::MakeRef<T>(L, component, lifetime);
		} else {
			if (logError) {
				OsiError("Couldn't get component of type " << T::ComponentType);
			}

			push(L, nullptr);
		}
	}

	template <class T>
	void PushComponentType(lua_State* L, EntitySystemHelpersBase* helpers, ObjectHandle const& handle, 
		LifetimeHolder const& lifetime, bool logError)
	{
		auto component = helpers->GetComponent<T>(handle, false);
		if (component) {
			ObjectProxy::MakeRef<T>(L, component, lifetime);
		} else {
			if (logError) {
				OsiError("Couldn't get component of type " << T::ComponentType);
			}

			push(L, nullptr);
		}
	}

#define T(cls) \
	case cls::ComponentType: \
	{ \
		PushComponentType<cls>(L, helpers, handle, lifetime, logError); \
		break; \
	}

	void PushComponent(lua_State* L, EntitySystemHelpersBase* helpers, EntityHandle const& handle, ExtComponentType componentType,
		LifetimeHolder const& lifetime, bool logError)
	{
		switch (componentType) {

#include <GameDefinitions/Components/AllComponentTypes.inl>

		default:
			OsiError("Don't know how to push component type: " << componentType);
			break;
		}
	}

#undef T

#define T(cls) \
	case cls::ComponentType: \
	{ \
		PushComponentType<cls>(L, helpers, handle, lifetime, logError); \
		break; \
	}

	void PushComponent(lua_State* L, EntitySystemHelpersBase* helpers, ObjectHandle const& handle, ExtComponentType componentType,
		LifetimeHolder const& lifetime, bool logError)
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
		auto self = checked_get<EntityProxy*>(L, 1);
		auto componentType = checked_get<ExtComponentType>(L, 2);
		PushComponent(L, self->entitySystem_, self->handle_, componentType, GetCurrentLifetime(), false);
		return 1;
	}

	int EntityProxy::GetAllComponents(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);

		bool warnOnMissing = false;
		if (lua_gettop(L) >= 2) {
			warnOnMissing = checked_get<bool>(L, 2);
		}

		lua_newtable(L);

		auto entity = self->entitySystem_->GetEntityWorld()->GetEntity(self->handle_);
		if (entity) {
			for (unsigned i = 0; i < entity->ComponentHandles.Size(); i++) {
				auto componentHandle = entity->ComponentHandles[i];
				if (componentHandle) {
					auto componentIdx = entity->SlotIndexToComponentIdMap[i];
					auto type = self->entitySystem_->GetComponentType((EntityWorldBase::ComponentTypeIndex)componentIdx);

					if (type) {
						push(L, type);
						PushComponent(L, self->entitySystem_, self->handle_, *type, GetCurrentLifetime(), true);
						lua_settable(L, -3);
					} else if (warnOnMissing) {
						auto name = self->entitySystem_->GetComponentName((EntityWorldBase::ComponentTypeIndex)componentIdx);
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

	int EntityProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto key = checked_get<FixedString>(L, 2);

		if (key == GFS.strHasRawComponent) {
			push(L, &EntityProxy::HasRawComponent);
			return 1;
		}

		if (key == GFS.strGetAllRawComponents) {
			push(L, &EntityProxy::GetAllRawComponents);
			return 1;
		}

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
			PushComponent(L, self->entitySystem_, self->handle_, *componentType, GetCurrentLifetime(), false);
		} else {
			auto componentTypeName = checked_get<char const*>(L, 2);
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

	int ObjectHandleProxy::GetComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		auto componentType = self->entitySystem_->GetComponentType((EntityWorldBase::HandleTypeIndex)self->handle_.GetType());
		if (componentType) {
			PushComponent(L, self->entitySystem_, self->handle_, *componentType, GetCurrentLifetime(), false);
		} else {
			OsiError("No component model exists for this component type");
			push(L, nullptr);
		}

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

		push(L, &ObjectHandleProxy::GetComponent);
		lua_setfield(L, -2, "Get");

		lua_setfield(L, -2, "__index");
	}

	int ObjectHandleProxy::Index(lua_State* L)
	{
		OsiError("Should not get here!");
		return 0;
	}

	int ObjectHandleProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);

		std::optional<STDString> componentName;
		if (gExtender->GetServer().IsInServerThread()) {
			componentName = gExtender->GetServer().GetEntityHelpers().GetComponentName((EntityWorldBase::HandleTypeIndex)handle_.GetType());
		} else {
			componentName = gExtender->GetClient().GetEntityHelpers().GetComponentName((EntityWorldBase::HandleTypeIndex)handle_.GetType());
		}

		char entityName[200];
		if (componentName) {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s Object (%016llx)", componentName->c_str(), handle_.Handle);
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Object (%016llx)", handle_.Handle);
		}

		push(L, entityName);
		return 1;
	}


	void RegisterEntityProxy(lua_State* L)
	{
		EntityProxy::RegisterMetatable(L);
		ObjectHandleProxy::RegisterMetatable(L);
	}
}
