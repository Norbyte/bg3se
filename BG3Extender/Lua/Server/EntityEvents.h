#pragma once

BEGIN_NS(lua)

class EntityReplicationEventHooks
{
public:
	EntityReplicationEventHooks(lua::State& state);
	~EntityReplicationEventHooks();

	void Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t flags, RegistryEntry&& hook);
	bool Unsubscribe(uint32_t index);

	void OnEntityReplication(ecs::EntityWorld& world);

private:
	struct ReplicationHook
	{
		uint64_t InvalidationFlags;
		RegistryEntry Hook;
		// Needed for looking up unsubscribe data
		ecs::ReplicationTypeIndex Type;
		EntityHandle Entity;
		uint32_t Index;
	};

	struct ReplicationHooks
	{
		uint64_t InvalidationFlags;
		Array<ReplicationHook*> GlobalHooks;
		MultiHashMap<EntityHandle, Array<ReplicationHook*>> EntityHooks;
	};

	lua::State& state_;
	BitSet<> hookedReplicationComponentMask_;
	Array<ReplicationHooks> hookedReplicationComponents_;
	Array<ReplicationHook*> subscriptions_;
	Array<uint32_t> freeSlots_;

	void OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type);
	void CallHandler(EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type, ReplicationHook const& hook);
	uint32_t FindFreeSlot();
	ReplicationHooks& AddComponentType(ecs::ReplicationTypeIndex type);
};

END_SE()
