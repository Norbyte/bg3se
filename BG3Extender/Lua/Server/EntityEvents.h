#pragma once

BEGIN_NS(lua)

class EntityReplicationEventHooks
{
public:
	using SubscriptionIndex = uint32_t;

	EntityReplicationEventHooks(lua::State& state);
	~EntityReplicationEventHooks();

	SubscriptionIndex Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t flags, RegistryEntry&& hook);
	bool Unsubscribe(SubscriptionIndex index);

	void OnEntityReplication(ecs::EntityWorld& world);

private:
	struct ReplicationHook
	{
		uint64_t InvalidationFlags;
		RegistryEntry Hook;
		// Needed for looking up unsubscribe data
		ecs::ReplicationTypeIndex Type;
		EntityHandle Entity;
	};

	struct ReplicationHooks
	{
		uint64_t InvalidationFlags;
		Array<SubscriptionIndex> GlobalHooks;
		HashMap<EntityHandle, Array<SubscriptionIndex>> EntityHooks;
	};

	lua::State& state_;
	BitSet<> hookedReplicationComponentMask_;
	Array<ReplicationHooks> hookedReplicationComponents_;
	SaltedPool<ReplicationHook> subscriptions_;

	void OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type);
	void CallHandler(EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type, ReplicationHook const& hook);
	ReplicationHooks& AddComponentType(ecs::ReplicationTypeIndex type);
};

END_SE()
