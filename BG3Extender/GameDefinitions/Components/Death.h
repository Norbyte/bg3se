#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(death)

struct DeadByDefaultComponent : public BaseComponent
{
    DEFINE_COMPONENT(DeadByDefault, "eoc::death::DeadByDefaultComponent")

    uint8_t DeadByDefault;
};

struct DeathData
{
    EntityHandle Cause;
    EntityHandle CauseOwner;
    Guid CauseUuid;
    stats::DeathType DeathType;
    DamageType MainDamageType;
    bool IsCritical;
    bool InstantKill;
    CauseType CauseType;
    int StoryActionId;
    bool Chasm;
    glm::vec3 ImpactPosition;
    glm::vec3 ImpactDirection;
    uint8_t field_48;
    float ImpactForce;
    Guid Combat;
    uint8_t field_60;
    uint8_t field_61;
    uint8_t field_62;
    bool LeaveTrace;
    std::optional<glm::vec3> ChasmPosition;
};

struct DeathRequestData : public DeathData
{
    uint8_t field_78;
    uint8_t field_79;
    bool SkipDowned;
};

struct ResurrectionData
{
    FixedString Animation;
    float HealthPercentage{ 0.1f };
    bool IsResurrected{ true };
    bool Heal{ true };
};

struct [[bg3::component]] DeathComponent : public DeathData
{
    DEFINE_COMPONENT(Death, "eoc::death::DeathComponent")
};

struct DeathStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(DeathState, "eoc::death::StateComponent")

    uint32_t State;
};

struct DeathTypeComponent : public BaseComponent
{
    DEFINE_COMPONENT(DeathType, "eoc::death::DeathTypeComponent")

    uint8_t DeathType;
};

struct DownedComponent : public BaseComponent
{
    DEFINE_COMPONENT(Downed, "eoc::death::DownedComponent")

    FixedString DownedStatus;
    Guid Entity;
};

END_NS()


BEGIN_NS(esv::death)

struct DelayedDeathComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerDelayDeath, "esv::death::DelayedDeathComponent")

    bg3se::death::DeathRequestData Death;
    uint32_t Reason;
    Guid field_88;
};


struct DelayDeathCauseComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerDelayDeathCause, "esv::death::DelayDeathCauseComponent")

    int DelayCount;
    int Reason;
    Guid field_8;
};

struct KillerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerKiller, "esv::death::KillerComponent")

    HashSet<EntityHandle> Killers;
};

struct StateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerDeathState, "esv::death::StateComponent")

    uint32_t Flags;
};

DEFINE_TAG_COMPONENT(esv::death, DeathContinueComponent, ServerDeathContinue)

struct DeathApplyKnockedOutOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(DeathApplyKnockedOut, "esv::death::ApplyKnockedOutOneFrameComponent")

    EntityHandle field_0;
    uint8_t field_8;
    int field_C;
    char field_10;
};

struct DeathAssignEntityToUserRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(DeathAssignEntityToUserRequest, "esv::death::AssignEntityToUserRequestOneFrameComponent")

    int UserID;
};

struct DeathDeadByDefaultRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(DeathDeadByDefaultRequest, "esv::death::DeadByDefaultRequestOneFrameComponent")

    EntityHandle Entity;
    bool field_8;
    stats::DeathType DeathType;
};

struct DeathRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerDeathRequest, "esv::death::DeathRequestOneFrameComponent")

    EntityHandle Entity;
    bg3se::death::DeathRequestData Death;
};

struct ModifyDelayDeathRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerModifyDelayDeathRequest, "esv::death::ModifyDelayDeathRequestOneFrameComponent")

    EntityHandle Entity;
    bool Increase;
};

struct ResurrectedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerResurrectedEvent, "esv::death::ResurrectedEventOneFrameComponent")

    bg3se::death::ResurrectionData Resurrect;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::death, DownedEventOneFrameComponent, DownedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::death, DiedEventOneFrameComponent, DiedEvent)

END_NS()
