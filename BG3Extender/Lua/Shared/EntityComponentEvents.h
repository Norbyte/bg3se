#pragma once

BEGIN_NS(lua)

enum class EntityComponentEvent
{
	Create = 1 >> 0,
	Destroy = 1 >> 1
};

class EntityComponentEventHooks
{
public:
	using SubscriptionIndex = uint32_t;

	EntityComponentEventHooks(State& state);
	~EntityComponentEventHooks();

	void BindECS();
	SubscriptionIndex Subscribe(ecs::ComponentTypeIndex type, EntityHandle entity, EntityComponentEvent events, RegistryEntry&& hook);
	bool Unsubscribe(SubscriptionIndex index);

private:
	struct ComponentHook
	{
		EntityComponentEvent Events;
		RegistryEntry Hook;
		ecs::ComponentTypeIndex Type;
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

	State& state_;
	BitSet<> hookedComponentMask_;
	Array<ComponentHooks> hookedComponents_;
	SaltedPool<ComponentHook> subscriptions_;
	ecs::EntityWorld* world_{ nullptr };

	void OnEntityEvent(ecs::EntityWorld& world, EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component);
	void CallHandler(EntityHandle entity, ecs::ComponentTypeIndex type, EntityComponentEvent events, void* component, ComponentHook const& hook);
	ComponentHooks& AddComponentType(ecs::ComponentTypeIndex type);

	static void OnComponentCreated(void* object, ecs::ComponentCallbackParams const& params, void* component);
	static void OnComponentDestroyed(void* object, ecs::ComponentCallbackParams const& params, void* component);
};

END_SE()

BEGIN_SE()

MARK_AS_BITFIELD(lua::EntityComponentEvent)

END_SE()
