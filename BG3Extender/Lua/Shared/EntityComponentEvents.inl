#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(lua)

EntityComponentEventHooks::EntityComponentEventHooks(lua::State& state)
	: state_(state)
{}

EntityComponentEventHooks::~EntityComponentEventHooks()
{
	for (uint32_t i = 0; i < hookedComponents_.size(); i++) {
		if (hookedComponentMask_[i]) {
			auto& hooks = hookedComponents_[i];
			auto callbacks = world_->ComponentCallbacks.Get((ecs::ComponentTypeIndex)i);
			callbacks->OnConstruct.Remove(hooks.ConstructRegistrant);
			callbacks->OnDestroy.Remove(hooks.DestructRegistrant);
			hookedComponentMask_.Clear(i);
		}
	}
}

void EntityComponentEventHooks::BindECS()
{
	world_ = state_.GetEntityWorld();
}

void EntityComponentEventHooks::FireDeferredEvents()
{
	for (auto index : deferredUnsubscriptions_) {
		Unsubscribe(index);
	}

	deferredUnsubscriptions_.clear();

	Array<DeferredEvent> events;
	std::swap(events, deferredEvents_);

	auto ecs = state_.GetEntitySystemHelpers();
	
	LuaVirtualPin lua(state_.GetExtensionState());
	if (lua) {
		for (auto& e : events) {
			auto hook = subscriptions_.Find(e.Index);
			if (hook != nullptr) {
				auto componentType = ecs->GetComponentType(e.Type);
				auto component = ecs->GetRawComponent(e.Entity, *componentType);
				CallHandlerUnsafe(e.Entity, e.Type, e.Event, component, *hook, e.Index);
			}
		}
	}
}

void EntityComponentEventHooks::OnComponentCreated(void* object, ecs::ComponentCallbackParams const& params, void* component)
{
	auto self = reinterpret_cast<EntityComponentEventHooks*>((uintptr_t)object & 0x0000ffffffffffffull);
	auto componentType = (ecs::ComponentTypeIndex)((uintptr_t)object >> 48);
	self->OnEntityEvent(*params.World, params.Entity, componentType, EntityComponentEvent::Create, component);
}

void EntityComponentEventHooks::OnComponentDestroyed(void* object, ecs::ComponentCallbackParams const& params, void* component)
{
	auto self = reinterpret_cast<EntityComponentEventHooks*>((uintptr_t)object & 0x0000ffffffffffffull);
	auto componentType = (ecs::ComponentTypeIndex)((uintptr_t)object >> 48);
	self->OnEntityEvent(*params.World, params.Entity, componentType, EntityComponentEvent::Destroy, component);
}

EntityComponentEventHooks::ComponentHooks& EntityComponentEventHooks::AddComponentType(ecs::ComponentTypeIndex type)
{
	auto index = (uint16_t)type;
	if (!hookedComponentMask_[index]) {
		hookedComponentMask_.Set(index);
		while (hookedComponents_.size() <= index) {
			hookedComponents_.push_back(ComponentHooks{});
		}

		auto callbacks = world_->ComponentCallbacks.Get(type);

		auto& hooks = hookedComponents_[index];
		auto self = (void*)((uintptr_t)this | ((uint64_t)index << 48));
		hooks.ConstructRegistrant = callbacks->OnConstruct.Add(ecs::ComponentCallbackHandler{ &OnComponentCreated, self });
		hooks.DestructRegistrant = callbacks->OnDestroy.Add(ecs::ComponentCallbackHandler{ &OnComponentDestroyed, self });
	}

	return hookedComponents_[index];
}

EntityComponentEventHooks::SubscriptionIndex EntityComponentEventHooks::Subscribe(ecs::ComponentTypeIndex type, EntityHandle entity, 
	EntityComponentEvent events, EntityComponentEventFlags flags, RegistryEntry&& hook)
{
	SubscriptionIndex index;
	auto sub = subscriptions_.Add(index);
	sub->Events = events;
	sub->Flags = flags;
	sub->Type = type;
	sub->Hook = std::move(hook);
	sub->Entity = entity;

	auto& pool = AddComponentType(type);
	pool.Events |= events;
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

bool EntityComponentEventHooks::Unsubscribe(SubscriptionIndex index)
{
	auto sub = subscriptions_.Find(index);
	if (sub == nullptr) {
		return false;
	}

	auto& pool = hookedComponents_[(unsigned)sub->Type];
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

void EntityComponentEventHooks::OnEntityEvent(ecs::EntityWorld& world, EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component)
{
	if (!hookedComponentMask_[(unsigned)type]) return;

	auto& hooks = hookedComponents_[(unsigned)type];
	if ((unsigned)(hooks.Events & events) == 0) return;

	for (auto index : hooks.GlobalHooks) {
		auto hook = subscriptions_.Find(index);
		if (hook != nullptr && (unsigned)(hook->Events & events) != 0) {
			if ((unsigned)(hook->Flags & EntityComponentEventFlags::Deferred)) {
				DeferHandler(entity, type, events, index);
			} else {
				CallHandler(entity, type, events, component, *hook, index);
			}
		}
	}

	auto entityHooks = hooks.EntityHooks.try_get(entity);
	if (entityHooks) {
		for (auto index : *entityHooks) {
			auto hook = subscriptions_.Find(index);
			if (hook != nullptr && (unsigned)(hook->Events & events) != 0) {
				if ((unsigned)(hook->Flags & EntityComponentEventFlags::Deferred)) {
					DeferHandler(entity, type, events, index);
				} else {
					CallHandler(entity, type, events, component, *hook, index);
				}
			}
		}
	}
}

void EntityComponentEventHooks::CallHandlerUnsafe(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component, ComponentHook& hook, SubscriptionIndex index)
{
	if ((unsigned)(hook.Flags & EntityComponentEventFlags::Once)) {
		deferredUnsubscriptions_.push_back(index);
		hook.Events = (EntityComponentEvent)0;
	}

	auto L = state_.GetState();
	auto componentType = state_.GetEntitySystemHelpers()->GetComponentType(type);
	hook.Hook.Push();
	Ref func(L, lua_absindex(L, -1));

	RawComponentRef componentRef{ component, type };
	ProtectedFunctionCaller<std::tuple<EntityHandle, ExtComponentType, RawComponentRef>, void> caller{ func, std::tuple(entity, *componentType, componentRef) };
	caller.Call(L, "Component event dispatch");
	lua_pop(L, 1);
}

void EntityComponentEventHooks::CallHandler(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component, ComponentHook& hook, SubscriptionIndex index)
{
	if ((unsigned)(hook.Flags & EntityComponentEventFlags::Once)) {
		deferredUnsubscriptions_.push_back(index);
		hook.Events = (EntityComponentEvent)0;
	}

	LuaVirtualPin lua(state_.GetExtensionState());
	if (lua) {
		CallHandlerUnsafe(entity, type, events, component, hook, index);
	}
}

void EntityComponentEventHooks::DeferHandler(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, SubscriptionIndex index)
{
	deferredEvents_.push_back(DeferredEvent{
		.Entity = entity,
		.Type = type,
		.Event = events,
		.Index = index
	});
}

END_NS()
