#pragma once

BEGIN_NS(lua)

struct EntityEventHelpers
{
	static constexpr uint64_t ReplicationEventHandleType = 1;
	static constexpr uint64_t ComponentEventHandleType = 2;

	static std::optional<LuaEntitySubscriptionId> SubscribeReplication(lua_State* L, EntityHandle entity, ExtComponentType component, RegistryEntry&& hook, std::optional<uint64_t> flags);
	static LuaEntitySubscriptionId Subscribe(lua_State* L, EntityHandle entity, ExtComponentType component, 
		EntityComponentEvent event, EntityComponentEventFlags flags, RegistryEntry&& hook);
	static bool Unsubscribe(lua_State* L, LuaEntitySubscriptionId handle);

};

END_SE()
