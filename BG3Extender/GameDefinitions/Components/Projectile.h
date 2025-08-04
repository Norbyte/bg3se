#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Components/SpellCastShared.h>

BEGIN_NS(projectile)

struct ProjectileResult : public path::Settings
{
    [[bg3::legacy(field_50)]] FixedString TemplateId;
    EntityHandle field_58;
    glm::vec3 field_60;
    float InterpolateValue{ .0f };
};

struct SourceInfoComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProjectileSource, "eoc::projectile::SourceInfoComponent")

    SpellId Spell;
    EntityHandle Entity;
};

END_NS()

BEGIN_NS(esv::projectile)

struct OnHitActionBase : public ProtectedGameObject<OnHitActionBase>
{
    [[bg3::hidden]] void* VMT;
    EntityHandle Caster;
    bool IsFromItem;
    bool IgnoreTargetChecks;
    SpellId Spell;
};

struct PierceOnHit : public OnHitActionBase
{
    int PierceCount;
    glm::vec3 Direction;
    Array<EntityHandle> HitTargets;
};

struct ForkOnHit : public OnHitActionBase
{
    int ForkCount;
    int ForkLevels;
    Array<EntityHandle> HitTargets;
};

struct AttachmentComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerProjectileAttachment, "esv::projectile::AttachmentComponent")

    EntityHandle Attachment;
};

struct SpellComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerProjectileSpell, "esv::projectile::SpellComponent")

    EntityHandle Spell;
    bg3se::spell_cast::IntermediateTarget Target;
    int Index;
};

struct InitializationData
{
    FixedString Projectile;
    glm::vec3 StartPosition;
    glm::vec3 TargetPosition;
    int field_1C;
    SpellId Spell;
    Guid SpellCastUuid;
    bool IsFromItem;
    bool IgnoreTargetChecks;
    EntityHandle Caster;
    EntityHandle Source;
    EntityHandle BeamSource;
    EntityHandle SourceWeapon;
    Guid HistoryUuid;
    bool MainDamageType;
    AbilityId SpellCastingAbility;
    bool IsTrap;
    [[bg3::legacy(field_93)]] bool MovesProjectileFromCaster;
    bool CanDeflect;
    bool IgnoreObjects;
    CauseType CauseType;
    int StoryActionId;
    ActionOriginator Originator;
    FixedString TextKey;
    HitDesc Hit;
    std::optional<glm::vec3> BeamTargetPos;
    EntityHandle TargetObject;
    float ExplodeRadius;
    bool NoMovement;
    OnHitActionBase* HitAction;
    EntityHandle ThrownObject;
    bool DamageMovingObjectOnLand;
    std::optional<bg3se::projectile::ProjectileResult> Path;
    std::optional<path::Settings> MovementSettings;
    EntityHandle MovingObject;
    SpellComponent SpellData;
};

struct InitializationComponent : public BaseProxyComponent
{
    DEFINE_COMPONENT(ServerProjectileInitialization, "esv::projectile::InitializationComponent")

    [[bg3::hidden]] void* field_0;
    InitializationData Data;
    [[bg3::hidden]] void* _PAD;
};


struct Projectile : public BaseProxyComponent
{
    DEFINE_COMPONENT(ServerProjectile, "esv::Projectile")

    [[bg3::hidden]] void* VMT;
    uint32_t field_8;
    uint32_t field_C;
    EntityHandle Entity;
    EntityHandle Caster;
    EntityHandle Source;
    EntityHandle TargetObject;
    EntityHandle HitObject;
    EntityHandle ClientHitObject;
    EntityHandle BeamSource;
    EntityHandle SourceWeapon;
    glm::vec3 SourcePos;
    glm::vec3 TargetPos;
    glm::vec3 TargetObjectPos;
    FixedString TextKey;
    [[bg3::legacy(Hit)]] HitDesc SpellRollResult;
    DamageType MainDamageType;
    CauseType Cause;
    float LifeTime;
    float HitInterpolation;
    float FallbackTimer;
    path::PathMover Path;
    path::PathMover Path2;
    float ExplodeRadius;
    [[bg3::legacy(ExplodeRadius2)]] float InfluenceRadius;
    SpellId Spell;
    stats::Functors* HitFunctors;
    AbilityId SpellCastingAbility;
    Guid SpellCastUuid;
    EntityHandle MovingObject;
    Transform MovingObjectTransform;
    FixedString SpawnEffect;
    bool SpawnFXOverridesImpactFX;
    EntityHandle BeamEffect;
    bool RequestDelete;
    bool Launched;
    bool IsOnHold;
    bool IsTrap;
    bool IsThrown;
    [[bg3::legacy(MoveFromOwner)]] bool MoveFromSource;
    bool IsFromItem;
    bool IgnoreTargetChecks;
    bool IgnoreRoof;
    bool CanDeflect;
    bool IgnoreObjects;
    bool IgnoreSurfaces;
    bool Used;
    bool Success;
    uint8_t field_526;
    bool DamageMovingObjectOnLand;
    [[bg3::legacy(NoMovement)]] bool ExplodeImmediate;
    bool Reacted;
    bool ItemActivated;
    bool IsChasmRecovery;
    OnHitActionBase* HitAction;
    int32_t StoryActionId;
    ActionOriginator Originator;
    uint64_t Flags;
    glm::vec3 SourcePosition;
    glm::vec3 field_574;
    FixedString Level;
    ProjectileTemplate* Template;
    [[bg3::hidden]] Array<void*> field_590;
    ecs::EntityRef Owner;
    bool ShouldFall;
    [[bg3::hidden]] void* _PAD;
};

struct ImpactEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ProjectileImpactEvent, "esv::projectile::ImpactEventOneFrameComponent")

    bg3se::spell_cast::IntermediateTarget Target;
    EntityHandle field_A8;
    bool field_B0;
};

struct RequestTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ProjectileRequestTargetTracking, "esv::projectile::RequestTargetTrackingOneFrameComponent")

    Guid field_0;
    HashSet<EntityHandle> Targets;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ProjectileUpdateTargetTracking, "esv::projectile::UpdateTargetTrackingOneFrameComponent")

    Guid field_0;
    HashSet<EntityHandle> Targets;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::projectile, SplitThrowableObjectRequestOneFrameComponent, ProjectileSplitThrowableObjectRequest)

END_NS()

BEGIN_NS(spell_cast)

struct ProjectileTargetData
{
    glm::vec3 field_0;
    IntermediateTarget Target;
    FixedString TextKey;
    int field_BC{ 0 };
    HitDesc Hit;
    std::optional<path::Settings> PathSettings;
    std::optional<projectile::ProjectileResult> Result;
    EntityHandle ThrownObject;
};

END_NS()

BEGIN_NS(esv::spell_cast)

struct ProjectileResultsExtraData
{
    glm::vec3 field_0;
    glm::vec3 field_C;
    Array<bg3se::projectile::ProjectileResult> Results;
};


struct ProjectileCacheComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerProjectileCache, "esv::spell_cast::ProjectileCacheComponent")

    std::optional<bg3se::spell_cast::ProjectileTargetData> Target;
    HashMap<int, float> ProjectileDelayTimers;
    [[bg3::hidden]] HashMap<int, void*> field_390; // ObjectQueue<ProjectileTargetData>
    Array<ProjectileResultsExtraData> ExtraData;
};

END_NS()
