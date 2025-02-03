#pragma once

#include <Lua/Shared/EntityReplicationEvents.h>

BEGIN_NS(ecl::lua)

struct ComponentRef
{
    EntityHandle Entity;
    ecs::ReplicationTypeIndex Type;

    inline bool operator == (ComponentRef const& o) const
    {
        return Entity == o.Entity && Type == o.Type;
    }
};

END_SE()

BEGIN_SE()

template <>
inline uint64_t HashMapHash<ecl::lua::ComponentRef>(ecl::lua::ComponentRef const& v)
{
    return HashMulti(v.Entity, v.Type);
}

END_SE()

BEGIN_NS(ecl::lua)

// Tracker for capturing change events when the game has no builtin tracker for a particular field
struct DummyFieldTracker : public ecs::FieldTracker
{
    struct SharedChange
    {
        uint64_t FieldMask;
        void* Component;
    };

    struct SharedChangeSet
    {
        HashMap<EntityHandle, SharedChange> ChangedEntities;
    };

    ~DummyFieldTracker() override;
    void Add(EntityHandle entity, void* component) override;
    void FireEvents() override;
    void Remove(EntityHandle entity) override;

    SharedChangeSet* SharedChanges{ nullptr };
    ecs::FieldTracker* Base{ nullptr };
    uint64_t FieldMask{ 0 };
    Signal<EntityHandle, void*, uint64_t> PerComponentChangeEvent;
};

class ClientEntityReplicationEventHooks : public EntityReplicationEventHooks
{
public:
    ClientEntityReplicationEventHooks(State& state);
    ~ClientEntityReplicationEventHooks();

    std::optional<SubscriptionIndex> Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t fields, RegistryEntry&& hook) override;
    bool Unsubscribe(SubscriptionIndex index) override;

    void PostUpdate();

private:
    State& state_;
    BitSet<> hookedReplicationComponentMask_;
    Array<ReplicationHooks> hookedReplicationComponents_;
    SaltedPool<ReplicationHook> subscriptions_;
    HashSet<ComponentRef> pendingReplications_;
    Array<DeferredEvent> pendingEvents_;

    void OnEntityReplication(ecs::ReplicationTypeIndex const& type, EntityHandle entity, void* component, uint64_t fields);
    void CallHandler(EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type, ReplicationHook const& hook);
    ReplicationHooks* AddComponentType(ecs::ReplicationTypeIndex type);
    void RemoveComponentType(ecs::ReplicationTypeIndex type);
};

END_SE()
