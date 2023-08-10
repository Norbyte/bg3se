#pragma once

BEGIN_NS(esv::lua)


FunctorEventHooks::FunctorEventHooks(lua::State& state)
	: state_(state)
{
	auto& hooks = gExtender->GetEngineHooks();
	hooks.DealDamageFunctor__ApplyDamage.SetWrapper(&FunctorEventHooks::OnDealDamage, this);
	hooks.StatsFunctorSet__ExecuteType1.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType1, StatsFunctorSet::ExecuteType1Proc>, this);
	hooks.StatsFunctorSet__ExecuteType2.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType2, StatsFunctorSet::ExecuteType2Proc>, this);
	hooks.StatsFunctorSet__ExecuteType3.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType3, StatsFunctorSet::ExecuteType3Proc>, this);
	hooks.StatsFunctorSet__ExecuteType4.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType4, StatsFunctorSet::ExecuteType4Proc>, this);
	hooks.StatsFunctorSet__ExecuteType5.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType5, StatsFunctorSet::ExecuteType5Proc>, this);
	hooks.StatsFunctorSet__ExecuteType6.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType6, StatsFunctorSet::ExecuteType6Proc>, this);
	hooks.StatsFunctorSet__ExecuteType7.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType7, StatsFunctorSet::ExecuteType7Proc>, this);
	hooks.StatsFunctorSet__ExecuteType8.SetWrapper(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType8, StatsFunctorSet::ExecuteType8Proc>, this);
}

FunctorEventHooks::~FunctorEventHooks()
{
	auto& hooks = gExtender->GetEngineHooks();
	hooks.DealDamageFunctor__ApplyDamage.ClearHook();
	hooks.StatsFunctorSet__ExecuteType1.ClearHook();
	hooks.StatsFunctorSet__ExecuteType2.ClearHook();
	hooks.StatsFunctorSet__ExecuteType3.ClearHook();
	hooks.StatsFunctorSet__ExecuteType4.ClearHook();
	hooks.StatsFunctorSet__ExecuteType5.ClearHook();
	hooks.StatsFunctorSet__ExecuteType6.ClearHook();
	hooks.StatsFunctorSet__ExecuteType7.ClearHook();
	hooks.StatsFunctorSet__ExecuteType8.ClearHook();
}

NewHit* FunctorEventHooks::OnDealDamage(DealDamageFunctor::ApplyDamageProc* next, NewHit* result, DealDamageFunctor* functor, EntityWorldHandle* casterHandle,
	EntityWorldHandle* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, int storyActionId, 
	ActionOriginator* originator, GuidResourceDefinitionManagerBase* classResourceMgr, Hit* hit, DamageSums* damageSums, uint64_t* unknownThothParam, HitWith hitWith)
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
	state_.ThrowEvent("DealDamage", evt, false, 0, ReadOnlyEvent{});

	auto ret = next(result, functor, casterHandle, targetHandle, position, isFromItem, spellId, storyActionId, originator, classResourceMgr, hit, damageSums, unknownThothParam, hitWith);
	return ret;
}

END_NS()
