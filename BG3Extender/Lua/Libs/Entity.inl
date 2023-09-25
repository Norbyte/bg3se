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
		EntityProxy::New(L, handle);
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
		// cls->ComponentTypeToIndex.Find()
		std::copy(cls->InstanceToPageMap.Keys.begin(), cls->InstanceToPageMap.Keys.end(), std::back_inserter(entities));
	}

	return entities;
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
	END_MODULE()
}

END_NS()
