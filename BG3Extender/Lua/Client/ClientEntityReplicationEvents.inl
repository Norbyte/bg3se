#include <Lua/Shared/LuaMethodCallHelpers.h>
#include <Lua/Client/ClientEntityReplicationEvents.h>

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

DummyFieldTracker::~DummyFieldTracker() {}

void DummyFieldTracker::Add(EntityHandle entity, EntityHandle entity2, void* component)
{
	if (Base) Base->Add(entity, entity2, component);

	auto change = SharedChanges->ChangedEntities.try_get(entity);
	if (!change) {
		SharedChanges->ChangedEntities.set(entity, SharedChange{FieldMask, component});
	} else if (!(change->FieldMask & FieldMask)) {
		change->FieldMask |= FieldMask;
	}
}

void DummyFieldTracker::FireEvents()
{
	if (Base) Base->FireEvents();

	for (auto& it : SharedChanges->ChangedEntities) {
		PerComponentChangeEvent.Invoke(it.Key(), it.Value().Component, it.Value().FieldMask);
	}

	SharedChanges->ChangedEntities.clear();
}

void DummyFieldTracker::Remove(EntityHandle entity)
{
	if (Base) Base->Remove(entity);

	SharedChanges->ChangedEntities.remove(entity);
}

ClientEntityReplicationEventHooks::ClientEntityReplicationEventHooks(State& state)
	: state_(state)
{}

ClientEntityReplicationEventHooks::~ClientEntityReplicationEventHooks()
{
	for (uint32_t i = 0; i < hookedReplicationComponentMask_.Size; i++) {
		RemoveComponentType((ecs::ReplicationTypeIndex)i);
	}
}

EntityReplicationEventHooks::ReplicationHooks* ClientEntityReplicationEventHooks::AddComponentType(ecs::ReplicationTypeIndex type)
{
	auto index = (unsigned)type;
	if (!hookedReplicationComponentMask_[index]) {
		auto& peer = GetStaticSymbols().GetEoCClient()->GameClient->ReplicationPeer;
		auto deserializer = peer.Deserializers.try_get((uint16_t)type);
		if (!deserializer) {
			auto name = state_.GetEntitySystemHelpers()->GetComponentName(type);
			ERR("Replication component '%s' has no deserializer, cannot hook", name->c_str());
			return nullptr;
		}

		auto& trackers = (*deserializer)->FieldTrackers;
		auto sharedState = GameAlloc<DummyFieldTracker::SharedChangeSet>();

		if (trackers.empty()) {
			for (uint32_t i = 0; i < 32; i++) {
				auto tracker = GameAlloc<DummyFieldTracker>();
				tracker->SharedChanges = sharedState;
				tracker->FieldMask = (1ull << i);
				tracker->PerComponentChangeEvent.Add(MakeFunction(&ClientEntityReplicationEventHooks::OnEntityReplication, this, type));
				trackers.push_back(tracker);
			}
		} else {
			// Replace original trackers with a dummy; we need these to be able to 
			// 'read' entity handles from FieldChangeData without handling different field sizes
			for (uint32_t i = 0; i < trackers.size(); i++) {
				auto base = trackers.try_get(i);
				if (!sharedState) {
					sharedState = GameAlloc<DummyFieldTracker::SharedChangeSet>();
				}

				auto tracker = GameAlloc<DummyFieldTracker>();
				tracker->SharedChanges = sharedState;
				tracker->Base = base ? *base : nullptr;
				tracker->FieldMask = (1ull << i);
				tracker->PerComponentChangeEvent.Add(MakeFunction(&ClientEntityReplicationEventHooks::OnEntityReplication, this, type));
				trackers.set(i, tracker);
			}
		}

		hookedReplicationComponentMask_.Set(index);
		while (hookedReplicationComponents_.size() <= index) {
			hookedReplicationComponents_.push_back(ReplicationHooks{});
		}
	}

	return &hookedReplicationComponents_[index];
}

