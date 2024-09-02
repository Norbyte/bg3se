#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(lua)

EntityReplicationEventHooks::EntityReplicationEventHooks(lua::State& state)
	: state_(state)
{}

EntityReplicationEventHooks::~EntityReplicationEventHooks() {}

EntityReplicationEventHooks::ReplicationHooks& EntityReplicationEventHooks::AddComponentType(ecs::ReplicationTypeIndex type)
{
	auto index = (unsigned)type;
	if (!hookedReplicationComponentMask_[index]) {
		hookedReplicationComponentMask_.Set(index);
		while (hookedReplicationComponents_.size() <= index) {
			hookedReplicationComponents_.push_back(ReplicationHooks{});
		}
	}

	return hookedReplicationComponents_[index];
}

EntityReplicationEventHooks::SubscriptionIndex EntityReplicationEventHooks::Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t flags, RegistryEntry&& hook)
{
	SubscriptionIndex index;
	auto sub = subscriptions_.Add(index);
	sub->InvalidationFlags = flags;
	sub->Hook = std::move(hook);
	sub->Type = type;
	sub->Entity = entity;

	auto& pool = AddComponentType(type);
	pool.InvalidationFlags |= flags;
	if (!entity) {
		pool.GlobalHooks.Add(index);
	} else {
		auto entityHooks = pool.EntityHooks.try_get(entity);
		if (entityHooks) {
			entityHooks->push_back(index);
		} else {
			entityHooks = pool.EntityHooks.add_key(entity);
			entityHooks->push_back(index);
		}
	}

	return index;
}

bool EntityReplicationEventHooks::Unsubscribe(SubscriptionIndex index)
{
	auto sub = subscriptions_.Find(index);
	if (sub == nullptr) {
		return false;
	}

	auto& pool = hookedReplicationComponents_[(unsigned)sub->Type];
	if (!sub->Entity) {
		for (unsigned i = 0; i < pool.GlobalHooks.size(); i++) {
			if (pool.GlobalHooks[i] == index) {
				pool.GlobalHooks.ordered_remove_at(i);
				break;
			}
		}
	} else {
		auto entityHooks = pool.EntityHooks.try_get(sub->Entity);
		if (entityHooks) {
			for (unsigned i = 0; i < entityHooks->size(); i++) {
				if ((*entityHooks)[i] == index) {
					entityHooks->ordered_remove_at(i);
					break;
				}
			}
		}
	}

	subscriptions_.Free(index);
	return true;
}

void EntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world)
{
	if (!world.Replication || !world.Replication->Dirty) return;

	for (unsigned i = 0; i < world.Replication->ComponentPools.size(); i++) {
		auto const& pool = world.Replication->ComponentPools[i];
		if (hookedReplicationComponentMask_[i] && pool.size() > 0) {
			for (auto const& entity : pool) {
				OnEntityReplication(world, entity.Key(), entity.Value(), ecs::ReplicationTypeIndex{ (uint16_t)i });
			}
		}
	}
}

void EntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type)
{
	auto& hooks = hookedReplicationComponents_[(unsigned)type];
	auto word1 = *flags.GetBuf();
	if ((hooks.InvalidationFlags & word1) == 0) return;

	Array<DeferredEvent> events;

	for (auto index : hooks.GlobalHooks) {
		auto hook = subscriptions_.Find(index);
		if (hook != nullptr && (hook->InvalidationFlags & word1) != 0) {
			events.push_back({
				.Entity = entity,
				.Flags = flags,
				.Type = type,
				.Hook = *hook
			});
		}
	}

	auto entityHooks = hooks.EntityHooks.try_get(entity);
	if (entityHooks) {
		for (auto index : *entityHooks) {
			auto hook = subscriptions_.Find(index);
			if (hook != nullptr && (hook->InvalidationFlags & word1) != 0) {
				events.push_back({
					.Entity = entity,
					.Flags = flags,
					.Type = type,
					.Hook = *hook
				});
			}
		}
	}

	if (!events.empty()) {
		LuaVirtualPin lua(state_.GetExtensionState());
		if (lua) {
			for (auto const& e : events) {
				CallHandler(e.Entity, e.Flags, e.Type, e.Hook);
			}
		}
	}
}

void EntityReplicationEventHooks::CallHandler(EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type, ReplicationHook const& hook)
{
	auto L = state_.GetState();
	auto componentType = state_.GetEntitySystemHelpers()->GetComponentType(type);
	auto word1 = *flags.GetBuf();
	hook.Hook.Push();
	Ref func(L, lua_absindex(L, -1));

	ProtectedFunctionCaller<std::tuple<EntityHandle, ExtComponentType, uint64_t>, void> caller{ func, std::tuple(entity, *componentType, word1) };
	caller.Call(L, "Entity replication event dispatch");
	lua_pop(L, 1);
}

END_NS()
