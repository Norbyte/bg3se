#pragma once

BEGIN_NS(lua)

class EntityReplicationEventHooks
{
public:
	using SubscriptionIndex = uint32_t;

	virtual std::optional<SubscriptionIndex> Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t fields, RegistryEntry&& hook) = 0;
	virtual bool Unsubscribe(SubscriptionIndex index) = 0;

	struct ReplicationHook
	{
		uint64_t Fields;
		RegistryEntry Hook;
		// Needed for looking up unsubscribe data
		ecs::ReplicationTypeIndex Type;
		EntityHandle Entity;
	};

	struct ReplicationHooks
	{
		uint64_t Fields;
		Array<SubscriptionIndex> GlobalHooks;
		HashMap<EntityHandle, Array<SubscriptionIndex>> EntityHooks;
	};

	struct DeferredEvent
	{
		EntityHandle Entity;
		uint64_t Fields;
		ecs::ReplicationTypeIndex Type;
		ReplicationHook const& Hook;
	};
};

END_SE()
