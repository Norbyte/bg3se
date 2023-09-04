#pragma once

BEGIN_NS(esv::lua)


FunctorEventHooks::FunctorEventHooks(lua::State& state)
	: state_(state)
{
	auto& hooks = gExtender->GetEngineHooks();
	hooks.stats__DealDamageFunctor__ApplyDamage.SetWrapper(&FunctorEventHooks::OnDealDamage, this);
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
	hooks.stats__Functors__ExecuteType1.ClearHook();
	hooks.stats__Functors__ExecuteType2.ClearHook();
	hooks.stats__Functors__ExecuteType3.ClearHook();
	hooks.stats__Functors__ExecuteType4.ClearHook();
	hooks.stats__Functors__ExecuteType5.ClearHook();
	hooks.stats__Functors__ExecuteType6.ClearHook();
	hooks.stats__Functors__ExecuteType7.ClearHook();
	hooks.stats__Functors__ExecuteType8.ClearHook();
}

NewHit* FunctorEventHooks::OnDealDamage(bg3se::stats::DealDamageFunctor::ApplyDamageProc* next, NewHit* result, bg3se::stats::DealDamageFunctor* functor, ecs::EntityRef* casterHandle,
	ecs::EntityRef* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, int storyActionId, 
	ActionOriginator* originator, resource::GuidResourceBankBase* classResourceMgr, Hit* hit, DamageSums* damageSums, uint64_t* unknownThothParam, HitWith hitWith)
{
	DealDamageEvent evt;
	evt.Functor = functor;
	evt.Caster = *casterHandle;
	evt.Target = *targetHandle;
	evt.Position = *position;
	evt.IsFromItem = isFromItem;
	evt.SpellId = spellId;
	evt.StoryActionId = storyActionId;
	evt.Originator = originator;
	evt.Hit = hit;
	evt.DamageSums = damageSums;
	evt.HitWith = hitWith;
	state_.ThrowEvent("DealDamage", evt, false, 0);

	auto ret = next(result, functor, casterHandle, targetHandle, position, isFromItem, spellId, storyActionId, originator, classResourceMgr, hit, damageSums, unknownThothParam, hitWith);
	return ret;
}

END_NS()
