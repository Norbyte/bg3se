#pragma once

BEGIN_NS(lua)

enum class EntityComponentEvent
{
	Create = 1 << 0,
	Destroy = 1 << 1
};

enum class EntityComponentEventFlags
{
	Deferred = 1 << 0,
	Once = 1 << 1
};

class EntityComponentEventHooks
{
public:
	using SubscriptionIndex = uint32_t;

	EntityComponentEventHooks(State& state);
	~EntityComponentEventHooks();

	void BindECS();
	void FireDeferredEvents();
	SubscriptionIndex Subscribe(ecs::ComponentTypeIndex type, EntityHandle entity, EntityComponentEvent events, 
		EntityComponentEventFlags flags, RegistryEntry&& hook);
	bool Unsubscribe(SubscriptionIndex index);
	void OnEntityEvent(ecs::EntityWorld& world, EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component);

private:
	struct ComponentHook
	{
		EntityComponentEvent Events;
		EntityComponentEventFlags Flags;
		ecs::ComponentTypeIndex Type;
		RegistryEntry Hook;
		EntityHandle Entity;
	};

	struct ComponentHooks
	{
		EntityComponentEvent Events;
		Array<SubscriptionIndex> GlobalHooks;
		HashMap<EntityHandle, Array<SubscriptionIndex>> EntityHooks;
		uint64_t ConstructRegistrant{ 0 };
		uint64_t DestructRegistrant{ 0 };
	};

	struct DeferredEvent
	{
		EntityHandle Entity;
		ecs::ComponentTypeIndex Type;
		EntityComponentEvent Event;
		SubscriptionIndex Index;
	};

	State& state_;
	BitSet<> hookedComponentMask_;
	Array<ComponentHooks> hookedComponents_;
	SaltedPool<ComponentHook> subscriptions_;
	ecs::EntityWorld* world_{ nullptr };
	Array<DeferredEvent> deferredEvents_;
	Array<SubscriptionIndex> deferredUnsubscriptions_;

	void CallHandler(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component, ComponentHook& hook, SubscriptionIndex index);
	void CallHandlerUnsafe(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component, ComponentHook& hook, SubscriptionIndex index);
	void DeferHandler(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, SubscriptionIndex index);
	ComponentHooks& AddComponentType(ecs::ComponentTypeIndex type);

	static void OnComponentCreated(void* object, ecs::ComponentCallbackParams const& params, void* component);
	static void OnComponentDestroyed(void* object, ecs::ComponentCallbackParams const& params, void* component);
};

END_SE()

BEGIN_SE()

MARK_AS_BITFIELD(lua::EntityComponentEvent)

END_SE()
