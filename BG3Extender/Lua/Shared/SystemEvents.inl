#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(lua)

SystemEventHooks::SystemEventHooks(lua::State& state)
    : state_(state)
{}

SystemEventHooks::~SystemEventHooks()
{
    for (uint32_t i = 0; i < hookedSystems_.size(); i++) {
        if (hookedSystemMask_[i]) {
            ecs_->SetSystemUpdateHook((ecs::SystemTypeIndex)i,{}, {});
            hookedSystemMask_.Clear(i);
        }
    }
}

void SystemEventHooks::BindECS()
{
    ecs_ = state_.GetEntitySystemHelpers();
    hookedSystems_.clear();
    hookedSystems_.resize(ecs_->GetEntityWorld()->Systems.Systems.size());
}

void SystemEventHooks::FireDeferredEvents()
{
    for (auto index : deferredUnsubscriptions_) {
        Unsubscribe(index);
    }

    deferredUnsubscriptions_.clear();
}

SystemEventHooks::SystemHooks* SystemEventHooks::TryAddSystemType(ecs::SystemTypeIndex type)
{
    auto index = (uint16_t)type;
    assert(index < hookedSystems_.size());
    if (!hookedSystemMask_[index]) {
        hookedSystemMask_.Set(index);
        using namespace std::placeholders;
        bool bound = ecs_->SetSystemUpdateHook(type,
            std::bind(&SystemEventHooks::OnSystemPreUpdate, this, _1, _2, _3, _4),
            std::bind(&SystemEventHooks::OnSystemPostUpdate, this, _1, _2, _3, _4));
        if (!bound) return nullptr;
    }

    return &hookedSystems_[index];
}

std::optional<SystemEventHooks::SubscriptionIndex> SystemEventHooks::Subscribe(ecs::SystemTypeIndex system, RegistryEntry&& hook, bool postUpdate, bool once)
{
    SubscriptionIndex index;
    auto sub = subscriptions_.Add(index);
    sub->Hook = std::move(hook);
    sub->System = system;
    sub->PostUpdate = postUpdate;
    sub->Once = once;
    sub->Active = true;

    auto pool = TryAddSystemType(system);
    if (!pool) return {};

    if (postUpdate) {
        pool->PostUpdateHooks.Add(index);
    } else {
        pool->PreUpdateHooks.Add(index);
    }

    return index;
}

bool SystemEventHooks::Unsubscribe(SubscriptionIndex index)
{
    auto sub = subscriptions_.Find(index);
    if (sub == nullptr) {
        return false;
    }

    auto& pool = hookedSystems_[(unsigned)sub->System];
    if (sub->PostUpdate) {
        for (unsigned i = 0; i < pool.PostUpdateHooks.size(); i++) {
            if (pool.PostUpdateHooks[i] == index) {
                pool.PostUpdateHooks.ordered_remove_at(i);
                break;
            }
        }
    } else {
        for (unsigned i = 0; i < pool.PreUpdateHooks.size(); i++) {
            if (pool.PreUpdateHooks[i] == index) {
                pool.PreUpdateHooks.ordered_remove_at(i);
                break;
            }
        }
    }

    subscriptions_.Free(index);
    return true;
}

void SystemEventHooks::OnSystemPreUpdate(ecs::EntitySystemHelpersBase*, BaseSystem*, GameTime const&, ecs::SystemTypeIndex systemType)
{
    assert(hookedSystemMask_[(unsigned)systemType]);
    auto& hooks = hookedSystems_[(unsigned)systemType];

    for (auto index : hooks.PreUpdateHooks) {
        auto hook = subscriptions_.Find(index);
        if (hook != nullptr) {
            CallHandler(systemType, *hook, index);
        }
    }
}

void SystemEventHooks::OnSystemPostUpdate(ecs::EntitySystemHelpersBase*, BaseSystem*, GameTime const&, ecs::SystemTypeIndex systemType)
{
    assert(hookedSystemMask_[(unsigned)systemType]);
    auto& hooks = hookedSystems_[(unsigned)systemType];

    for (auto index : hooks.PostUpdateHooks) {
        auto hook = subscriptions_.Find(index);
        if (hook != nullptr) {
            CallHandler(systemType, *hook, index);
        }
    }
}

void SystemEventHooks::CallHandlerUnsafe(ecs::SystemTypeIndex type, SystemHook& hook, SubscriptionIndex index)
{
    auto L = state_.GetState();
    hook.Hook.Push(L);
    Ref func(L, lua_absindex(L, -1));

    ProtectedFunctionCaller<std::tuple<>, void> caller{ func, std::tuple() };
    caller.Call(L, "System update event dispatch");
    lua_pop(L, 1);
}

void SystemEventHooks::CallHandler(ecs::SystemTypeIndex type, SystemHook& hook, SubscriptionIndex index)
{
    if (!hook.Active) return;

    if (hook.Once) {
        deferredUnsubscriptions_.push_back(index);
        hook.Active = false;
    }

    LuaVirtualPin lua(state_.GetExtensionState());
    if (lua) {
        CallHandlerUnsafe(type, hook, index);
    }
}

END_NS()
