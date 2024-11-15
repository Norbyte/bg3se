#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Server/ServerEntityReplicationEvents.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

ServerEntityReplicationEventHooks::ServerEntityReplicationEventHooks(State& state)
	: state_(state)
{}

ServerEntityReplicationEventHooks::~ServerEntityReplicationEventHooks() {}

EntityReplicationEventHooks::ReplicationHooks& ServerEntityReplicationEventHooks::AddComponentType(ecs::ReplicationTypeIndex type)
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

std::optional<EntityReplicationEventHooks::SubscriptionIndex> ServerEntityReplicationEventHooks::Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t fields, RegistryEntry&& hook)
{
	SubscriptionIndex index;
	auto sub = subscriptions_.Add(index);
	sub->Fields = fields;
	sub->Hook = std::move(hook);
	sub->Type = type;
	sub->Entity = entity;

	auto& pool = AddComponentType(type);
	pool.Fields |= fields;
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

bool ServerEntityReplicationEventHooks::Unsubscribe(SubscriptionIndex index)
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

void ServerEntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world, ecs::SyncBuffers* buffers)
{
	if (!buffers || !buffers->Dirty) return;

	for (unsigned i = 0; i < buffers->ComponentPools.size(); i++) {
		auto const& pool = buffers->ComponentPools[i];
		if (hookedReplicationComponentMask_[i] && pool.size() > 0) {
			for (auto const& entity : pool) {
				OnEntityReplication(world, entity.Key(), *entity.Value().GetBuf(), ecs::ReplicationTypeIndex{(uint16_t)i});
			}
		}
	}
}

void ServerEntityReplicationEventHooks::OnEntityReplication(ecs::EntityWorld& world, EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type)
{
	auto& hooks = hookedReplicationComponents_[(unsigned)type];
	if ((hooks.Fields & fields) == 0) return;

	Array<DeferredEvent> events;

	for (auto index : hooks.GlobalHooks) {
		auto hook = subscriptions_.Find(index);
		if (hook != nullptr && (hook->Fields & fields) != 0) {
			events.push_back({
				.Entity = entity,
				.Fields = fields,
				.Type = type,
				.Hook = *hook
			});
		}
	}

	auto entityHooks = hooks.EntityHooks.try_get(entity);
	if (entityHooks) {
		for (auto index : *entityHooks) {
			auto hook = subscriptions_.Find(index);
			if (hook != nullptr && (hook->Fields & fields) != 0) {
				events.push_back({
					.Entity = entity,
					.Fields = fields,
					.Type = type,
					.Hook = *hook
				});
			}
		}
	}

	if (!events.empty()) {
		esv::LuaServerPin lua(gExtender->GetServer().GetExtensionState());
		if (lua) {
			for (auto const& e : events) {
				CallHandler(e.Entity, e.Fields, e.Type, e.Hook);
			}
		}
	}
}

void ServerEntityReplicationEventHooks::CallHandler(EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type, ReplicationHook const& hook)
{
	auto L = state_.GetState();
	auto componentType = state_.GetEntitySystemHelpers()->GetComponentType(type);
	hook.Hook.Push();
	Ref func(L, lua_absindex(L, -1));

	ProtectedFunctionCaller<std::tuple<EntityHandle, ExtComponentType, uint64_t>, void> caller{ func, std::tuple(entity, *componentType, fields) };
	caller.Call(L, "Entity replication event dispatch");
	lua_pop(L, 1);
}

END_NS()
