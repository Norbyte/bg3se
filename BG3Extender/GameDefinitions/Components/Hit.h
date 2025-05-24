#pragma once

BEGIN_NS(hit)

struct TargetInfo
{
    FixedString SpellId;
    FixedString field_4;
    uint8_t field_8;
    glm::vec3 ImpactPosition;
    glm::vec3 ImpactDirection;
    int TotalDamageDone;
    int ArmorAbsorption;
    uint32_t EffectFlags;
    DamageType MainDamageType;
    CauseType CauseType;
    HitWith HitWith;
    Array<DamagePair> Damage;
    uint8_t AttackFlags;
    Array<bool> ConditionRollResults;
    HashSet<uint8_t> ResistanceTypes;
    Guid SpellCastGuid;
    uint8_t field_A0;
    uint8_t field_A1;
    uint8_t field_A2;
    uint8_t field_A3;
};


struct TargetComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitTarget, "eoc::hit::TargetComponent")

    EntityHandle field_0;
    EntityHandle field_8;
    TargetInfo Target;
};


struct AttackerComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitAttacker, "eoc::hit::AttackerComponent")

    EntityHandle Attacker;
};


struct WeaponComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitWeapon, "eoc::hit::WeaponComponent")

    EntityHandle Weapon;
};


struct ThrownObjectComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitThrownObject, "eoc::hit::ThrownObjectComponent")

    EntityHandle ThrownObject;
};


struct ProxyComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitProxy, "eoc::hit::ProxyComponent")

    EntityHandle Owner;
    FixedString field_8;
};


struct ProxyOwnerComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitProxyOwner, "eoc::hit::ProxyOwnerComponent")

    Array<EntityHandle> Owners;
};


struct ReactionSource
{
    EntityHandle field_0;
    TargetInfo Target;
};

struct ReactionComponent : public BaseComponent
{
    DEFINE_COMPONENT(HitReaction, "eoc::hit::ReactionComponent")

    HashMap<EntityHandle, ReactionSource> Reactions;
};

END_NS()

BEGIN_NS(esv::hit)

struct AnimationEventRequestsData
{
    uint8_t HitAnimationType;
    uint8_t field_1;
    bool field_2;
    glm::vec3 field_4;
    glm::vec3 field_10;
    glm::vec3 field_1C;
    EntityHandle field_28;
};

struct HitAnimationRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HitAnimationRequest, "esv::hit::HitAnimationRequestOneFrameComponent")

    Array<AnimationEventRequestsData> Requests;
};

struct HitResultEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HitResultEvent, "esv::hit::HitResultEventOneFrameComponent")

    [[bg3::legacy(field_0)]] ActionOriginator Originator;
    [[bg3::legacy(field_20)]] HitDesc Hit;
    uint8_t field_1D0;
    EntityHandle Target;
    bool Lethal;
    bool ShouldBeDowned;
    int AC;
    uint8_t field_1E8;
};

struct OnHitJoinCombatRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HitJoinCombatRequest, "esv::hit::OnHitJoinCombatRequestOneFrameComponent")

    HashSet<EntityHandle> Entities;
};

struct HitRequest
{
    EntityHandle Target;
    EntityHandle TargetProxy;
    HitDesc Desc;
    HitWith HitWith{ HitWith::None };
    int StoryActionId{ -1 };
    ActionOriginator Originator;
    AbilityId Ability{ AbilityId::None };
    EntityHandle Weapon;
    bool FromFunctor{ false };
    bool field_1F9{ false };
    uint8_t field_1FA{ 0 };
};

struct ReactionRequest
{
    EntityHandle Target;
    EntityHandle TargetProxy;
    HitDesc Hit;
};

struct HitSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerHit, "esv::hit::HitSystem")

    [[bg3::hidden]] void* GameEventListenerVMT;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* GlobalSwitches;
    [[bg3::hidden]] void* StoryCharacterEventManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* AnimationBlueprintSystem;
    [[bg3::hidden]] void* ThothMachine;
    Array<HitRequest> Hits;
    Array<ReactionRequest> Reactions;
    Array<EntityHandle> HitAnimationFinished;
    uint8_t field_80;
    [[bg3::hidden]] UnknownFunction field_88;
    [[bg3::hidden]] void* HitHelper;
};

struct HitEventData
{
    Guid Inflicter;
    Guid InflicterOwner;
    Guid TargetProxy;
    char DamageType;
    int Damage;
    BYTE CauseType;
    int StoryActionId;
};

struct UnresolvedHitNotificationComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(UnresolvedHitNotification, "esv::hit::UnresolvedHitNotificationComponent")

    Array<HitEventData> Hits;
};

struct HitNotificationEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HitNotification, "esv::hit::HitNotificationEventOneFrameComponent")

    EntityHandle Target;
    HitEventData Hit;
    bool Dead;
};

struct HitNotificationRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(HitNotificationRequest, "esv::hit::HitNotificationRequestOneFrameComponent")

    EntityHandle Target;
    HitEventData Hit;
};


END_NS()
