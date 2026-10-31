#pragma once

BEGIN_NS(esv::lua)

struct GameStateChangedEventParams : public lua::EventBase
{
    esv::GameState FromState;
    esv::GameState ToState;
};

struct DealDamageEventParams : public lua::EventBase
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

struct DealtDamageEventParams : public DealDamageEventParams
{
    HitResult* Result;
};

struct BeforeDealDamageEventParams : public lua::EventBase
{
    HitDesc* Hit;
    [[bg3::legacy(DamageSums)]] AttackDesc* Attack;
};

struct ExecuteFunctorEventParams : public lua::EventBase
{
    bg3se::stats::Functors* Functor;
    bg3se::stats::ContextData* Params;
};

struct AfterExecuteFunctorEventParams : public lua::EventBase
{
    bg3se::stats::Functors* Functor;
    bg3se::stats::ContextData* Params;
    HitResult* Hit;
};

END_NS()
