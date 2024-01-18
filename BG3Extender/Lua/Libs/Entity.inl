/// <lua_module>Entity</lua_module>
BEGIN_NS(lua::entity)

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
		if (cls->ComponentTypeToIndex.Find(*componentType)) {
			std::copy(cls->InstanceToPageMap.Keys.begin(), cls->InstanceToPageMap.Keys.end(), std::back_inserter(entities));
		}
	}

	return entities;
}

Array<EntityHandle> GetAllEntities(lua_State* L)
{
	Array<EntityHandle> entities;

	auto world = State::FromLua(L)->GetEntitySystemHelpers()->GetEntityWorld();
	for (auto cls : world->EntityTypes->EntityClasses) {
		std::copy(cls->InstanceToPageMap.Keys.begin(), cls->InstanceToPageMap.Keys.end(), std::back_inserter(entities));
	}

	return entities;
}

EntityReplicationEventHooks::SubscriptionIndex Subscribe(lua_State* L, ExtComponentType type, FunctionRef func, std::optional<EntityHandle> entity, std::optional<uint64_t> flags)
{
	auto hooks = State::FromLua(L)->GetReplicationEventHooks();
	if (!hooks) {
		luaL_error(L, "Entity events are only available on the server");
	}

	auto replicationType = State::FromLua(L)->GetEntitySystemHelpers()->GetReplicationIndex(type);
	if (!replicationType) {
		luaL_error(L, "No events are available for components of type %s", EnumInfo<ExtComponentType>::Store->Find((EnumUnderlyingType)type).GetString());
	}

	return hooks->Subscribe(*replicationType, entity ? *entity : EntityHandle{}, flags ? *flags : 0xffffffffffffffffull, RegistryEntry(L, func.Index));
}

bool Unsubscribe(lua_State* L, unsigned index)
{
	auto hooks = State::FromLua(L)->GetReplicationEventHooks();
	if (!hooks) {
		luaL_error(L, "Entity events are only available on the server");
	}

	return hooks->Unsubscribe(index);
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
	MODULE_FUNCTION(Unsubscribe)
	END_MODULE()
}

END_NS()
