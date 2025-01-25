#pragma once

#include <Lua/Shared/EntityReplicationEvents.h>

BEGIN_NS(esv::lua)

class ServerEntityReplicationEventHooks : public bg3se::lua::EntityReplicationEventHooks
{
public:
    ServerEntityReplicationEventHooks(State& state);
    ~ServerEntityReplicationEventHooks();

    std::optional<SubscriptionIndex> Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t fields, RegistryEntry&& hook) override;
    bool Unsubscribe(SubscriptionIndex index) override;

    void OnEntityReplication(ecs::EntityWorld& world, ecs::SyncBuffers* buffers);

private:
    State& state_;
    BitSet<> hookedReplicationComponentMask_;
    Array<ReplicationHooks> hookedReplicationComponents_;
    SaltedPool<ReplicationHook> subscriptions_;

    void OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type);
    void CallHandler(EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type, ReplicationHook const& hook);
    ReplicationHooks& AddComponentType(ecs::ReplicationTypeIndex type);
};

END_SE()
