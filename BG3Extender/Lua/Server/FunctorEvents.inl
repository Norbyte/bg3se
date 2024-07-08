#pragma once

BEGIN_NS(esv::lua)


FunctorEventHooks::FunctorEventHooks(lua::State& state)
	: state_(state)
{
	auto& hooks = gExtender->GetEngineHooks();
	hooks.stats__DealDamageFunctor__ApplyDamage.SetWrapper(&FunctorEventHooks::OnDealDamage, this);
	hooks.esv__StatsSystem__ThrowDamageEvent.SetWrapper(&FunctorEventHooks::OnEntityDamageEvent, this);
	hooks.stats__Functors__ExecuteType1.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType1, bg3se::stats::Functors::ExecuteType1Proc>, this);
	hooks.stats__Functors__ExecuteType2.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType2, bg3se::stats::Functors::ExecuteType2Proc>, this);
	hooks.stats__Functors__ExecuteType3.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType3, bg3se::stats::Functors::ExecuteType3Proc>, this);
	hooks.stats__Functors__ExecuteType4.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType4, bg3se::stats::Functors::ExecuteType4Proc>, this);
	hooks.stats__Functors__ExecuteType5.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType5, bg3se::stats::Functors::ExecuteType5Proc>, this);
	hooks.stats__Functors__ExecuteType6.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType6, bg3se::stats::Functors::ExecuteType6Proc>, this);
	hooks.stats__Functors__ExecuteType7.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType7, bg3se::stats::Functors::ExecuteType7Proc>, this);
	hooks.stats__Functors__ExecuteType8.SetWrapper(&FunctorEventHooks::OnFunctorExecute<bg3se::stats::FunctorExecParamsType8, bg3se::stats::Functors::ExecuteType8Proc>, this);
}

FunctorEventHooks::~FunctorEventHooks()
{
	auto& hooks = gExtender->GetEngineHooks();
	hooks.stats__DealDamageFunctor__ApplyDamage.ClearHook();
	hooks.esv__StatsSystem__ThrowDamageEvent.ClearHook();
	hooks.stats__Functors__ExecuteType1.ClearHook();
	hooks.stats__Functors__ExecuteType2.ClearHook();
	hooks.stats__Functors__ExecuteType3.ClearHook();
	hooks.stats__Functors__ExecuteType4.ClearHook();
	hooks.stats__Functors__ExecuteType5.ClearHook();
	hooks.stats__Functors__ExecuteType6.ClearHook();
	hooks.stats__Functors__ExecuteType7.ClearHook();
	hooks.stats__Functors__ExecuteType8.ClearHook();
}

HitResult* FunctorEventHooks::OnDealDamage(bg3se::stats::DealDamageFunctor::ApplyDamageProc* next, 
	HitResult* result, bg3se::stats::DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
	ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId,
	int storyActionId, ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr,
	HitDesc* hit, DamageSums* damageSums, EntityHandle* sourceHandle2, HitWith hitWith, int conditionRollIndex,
	bool entityDamagedEventParam, __int64 a17, SpellId* spellId2)
{
	{
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
		evt.DamageSums = damageSums;
		evt.HitWith = hitWith;
		evt.Caster2 = *sourceHandle2;
		evt.SpellId2 = spellId2;
		state_.ThrowEvent("DealDamage", evt, false, 0);
	}

	auto ret = next(result, functor, casterHandle, targetHandle, position, isFromItem, spellId, storyActionId, originator, classResourceMgr, 
		hit, damageSums, sourceHandle2, hitWith, conditionRollIndex, entityDamagedEventParam, a17, spellId2);

	{
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
		evt.DamageSums = damageSums;
		evt.HitWith = hitWith;
		evt.Caster2 = *sourceHandle2;
		evt.SpellId2 = spellId2;
		evt.Result = result;
		state_.ThrowEvent("DealtDamage", evt, false, 0);
	}

	return ret;
}

void FunctorEventHooks::OnEntityDamageEvent(bg3se::stats::StatsSystem_ThrowDamageEventProc* next, void* statsSystem,
	void* temp5, HitDesc* hit, DamageSums* damageAmounts, bool a5, bool a6)
{
	{
		BeforeDealDamageEvent evt;
		evt.Hit = hit;
		evt.DamageSums = damageAmounts;
		state_.ThrowEvent("BeforeDealDamage", evt, false, 0);
	}

	next(statsSystem, temp5, hit, damageAmounts, a5, a6);
}

END_NS()