void ClientEntityReplicationEventHooks::RemoveComponentType(ecs::ReplicationTypeIndex type)
{
	auto index = (unsigned)type;
	if (!hookedReplicationComponentMask_[index]) return;

	auto& peer = GetStaticSymbols().GetEoCClient()->GameClient->ReplicationPeer;
	auto& trackers = peer.Deserializers[(uint16_t)type]->FieldTrackers;

	// Restore original trackers
	for (uint32_t i = 0; i < trackers.size(); i++) {
		auto tracker = trackers[i];
		if (tracker) {
			auto base = static_cast<DummyFieldTracker*>(tracker)->Base;
			if (base) {
				trackers.set(i, base);
			} else {
				trackers.clear(i);
			}

			GameDelete(tracker);
		}
	}

	hookedReplicationComponents_[index] = ReplicationHooks{};
	hookedReplicationComponentMask_.Clear(index);
}

std::optional<EntityReplicationEventHooks::SubscriptionIndex> ClientEntityReplicationEventHooks::Subscribe(ecs::ReplicationTypeIndex type, EntityHandle entity, uint64_t fields, RegistryEntry&& hook)
{
	auto pool = AddComponentType(type);
	if (!pool) return {};

	SubscriptionIndex index;
	auto sub = subscriptions_.Add(index);
	sub->Fields = fields;
	sub->Hook = std::move(hook);
	sub->Type = type;
	sub->Entity = entity;

	pool->Fields |= fields;
	if (!entity) {
		pool->GlobalHooks.Add(index);
	} else {
		auto entityHooks = pool->EntityHooks.try_get(entity);
		if (entityHooks) {
			entityHooks->push_back(index);
		} else {
			entityHooks = pool->EntityHooks.add_key(entity);
			entityHooks->push_back(index);
		}
	}

	return index;
}

bool ClientEntityReplicationEventHooks::Unsubscribe(SubscriptionIndex index)
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
		auto hookMap = pool.EntityHooks.find(sub->Entity);
		if (hookMap) {
			auto& hooks = hookMap.Value();
			for (unsigned i = 0; i < hooks.size(); i++) {
				if (hooks[i] == index) {
					hooks.ordered_remove_at(i);
					break;
				}
			}

			if (hooks.empty()) {
				pool.EntityHooks.remove(hookMap);
			}
		}
	}

	if (pool.GlobalHooks.empty() && pool.EntityHooks.empty()) {
		RemoveComponentType(sub->Type);
	}

	subscriptions_.Free(index);
	return true;
}

void ClientEntityReplicationEventHooks::PostUpdate()
{
	if (!pendingEvents_.empty()) {
		ecl::LuaClientPin lua(gExtender->GetClient().GetExtensionState());
		if (lua) {
			for (auto const& e : pendingEvents_) {
				CallHandler(e.Entity, e.Fields, e.Type, e.Hook);
			}
		}
	}

	pendingReplications_.clear();
	pendingEvents_.clear();
}

void ClientEntityReplicationEventHooks::OnEntityReplication(ecs::ReplicationTypeIndex type, EntityHandle entity, void* component, uint64_t fields)
{
	auto& hooks = hookedReplicationComponents_[(unsigned)type];
	if ((hooks.Fields & fields) == 0) return;

	ComponentRef ref{ entity, type };
	if (pendingReplications_.contains(ref)) return;

	pendingReplications_.insert(ref);

	for (auto index : hooks.GlobalHooks) {
		auto hook = subscriptions_.Find(index);
		if (hook != nullptr && (hook->Fields & fields) != 0) {
			pendingEvents_.push_back({
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
				pendingEvents_.push_back({
					.Entity = entity,
					.Fields = fields,
					.Type = type,
					.Hook = *hook
				});
			}
		}
	}
}

void ClientEntityReplicationEventHooks::CallHandler(EntityHandle entity, uint64_t fields, ecs::ReplicationTypeIndex type, ReplicationHook const& hook)
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
