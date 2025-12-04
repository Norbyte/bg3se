#include <GameDefinitions/Components/Effect.h>

BEGIN_SE()

MaterialInfo* sDummyMaterialInfo{ nullptr };

void EffectsManager::Invoke(EffectInvoke invoke)
{
    EnterCriticalSection(&InvokesCS);
    Invokes.push_back(std::move(invoke));
    LeaveCriticalSection(&InvokesCS);
}

void EffectsManager::AddMaterial(OverlayMaterialRequest material)
{
    material.Material = &sDummyMaterialInfo;
    EnterCriticalSection(&AddOverlayMaterialRequestsCS);
    OverlayMaterialRequests.push_back(std::move(material));
    LeaveCriticalSection(&AddOverlayMaterialRequestsCS);
}

END_SE()

BEGIN_NS(ecl::effect)

HandlerComponentView::HandlerComponentView(EntityHandle entity)
    : Entity(entity)
{
    auto& ecs = gExtender->GetClient().GetEntityHelpers();
    EffectHandler = ecs.GetComponent<HandlerComponent>(entity);
}

EntityHandle HandlerSystem::InitMultiEffect(EffectHandlerInitInfo info)
{
    ERR("InitMultiEffect(): Disabled for now as AddImmediateDefaultComponent() is cooked on effects");
    return {};

    auto& ecs = gExtender->GetClient().GetEntityHelpers();

    auto sceneRoot = ecs.GetSingletonEntity<SceneComponent>();
    auto scene = ecs.GetComponent<SceneComponent>(sceneRoot);

    auto effect = ecs.GetEntityWorld()->Deferred()->CreateEntityImmediate();
    auto ops = ecs.GetComponentOps<HandlerComponent>();
    ops->AddImmediateDefaultComponent(effect.Handle, 0);

    HandlerComponentView handler(effect);
    Helper->InitMultiEffect(handler, info, (Scene*)scene);
    return effect;
}

bool HandlerSystem::UpdateMultiEffect(EntityHandle effect)
{
    HandlerComponentView handler(effect);
    if (!handler.EffectHandler) return false;

    Helper->UpdateMultiEffect(handler);
    return true;
}

bool HandlerSystem::DestroyMultiEffect(EntityHandle effect)
{
    HandlerComponentView handler(effect);
    if (!handler.EffectHandler) return false;

    Helper->DestroyMultiEffect(handler);
    return true;
}

bool HandlerSystem::HideMultiEffect(EntityHandle effect)
{
    HandlerComponentView handler(effect);
    if (!handler.EffectHandler) return false;

    Helper->HideMultiEffect(handler);
    return true;
}

bool HandlerSystem::UnhideMultiEffect(EntityHandle effect)
{
    HandlerComponentView handler(effect);
    if (!handler.EffectHandler) return false;

    Helper->UnhideMultiEffect(handler);
    return true;
}

END_NS()
