#pragma once

BEGIN_NS(esv::lua)

template <class TParams>
void LuaTriggerFunctorPreExecEvent(bg3se::stats::Functors* self, TParams* params)
{
    LuaServerPin lua(gExtender->GetServer().GetExtensionState());
    if (lua) {
        ExecuteFunctorEvent evt{
            .Functor = self,
            .Params = params
        };
        lua->ThrowEvent("ExecuteFunctor", evt, false, 0);
    }
}

template <class TParams>
void LuaTriggerFunctorPostExecEvent(bg3se::stats::Functors* self, TParams* params, HitResult* hit)
{
    LuaServerPin lua(gExtender->GetServer().GetExtensionState());
    if (lua) {
        AfterExecuteFunctorEvent evt{ 
            .Functor = self, 
            .Params = params, 
            .Hit = hit
        };
        lua->ThrowEvent("AfterExecuteFunctor", evt, false, 0);
    }
}

template <class TParams>
void FunctorEventHooks::OnFunctorExecute(bg3se::stats::ExecuteFunctorProc<TParams>* next, HitResult* hit, bg3se::stats::Functors* self, TParams* params)
{
    LuaTriggerFunctorPreExecEvent<TParams>(self, params);
    next(hit, self, params);
    LuaTriggerFunctorPostExecEvent<TParams>(self, params, hit);
}

void FunctorEventHooks::OnInterruptFunctorExecute(bg3se::stats::ExecuteInterruptFunctorProc* next, HitResult* hit, ecs::EntityWorld* world, bg3se::stats::Functors* self, bg3se::stats::InterruptContextData* params)
{
    LuaTriggerFunctorPreExecEvent(self, params);
    next(hit, world, self, params);
    LuaTriggerFunctorPostExecEvent(self, params, hit);
}

FunctorEventHooks::FunctorEventHooks()
{
    auto& hooks = gExtender->GetEngineHooks();
    hooks.stats__DealDamageFunctor__ApplyDamage.SetWrapper(&FunctorEventHooks::OnDealDamage, this);
    hooks.esv__StatsSystem__ThrowDamageEvent.SetWrapper(&FunctorEventHooks::OnEntityDamageEvent, this);

    hooks.esv__ExecuteStatsFunctor_AttackTargetContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::AttackTargetContextData>, this);
    hooks.esv__ExecuteStatsFunctor_AttackPositionContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::AttackPositionContextData>, this);
    hooks.esv__ExecuteStatsFunctor_MoveContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::MoveContextData>, this);
    hooks.esv__ExecuteStatsFunctor_TargetContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::TargetContextData>, this);
    hooks.esv__ExecuteStatsFunctor_NearbyAttackedContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::NearbyAttackedContextData>, this);
    hooks.esv__ExecuteStatsFunctor_NearbyAttackingContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::NearbyAttackingContextData>, this);
    hooks.esv__ExecuteStatsFunctor_EquipContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::EquipContextData>, this);
    hooks.esv__ExecuteStatsFunctor_SourceContext.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::SourceContextData>, this);
    hooks.esv__ExecuteStatsFunctor_InterruptContext.SetWrapper(&FunctorEventHooks::OnInterruptFunctorExecute, this);
}

FunctorEventHooks::~FunctorEventHooks()
{
    auto& hooks = gExtender->GetEngineHooks();
    hooks.stats__DealDamageFunctor__ApplyDamage.ClearHook();
    hooks.esv__StatsSystem__ThrowDamageEvent.ClearHook();

    hooks.esv__ExecuteStatsFunctor_AttackTargetContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_AttackPositionContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_MoveContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_TargetContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_NearbyAttackedContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_NearbyAttackingContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_EquipContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_SourceContext.ClearHook();
    hooks.esv__ExecuteStatsFunctor_InterruptContext.ClearHook();
}

HitResult* FunctorEventHooks::OnDealDamage(bg3se::stats::DealDamageFunctor::ApplyDamageProc* next, 
    HitResult* result, bg3se::stats::DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
    ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId,
    int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr,
    HitDesc* hit, AttackDesc* attack, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
    bool entityDamagedEventParam, __int64 a17, SpellId* spellId2)
{
    {
        LuaServerPin lua(gExtender->GetServer().GetExtensionState());
        if (lua) {
            DealDamageEvent evt;
            evt.Functor = functor;
            evt.Caster = casterHandle->Handle;
            evt.Target = targetHandle->Handle;
            evt.Position = *position;
            evt.IsFromItem = isFromItem;
            evt.SpellId = spellId;
            evt.StoryActionId = storyActionId;
            evt.Originator = originator;
            evt.Hit = hit;
            evt.Attack = attack;
            evt.HitWith = hitWith;
            evt.Caster2 = *sourceHandle2;
            evt.SpellId2 = spellId2;
            lua->ThrowEvent("DealDamage", evt, false, 0);
        }
    }

    auto ret = next(result, functor, casterHandle, targetHandle, position, isFromItem, spellId, storyActionId, originator, classResourceMgr, 
        hit, attack, sourceHandle2, hitWith, conditionRollIndex, entityDamagedEventParam, a17, spellId2);

    {
        LuaServerPin lua(gExtender->GetServer().GetExtensionState());
        if (lua) {
            DealtDamageEvent evt;
            evt.Functor = functor;
            evt.Caster = casterHandle->Handle;
            evt.Target = targetHandle->Handle;
            evt.Position = *position;
            evt.IsFromItem = isFromItem;
            evt.SpellId = spellId;
            evt.StoryActionId = storyActionId;
            evt.Originator = originator;
            evt.Hit = hit;
            evt.Attack = attack;
            evt.HitWith = hitWith;
            evt.Caster2 = *sourceHandle2;
            evt.SpellId2 = spellId2;
            evt.Result = result;
            lua->ThrowEvent("DealtDamage", evt, false, 0);
        }
    }

    return ret;
}

void FunctorEventHooks::OnEntityDamageEvent(bg3se::stats::StatsSystem_ThrowDamageEventProc* next, void* statsSystem,
    void* temp5, HitDesc* hit, AttackDesc* attack, bool a5, bool a6)
{
    {
        LuaServerPin lua(gExtender->GetServer().GetExtensionState());
        if (lua) {
            BeforeDealDamageEvent evt;
            evt.Hit = hit;
            evt.Attack = attack;
            lua->ThrowEvent("BeforeDealDamage", evt, false, 0);
        }
    }

    next(statsSystem, temp5, hit, attack, a5, a6);
}

END_NS()
