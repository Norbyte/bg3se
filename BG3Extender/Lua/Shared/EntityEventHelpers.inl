BEGIN_NS(lua)

std::optional<LuaEntitySubscriptionId> EntityEventHelpers::SubscribeReplication(lua_State* L, EntityHandle entity, ExtComponentType component, RegistryEntry&& hook, std::optional<uint64_t> flags)
{
	auto replicationType = State::FromLua(L)->GetEntitySystemHelpers()->GetReplicationIndex(component);
	if (!replicationType) {
		luaL_error(L, "No replication events are available for components of type %s", EnumInfo<ExtComponentType>::GetStore().Find((EnumUnderlyingType)component).GetString());
	}

	auto hooks = State::FromLua(L)->GetReplicationEventHooks();
	auto index = hooks->Subscribe(*replicationType, entity, flags ? *flags : 0xffffffffffffffffull, std::move(hook));
	if (index) {
		return LuaEntitySubscriptionId((ReplicationEventHandleType << 32) | *index);
	} else {
		return {};
	}
}

LuaEntitySubscriptionId EntityEventHelpers::Subscribe(lua_State* L, EntityHandle entity, ExtComponentType component, 
	EntityComponentEvent event, EntityComponentEventFlags flags, RegistryEntry&& hook)
{
	auto state = State::FromLua(L);
	auto componentType = state->GetEntitySystemHelpers()->GetComponentIndex(component);
	if (!componentType) {
		luaL_error(L, "No events are available for components of type %s", EnumInfo<ExtComponentType>::GetStore().Find((EnumUnderlyingType)component).GetString());
	}

	auto const& meta = state->GetEntitySystemHelpers()->GetComponentMeta(component);
	if (meta.OneFrame) {
		if ((event & EntityComponentEvent::Destroy) == EntityComponentEvent::Destroy) {
			luaL_error(L, "Destroy events are not available for one-frame components (%s)", EnumInfo<ExtComponentType>::GetStore().Find((EnumUnderlyingType)component).GetString());
		}

		if ((event & EntityComponentEvent::Create) == EntityComponentEvent::Create
			&& (flags & EntityComponentEventFlags::Deferred) != EntityComponentEventFlags::Deferred) {
			flags |= EntityComponentEventFlags::Deferred;
			WARN("Only deferred create events are available for one-frame components (%s)", EnumInfo<ExtComponentType>::GetStore().Find((EnumUnderlyingType)component).GetString());
		}
	}

	auto& hooks = state->GetComponentEventHooks();
	auto index = hooks.Subscribe(*componentType, entity, event, flags, std::move(hook));
	return LuaEntitySubscriptionId((ComponentEventHandleType << 32) | index);
}

bool EntityEventHelpers::Unsubscribe(lua_State* L, LuaEntitySubscriptionId handle)
{
	switch ((uint64_t)handle >> 32) {
	case ReplicationEventHandleType:
	{
		return State::FromLua(L)->GetReplicationEventHooks()->Unsubscribe((uint32_t)handle);
	}

	case ComponentEventHandleType:
	{
		return State::FromLua(L)->GetComponentEventHooks().Unsubscribe((uint32_t)handle);
	}

	default:
		OsiWarn("Illegible subscription index");
		return false;
	}
}

END_NS()
