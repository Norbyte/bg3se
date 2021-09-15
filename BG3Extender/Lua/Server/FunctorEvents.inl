#pragma once

BEGIN_NS(esv::lua)


FunctorEventHooks::FunctorEventHooks(lua::State& state)
	: state_(state)
{
	auto& hooks = gExtender->GetEngineHooks();
	using namespace std::placeholders;
	hooks.DealDamageFunctor__ApplyDamage.SetWrapper(std::bind(&FunctorEventHooks::OnDealDamage, this, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15));
	hooks.StatsFunctorSet__ExecuteType1.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType1, StatsFunctorSet::ExecuteType1Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType2.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType2, StatsFunctorSet::ExecuteType2Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType3.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType3, StatsFunctorSet::ExecuteType3Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType4.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType4, StatsFunctorSet::ExecuteType4Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType5.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType5, StatsFunctorSet::ExecuteType5Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType6.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType6, StatsFunctorSet::ExecuteType6Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType7.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType7, StatsFunctorSet::ExecuteType7Proc>, this, _1, _2, _3, _4));
	hooks.StatsFunctorSet__ExecuteType8.SetWrapper(std::bind(&FunctorEventHooks::OnFunctorExecute<FunctorExecParamsType8, StatsFunctorSet::ExecuteType8Proc>, this, _1, _2, _3, _4));
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
