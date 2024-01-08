#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(lua)

EntityReplicationEventHooks::EntityReplicationEventHooks(lua::State& state)
	: state_(state)
{}

EntityReplicationEventHooks::~EntityReplicationEventHooks() {}

uint32_t EntityReplicationEventHooks::FindFreeSlot()
{
	if (!freeSlots_.empty()) {
		return freeSlots_.pop_last();
	}

	auto slot = subscriptions_.size();
	subscriptions_.push_back(nullptr);
	return slot;
}

EntityReplicationEventHooks::ReplicationHooks& EntityReplicationEventHooks::AddComponentType(ecs::ReplicationTypeIndex type)
{
	auto index = (unsigned)type.Value();
	if (!hookedReplicationComponentMask_[index]) {
		hookedReplicationComponentMask_.Set(index);
		while (hookedReplicationComponents_.size() <= index) {
			hookedReplicationComponents_.push_back(ReplicationHooks{});
		}
	}

	return hookedReplicationComponents_[index];
}

void EntityReplicationEventHooks::Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t flags, RegistryEntry&& hook)
{
	auto slot = FindFreeSlot();
	auto& pool = AddComponentType(type);
	auto hookEntry = GameAlloc<ReplicationHook>();
	hookEntry->InvalidationFlags = flags;
	hookEntry->Hook = std::move(hook);
	hookEntry->Type = type;
	hookEntry->Entity = entity;
	hookEntry->Index = slot;

	pool.InvalidationFlags |= flags;
	if (!entity) {
		pool.GlobalHooks.Add(hookEntry);
	} else {
		auto entityHooks = pool.EntityHooks.Find(entity);
		if (entityHooks) {
			(*entityHooks)->push_back(hookEntry);
		} else {
			entityHooks = pool.EntityHooks.Set(entity, {});
			(*entityHooks)->push_back(hookEntry);
		}
	}

	subscriptions_[slot] = hookEntry;
}

bool EntityReplicationEventHooks::Unsubscribe(uint32_t index)
{
	if (index >= subscriptions_.size() || subscriptions_[index] == nullptr) {
		return false;
	}

	auto sub = subscriptions_[index];
	auto& pool = hookedReplicationComponents_[(unsigned)sub->Type.Value()];
	if (!sub->Entity) {
		for (unsigned i = 0; i < pool.GlobalHooks.size(); i++) {
			if (pool.GlobalHooks[i]->Index == index) {
				pool.GlobalHooks.remove_at(i);
				break;
			}
		}
	} else {
		auto entityHooks = pool.EntityHooks.Find(sub->Entity);
		if (entityHooks) {
			for (unsigned i = 0; i < (*entityHooks)->size(); i++) {
				if ((**entityHooks)[i]->Index == index) {
					(*entityHooks)->remove_at(i);
					break;
				}
			}
		}
	}

	subscriptions_[index] = nullptr;
	freeSlots_.push_back(index);
	GameFree(sub);
	return true;
}

void EntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world)
{
	if (!world.Replication || !world.Replication->Dirty) return;

	for (unsigned i = 0; i < world.Replication->ComponentPools.size(); i++) {
		auto const& pool = world.Replication->ComponentPools[i];
		if (hookedReplicationComponentMask_[i] && pool.size() > 0) {
			for (auto const& entity : pool) {
				OnEntityReplication(world, entity.Key(), entity.Value(), i);
			}
		}
	}
}

void EntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, BitSet<> const& flags, ecs::ReplicationTypeIndex type)
{
	auto& hooks = hookedReplicationComponents_[type.Value()];
	auto word1 = *flags.GetBuf();
	if ((hooks.InvalidationFlags & word1) == 0) return;

	for (auto const& hook : hooks.GlobalHooks) {
		if ((hook->InvalidationFlags & word1) != 0) {
			CallHandler(entity, flags, type, *hook);
		}
	}

	auto entityHooks = hooks.EntityHooks.Find(entity);
	if (entityHooks) {
		for (auto const& hook : **entityHooks) {
			if ((hook->InvalidationFlags & word1) != 0) {
				CallHandler(entity, flags, type, *hook);
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
