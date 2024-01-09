#include <Lua/Shared/Proxies/LuaEntityProxy.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/All.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

ecs::EntitySystemHelpersBase* GetEntitySystem(lua_State* L)
{
	return State::FromLua(L)->GetEntitySystemHelpers();
}

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


void EntityHelper::PushComponentByType(lua_State* L, ExtComponentType componentType) const
{
	StackCheck _(L, 1);
	auto ecs = GetEntitySystem(L);
	PushComponent(L, ecs, handle_, componentType, GetCurrentLifetime(L));
}

Array<ExtComponentType> EntityHelper::GetAllComponentTypes() const
{
	Array<ExtComponentType> types;

	auto world = ecs_->GetEntityWorld();
	auto entityClass = world->GetEntityClass(handle_);
	if (entityClass != nullptr) {
		for (auto typeInfo : entityClass->ComponentTypeToIndex) {
			auto extType = ecs_->GetComponentType(typeInfo.Key());
			if (extType) {
				types.push_back(*extType);
			}
		}
	}

	return types;
}


EntityHandle EntityProxyMetatable::Get(lua_State* L, int index)
{
	CppValueMetadata meta;
	lua_get_cppvalue(L, index, MetatableTag::Entity, meta);
	return GetHandle(meta);
}

EntityHelper EntityProxyMetatable::GetHelper(lua_State* L, int index)
{
	return EntityHelper(Get(L, index), GetEntitySystem(L));
}

int EntityProxyMetatable::CreateComponent(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	auto componentType = get<ExtComponentType>(L, 2);
	auto ecs = GetEntitySystem(L);
	auto typeId = (uint16_t)*ecs->GetComponentIndex(componentType);
	auto world = ecs->GetEntityWorld();
	if (typeId < world->ComponentOpsList.size()) {
		auto ops = world->ComponentOpsList[typeId];
		if (ops != nullptr) {
			ops->AddImmediateDefaultComponent(handle.Handle, 0);
			PushComponent(L, ecs, handle, componentType, GetCurrentLifetime(L));
			return 1;
		}
	}

	OsiError("Unable to construct components of this type: " << componentType);
	push(L, nullptr);
	return 1;
}

int EntityProxyMetatable::GetComponent(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	auto componentType = get<ExtComponentType>(L, 2);
	auto ecs = GetEntitySystem(L);
	PushComponent(L, ecs, handle, componentType, GetCurrentLifetime(L));
	return 1;
}

