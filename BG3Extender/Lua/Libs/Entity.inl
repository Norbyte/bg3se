/// <lua_module>Entity</lua_module>
BEGIN_NS(lua::entity)

static constexpr uint64_t ReplicationEventHandleType = 1;
static constexpr uint64_t ComponentEventHandleType = 2;

std::optional<Guid> HandleToUuid(lua_State* L, EntityHandle entity)
{
	auto uuid = State::FromLua(L)->GetEntitySystemHelpers()->GetComponent<UuidComponent>(entity);
	if (uuid) {
		return uuid->EntityUuid;
	} else {
		return {};
	}
}

EntityHandle UuidToHandle(lua_State* L, Guid uuid)
{
	return State::FromLua(L)->GetEntitySystemHelpers()->GetEntityHandle(uuid);
}

UserReturn Get(lua_State* L, Guid uuid)
{
	auto handle = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityHandle(uuid);
	if (handle) {
		EntityProxyMetatable::Make(L, handle);
	} else {
		push(L, nullptr);
	}

	return 1;
}

MultiHashMap<Guid, EntityHandle> GetAllEntitiesWithUuid(lua_State* L)
{
	auto mappings = State::FromLua(L)->GetEntitySystemHelpers()->GetUuidMappings();
	if (mappings) {
		return mappings->Mappings;
	} else {
		return {};
	}
}

Array<EntityHandle> GetAllEntitiesWithComponent(lua_State* L, ExtComponentType component)
{
	auto ecs = State::FromLua(L)->GetEntitySystemHelpers();
	auto componentType = ecs->GetComponentIndex(component);
	if (!componentType) return {};

	Array<EntityHandle> entities;
	auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
	for (auto cls : world->EntityTypes->EntityClasses) {
		if (cls->ComponentTypeToIndex.try_get(*componentType)) {
			std::copy(cls->InstanceToPageMap.keys().begin(), cls->InstanceToPageMap.keys().end(), std::back_inserter(entities));
		}
	}

	return entities;
}

Array<EntityHandle> GetAllEntities(lua_State* L)
{
	Array<EntityHandle> entities;

	auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
	for (auto cls : world->EntityTypes->EntityClasses) {
		std::copy(cls->InstanceToPageMap.keys().begin(), cls->InstanceToPageMap.keys().end(), std::back_inserter(entities));
	}

	return entities;
}

uint64_t Subscribe(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity, std::optional<uint64_t> flags)
{
	auto hooks = State::FromLua(L)->GetReplicationEventHooks();
	if (!hooks) {
		luaL_error(L, "Entity events are only available on the server");
	}

	auto replicationType = State::FromLua(L)->GetEntitySystemHelpers()->GetReplicationIndex(type);
	if (!replicationType) {
		luaL_error(L, "No events are available for components of type %s", EnumInfo<ExtComponentType>::Store->Find((EnumUnderlyingType)type).GetString());
	}

	auto index = hooks->Subscribe(*replicationType, entity ? *entity : EntityHandle{}, flags ? *flags : 0xffffffffffffffffull, RegistryEntry(L, func.Index));
	return (ReplicationEventHandleType << 32) | index;
}

uint64_t OnCreate(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
	auto componentType = State::FromLua(L)->GetEntitySystemHelpers()->GetComponentIndex(type);
	if (!componentType) {
		luaL_error(L, "No events are available for components of type %s", EnumInfo<ExtComponentType>::Store->Find((EnumUnderlyingType)type).GetString());
	}

	auto& hooks = State::FromLua(L)->GetComponentEventHooks();
	auto index = hooks.Subscribe(*componentType, entity ? *entity : EntityHandle{}, EntityComponentEvent::Create, RegistryEntry(L, func.Index));
	return (ComponentEventHandleType << 32) | index;
}

uint64_t OnDestroy(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity)
{
	auto componentType = State::FromLua(L)->GetEntitySystemHelpers()->GetComponentIndex(type);
	if (!componentType) {
		luaL_error(L, "No events are available for components of type %s", EnumInfo<ExtComponentType>::Store->Find((EnumUnderlyingType)type).GetString());
	}

	auto& hooks = State::FromLua(L)->GetComponentEventHooks();
	auto index = hooks.Subscribe(*componentType, entity ? *entity : EntityHandle{}, EntityComponentEvent::Destroy, RegistryEntry(L, func.Index));
	return (ComponentEventHandleType << 32) | index;
}

bool Unsubscribe(lua_State* L, uint64_t index)
{
	switch (index >> 32) {
	case ReplicationEventHandleType:
	{
		auto hooks = State::FromLua(L)->GetReplicationEventHooks();
		if (!hooks) {
			luaL_error(L, "Entity events are only available on the server");
		}

		return hooks->Unsubscribe((uint32_t)index);
	}

	case ComponentEventHandleType:
	{
		return State::FromLua(L)->GetComponentEventHooks().Unsubscribe((uint32_t)index);
	}

	default:
		OsiWarn("Illegible subscription index");
		return false;
	}
}

void RegisterEntityLib()
{
	DECLARE_MODULE(Entity, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(HandleToUuid)
	MODULE_FUNCTION(UuidToHandle)
	MODULE_FUNCTION(Get)
	MODULE_FUNCTION(GetAllEntitiesWithUuid)
	MODULE_FUNCTION(GetAllEntitiesWithComponent)
	MODULE_FUNCTION(GetAllEntities)
	MODULE_FUNCTION(Subscribe)
	MODULE_NAMED_FUNCTION("OnChange", Subscribe)
	MODULE_FUNCTION(OnCreate)
	MODULE_FUNCTION(OnDestroy)
	MODULE_FUNCTION(Unsubscribe)
	END_MODULE()
}

END_NS()
