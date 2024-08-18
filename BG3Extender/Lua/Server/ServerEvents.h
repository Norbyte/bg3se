#pragma once

BEGIN_NS(esv::lua)

struct GameStateChangedEvent : public lua::EventBase
{
	esv::GameState FromState;
	esv::GameState ToState;
};

struct DealDamageEvent : public lua::EventBase
{
	bg3se::stats::DealDamageFunctor* Functor;
	EntityHandle Caster;
	EntityHandle Target;
	glm::vec3 Position;
	bool IsFromItem;
	SpellIdWithPrototype* SpellId;
	int StoryActionId;
	ActionOriginator* Originator;
	HitDesc* Hit;
	[[bg3::legacy(DamageSums)]] AttackDesc* Attack;
	HitWith HitWith;
	EntityHandle Caster2;
	bg3se::SpellId* SpellId2;
};

struct DealtDamageEvent : public esv::lua::DealDamageEvent
{
	HitResult* Result;
};

struct BeforeDealDamageEvent : public lua::EventBase
{
	HitDesc* Hit;
	[[bg3::legacy(DamageSums)]] AttackDesc* Attack;
};

struct ExecuteFunctorEvent : public lua::EventBase
{
	bg3se::stats::Functors* Functor;
	bg3se::stats::ContextData* Params;
};

struct AfterExecuteFunctorEvent : public lua::EventBase
{
	bg3se::stats::Functors* Functor;
	bg3se::stats::ContextData* Params;
	HitResult* Hit;
};

END_NS()
