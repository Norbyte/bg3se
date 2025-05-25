#pragma once

BEGIN_NS(lua)

class SystemEventHooks
{
public:
    using SubscriptionIndex = uint32_t;

    SystemEventHooks(State& state);
    ~SystemEventHooks();

    void BindECS();
    void FireDeferredEvents();
    std::optional<SubscriptionIndex> Subscribe(ecs::SystemTypeIndex system, RegistryEntry&& hook, bool postUpdate, bool once);
    bool Unsubscribe(SubscriptionIndex index);

private:
    struct SystemHook
    {
        RegistryEntry Hook;
        ecs::SystemTypeIndex System;
        bool PostUpdate;
        bool Once;
        bool Active;
    };

    struct SystemHooks
    {
        Array<SubscriptionIndex> PreUpdateHooks;
        Array<SubscriptionIndex> PostUpdateHooks;
    };

    State& state_;
    ecs::EntitySystemHelpersBase* ecs_{ nullptr };
    BitSet<> hookedSystemMask_;
    Array<SystemHooks> hookedSystems_;
    SaltedPool<SystemHook> subscriptions_;
    Array<SubscriptionIndex> deferredUnsubscriptions_;

    void CallHandler(ecs::SystemTypeIndex type, SystemHook& hook, SubscriptionIndex index);
    void CallHandlerUnsafe(ecs::SystemTypeIndex type, SystemHook& hook, SubscriptionIndex index);

    SystemHooks* TryAddSystemType(ecs::SystemTypeIndex type);
    void OnSystemPreUpdate(ecs::EntitySystemHelpersBase*, BaseSystem*, GameTime const&, ecs::SystemTypeIndex systemType);
    void OnSystemPostUpdate(ecs::EntitySystemHelpersBase*, BaseSystem*, GameTime const&, ecs::SystemTypeIndex systemType);
};

END_SE()