int EntityProxyMetatable::GetAllComponents(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);

	bool warnOnMissing = false;
	if (lua_gettop(L) >= 2) {
		warnOnMissing = get<bool>(L, 2);
	}

	lua_newtable(L);

	auto ecs = GetEntitySystem(L);
	auto world = ecs->GetEntityWorld();
	auto entityClass = world->GetEntityClass(handle);
	if (entityClass != nullptr) {
		auto componentPtr = entityClass->InstanceToPageMap.Find(handle);
		if (componentPtr) {
			for (auto typeInfo : entityClass->ComponentTypeToIndex) {
				auto extType = ecs->GetComponentType(typeInfo.Key());
				if (extType) {
					auto const& meta = ecs->GetComponentMeta(*extType);
					auto component = entityClass->GetComponent(**componentPtr, typeInfo.Value(), meta.Size, meta.IsProxy);

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

int EntityProxyMetatable::GetAllComponentNames(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	auto reqMapped = get<std::optional<bool>>(L, 2);

	lua_newtable(L);

	auto ecs = GetEntitySystem(L);
	auto world = ecs->GetEntityWorld();
	auto entityClass = world->GetEntityClass(handle);
	if (entityClass != nullptr) {
		for (auto componentIdx : entityClass->ComponentTypeToIndex.Keys) {
			auto name = ecs->GetComponentName(componentIdx);
			if (name) {
				auto mapped = ecs->GetComponentType(componentIdx).has_value();
				if (!reqMapped || *reqMapped == mapped) {
					push(L, componentIdx.Value());
					push(L, **name);
					lua_rawset(L, -3);
				}
			}
		}
	}

	return 1;
}

int EntityProxyMetatable::GetEntityType(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	push(L, handle.GetType());
	return 1;
}

int EntityProxyMetatable::GetSalt(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	push(L, handle.GetSalt());
	return 1;
}

int EntityProxyMetatable::GetIndex(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	push(L, handle.GetIndex());
	return 1;
}

int EntityProxyMetatable::IsAlive(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	auto world = GetEntitySystem(L)->GetEntityWorld();
	bool alive{ false };
	auto entityClass = world->GetEntityClass(handle);
	if (entityClass != nullptr) {
		alive = (bool)entityClass->InstanceToPageMap.Find(handle);
	}

	push(L, alive);
	return 1;
}


int EntityProxyMetatable::GetReplicationFlags(lua_State* L)
{
	StackCheck _(L, 1);
	auto handle = Get(L, 1);
	auto component = get<ExtComponentType>(L, 2);

	uint32_t qword{ 0 };
	if (lua_gettop(L) > 2) {
		qword = get<uint32_t>(L, 3);
	}

	auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
	auto flagMask = ecs->GetReplicationFlags(handle, component);
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
		OsiError("Unable to replicate; " << component << " type cannot be replicated or the replication ID is not mapped");
	}
}

int EntityProxyMetatable::SetReplicationFlags(lua_State* L)
{
	StackCheck _(L);
	auto handle = Get(L, 1);
	auto component = get<ExtComponentType>(L, 2);

	uint64_t flags{ 0 };
	if (lua_gettop(L) > 2) {
		flags = get<uint64_t>(L, 3);
	}

	uint32_t qword{ 0 };
	if (lua_gettop(L) > 3) {
		qword = get<uint32_t>(L, 4);
	}

	ReplicateComponent(L, handle, component, qword, flags);
	return 0;
}

int EntityProxyMetatable::Replicate(lua_State* L)
{
	StackCheck _(L);
	auto handle = Get(L, 1);
	auto component = get<ExtComponentType>(L, 2);

	ReplicateComponent(L, handle, component, 0, 0xffffffffffffffffull);
	return 0;
}


int EntityProxyMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	StackCheck _(L, 1);
	auto handle = GetHandle(self);
	auto key = get<FixedString>(L, 2);

	if (key == GFS.strCreateComponent) {
		push(L, &CreateComponent);
		return 1;
	}

	if (key == GFS.strGetComponent) {
		push(L, &GetComponent);
		return 1;
	}

	if (key == GFS.strGetAllComponents) {
		push(L, &GetAllComponents);
		return 1;
	}

	if (key == GFS.strGetAllComponentNames) {
		push(L, &GetAllComponentNames);
		return 1;
	}

	if (key == GFS.strGetEntityType) {
		push(L, &GetEntityType);
		return 1;
	}

	if (key == GFS.strGetSalt) {
		push(L, &GetSalt);
		return 1;
	}

	if (key == GFS.strGetIndex) {
		push(L, &GetIndex);
		return 1;
	}

	if (key == GFS.strIsAlive) {
		push(L, &IsAlive);
		return 1;
	}

	if (key == GFS.strGetReplicationFlags) {
		push(L, &GetReplicationFlags);
		return 1;
	}

	if (key == GFS.strSetReplicationFlags) {
		push(L, &SetReplicationFlags);
		return 1;
	}

	if (key == GFS.strReplicate) {
		push(L, &Replicate);
		return 1;
	}

	if (key == GFS.strVars) {
		UserVariableHolderMetatable::Make(L, handle);
		return 1;
	}

	auto componentType = EnumInfo<ExtComponentType>::Find(key);
	if (componentType) {
		auto ecs = GetEntitySystem(L);
		auto rawComponent = ecs->GetRawComponent(handle, *componentType);
		if (rawComponent != nullptr) {
			PushComponent(L, GetEntitySystem(L), handle, *componentType, GetCurrentLifetime(L));
		} else {
			push(L, nullptr);
		}
	} else {
		auto componentTypeName = get<char const*>(L, 2);
		luaL_error(L, "Not a valid EntityProxy method or component type: %s", componentTypeName);
	}

	return 1;
}

bool EntityProxyMetatable::IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	CppValueMetadata other;
	return lua_try_get_cppvalue(L, otherIndex, MetatableTag::Entity, other)
		&& GetHandle(other) == GetHandle(self);
}

char const* EntityProxyMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	return "EntityProxy";
}

int EntityProxyMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	StackCheck _(L, 1);
	char entityName[100];
	auto handle = GetHandle(self);
	sprintf_s(entityName, "Entity (%016llx)", handle.Handle);
	push(L, entityName);
	return 1;
}

EntityHandle do_get(lua_State* L, int index, Overload<EntityHandle>)
{
	if (lua_type(L, index) == LUA_TNIL) {
		return NullEntityHandle;
	} else {
		return EntityProxyMetatable::Get(L, index);
	}
}

ecs::EntityRef do_get(lua_State* L, int index, Overload<ecs::EntityRef>)
{
	if (lua_type(L, index) == LUA_TNIL) {
		return ecs::EntityRef{ NullEntityHandle, State::FromLua(L)->GetEntityWorld() };
	} else {
		return ecs::EntityRef{ EntityProxyMetatable::Get(L, index), State::FromLua(L)->GetEntityWorld() };
	}
}

TypeInformationRef do_get(lua_State* L, int index, Overload<TypeInformationRef>)
{
	luaL_error(L, "TypeInformationRef is an engine-only type");
	return {};
}

EntityHelper do_get(lua_State* L, int index, Overload<EntityHelper>)
{
	luaL_checktype(L, index, LUA_TUSERDATA);
	return EntityProxyMetatable::GetHelper(L, index);
}

END_NS()
