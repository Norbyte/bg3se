#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Components/SpellCastShared.h>
#include <GameDefinitions/Components/Interrupt.h>
#include <GameDefinitions/Components/Projectile.h>


BEGIN_NS(spell_cast)

struct AnimationInfoComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastAnimationInfo, "eoc::spell_cast::AnimationInfoComponent")

    [[bg3::legacy(field_0)]] SpellAnimationCastEvent Event;
    std::optional<glm::vec3> TargetHitPosition;
    glm::vec3 TargetPosition;
    EntityHandle Target;
    uint8_t field_28;
    bool TargetIsCaster;
    [[bg3::legacy(field_2A)]] bool LoopingCastAnimation;
    uint8_t field_2B; // CacheFlag256
    [[bg3::legacy(field_2C)]] bool DualWielding;
    [[bg3::legacy(field_2D)]] bool HasWeapon;
    [[bg3::legacy(field_2E)]] uint8_t ObjectSize;
};

struct CacheComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastCache, "eoc::spell_cast::CacheComponent")

    AbilityId SpellCastingAbility;
    uint32_t Flags;
};

struct DataCache
{
    int field_0;
    uint8_t field_4;
    uint8_t field_5;
    EntityHandle Entity;
    SpellId Spell;
    uint8_t field_48;
    uint8_t field_49;
};

struct DataCacheSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastDataCache, "eoc::spell_cast::DataCacheSingletonComponent")

    HashMap<FixedString, DataCache> Spells;
};

struct IsCastingComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastIsCasting, "eoc::spell_cast::IsCastingComponent")
    
    EntityHandle Cast;
};

struct InterruptResultsComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastInterruptResults, "eoc::spell_cast::InterruptResultsComponent")
    
    bool HasReplacement;
    HashSet<EntityHandle> Results;
};

struct MovementComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastMovement, "eoc::spell_cast::MovementComponent")

    glm::vec3 Position;
    std::optional<glm::vec3> field_C;
    // FixedString TextKey;
};

struct RollsComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastRolls, "eoc::spell_cast::RollsComponent")

    Array<SpellRollData> Rolls;
};

struct StateComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastState, "eoc::spell_cast::StateComponent")

    EntityHandle Entity;
    EntityHandle Caster;
    SpellId SpellId;
    [[bg3::legacy(field_38), bg3::legacy(Flags)]] SpellCastOptions CastOptions;
    Array<InitialTarget> Targets;
    [[bg3::legacy(CasterMoveToPosition)]] std::optional<glm::vec3> CastPosition;
    [[bg3::legacy(field_60)]] std::optional<glm::vec3> CastEndPosition;
    glm::vec3 CasterStartPosition;
    [[bg3::legacy(field_80)]] EntityHandle Source;
    [[bg3::legacy(field_88)]] uint32_t Random;
    Guid SpellCastGuid;
    [[bg3::legacy(field_A0)]] STDString NetGuid;
};

struct SyncTargetingComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellSyncTargeting, "eoc::spell_cast::SyncTargetingComponent")

    BaseTarget Target;
    Array<InitialTarget> Targets;
    [[bg3::legacy(field_40)]] bool CanMoveToThrowTarget;
    int field_44;
    std::optional<glm::vec3> field_48;
    [[bg3::legacy(field_58)]] std::optional<glm::vec3> HoverPosition;
    std::optional<glm::vec3> CasterPosition;
    std::optional<glm::vec3> CasterMoveToPosition;
    std::optional<glm::vec3> field_88;
};

struct OutcomeComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastOutcome, "eoc::spell_cast::OutcomeComponent")

    SpellCastFailReason Result;
};

struct ExecutionTimeComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastExecutionTime, "eoc::spell_cast::ExecutionTimeComponent")

    std::optional<float> Time;
};

struct CastEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastEvent, "eoc::spell_cast::CastEventOneFrameComponent")

    HitDesc Hit;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
};

struct CastTextKeyEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastTextKeyEvent, "eoc::spell_cast::CastTextKeyEventOneFrameComponent")

    EntityHandle CastEntity;
    FixedString TextKey;
};

struct CastHitEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastHitEvent, "eoc::spell_cast::CastHitEventOneFrameComponent")

    EntityHandle CastEntity;
    FixedString TextKey;
};

struct DestroyEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastDestroyEvent, "eoc::spell_cast::DestroyEventOneFrameComponent")

    Guid SpellCastGuid;
};

struct SpellRollAbortEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastSpellRollAbortEvent, "eoc::spell_cast::SpellRollAbortEventOneFrameComponent")

    HitDesc Hit;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
};

struct TargetHitEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastTargetHitEvent, "eoc::spell_cast::TargetHitEventOneFrameComponent")

    HitDesc Hit;
    AttackDesc Attack;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    EntityHandle SpellCast;
    glm::vec3 TargetPosition;
};

struct TargetHitInterruptEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastTargetHitInterruptEvent, "eoc::spell_cast::TargetHitInterruptEventOneFrameComponent")

    HitDesc Hit;
    AttackDesc Attack;
    [[bg3::legacy(field_1D0)]] EntityHandle Target;
    [[bg3::legacy(field_1D8)]] EntityHandle Source;
    [[bg3::legacy(field_1E0)]] std::optional<glm::vec3> TargetPosition;
    [[bg3::legacy(field_1F0)]] std::optional<glm::vec3> SourcePosition;
    SpellId Spell;
};

struct TargetReactionEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastTargetReactionEvent, "eoc::spell_cast::TargetReactionEventOneFrameComponent")

    HitDesc Hit;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    EntityHandle CastEntity;
};

struct TargetsChangedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastTargetsChangedEvent, "eoc::spell_cast::TargetsChangedEventOneFrameComponent")

    Array<InitialTarget> Targets;
};

struct AnimationRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastAnimationRequest, "eoc::spell_cast::AnimationRequestOneFrameComponent")

    SpellAnimationCastEvent Event;
    std::optional<glm::vec3> TargetHitPosition;
    glm::vec3 TargetPosition;
    EntityHandle Target;
    uint8_t field_28;
    bool TargetIsCaster;
    bool LoopingCastAnimation;
    bool CacheFlag256;
    bool DualWielding;
    bool HasWeapon;
    uint8_t ObjectSize;
};

struct FinishedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastFinishedEvent, "eoc::spell_cast::FinishedEventOneFrameComponent")

    SpellCastFailReason Result;
    uint8_t field_1;
};

DEFINE_TAG_COMPONENT(eoc::spell_cast, CanBeTargetedComponent, SpellCastCanBeTargeted)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PrepareStartEventOneFrameComponent, SpellCastPrepareStartEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PrepareEndEventOneFrameComponent, SpellCastPrepareEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, LogicExecutionStartEventOneFrameComponent, SpellCastLogicExecutionStartEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, LogicExecutionEndEventOneFrameComponent, SpellCastLogicExecutionEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PreviewEndEventOneFrameComponent, SpellCastPreviewEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, CounteredEventOneFrameComponent, SpellCastCounteredEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, JumpStartEventOneFrameComponent, SpellCastJumpStartEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, ThrowPickupPositionChangedEventOneFrameComponent, SpellCastThrowPickupPositionChangedEvent)

END_NS()


BEGIN_NS(esv::spell_cast)

using namespace bg3se::spell_cast;

struct CastStartRequest
{
    SpellId Spell;
    SpellCastOptions CastOptions{ 0 };
    EntityHandle Caster;
    Array<InitialTarget> Targets;
    ActionOriginator Originator;
    EntityHandle Item;
    EntityHandle field_70;
    std::optional<glm::vec3> CastPosition;
    int StoryActionId{ 0 };
    STDString NetGuid;
    uint8_t field_A8{ 1 };
    Guid RequestGuid;
};

struct CastCancelRequest
{
    EntityHandle Caster;
    std::optional<Guid> SpellCastGuid;
    bool Forced{ false };
    bool CharacterReassigned{ false };
};

struct CastConfirmRequest
{
    EntityHandle Entity;
    Guid SpellCastGuid;
    [[bg3::hidden]] Array<void*> ProjectileResultsExtraData;
    std::optional<navigation::TargetInfo> TargetInfo;
    std::optional<PathSettings> PathSettings;
    bool field_AC{ false };
};

struct PreviewSetRequest
{
    EntityHandle Entity;
    Guid SpellCastGuid;
    uint8_t field_18;
    std::optional<std::variant<std::optional<navigation::TargetInfo>, std::optional<PathSettings>, uint8_t /* SpellTargetingState */, std::optional<glm::vec3>, BaseTarget, bool, int>> Param;
};

struct CastHitDelayInfo
{
    int InitialIndex;
    int NextIndex;
    int IntermediateIndex;
    std::optional<int> OverallIndex;
    FixedString TextKey;
    int TextKeyIndex;
};

struct CastHitDelayComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastHitDelay, "esv::spell_cast::CastHitDelayComponent")

    Array<CastHitDelayInfo> Delays;
    float HitDelay;
    float HitDelayRemaining;
};

struct CastRequestsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastRequests, "esv::spell_cast::CastRequestsComponent")

    Array<CastStartRequest> StartRequests;
    Array<CastConfirmRequest> ConfirmRequests;
    Array<CastCancelRequest> CancelRequests;
    Array<PreviewSetRequest> PreviewSetRequests;
};

struct PendingRequestsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastPendingRequests, "esv::spell_cast::PendingRequestsComponent")

    Array<CastConfirmRequest> ConfirmRequests;
    Array<CastCancelRequest> CancelRequests;
    Array<PreviewSetRequest> PreviewSetRequests;
};


struct CastResponsibleComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastResponsible, "esv::spell_cast::CastResponsibleComponent")

    EntityHandle Entity;
};

DEFINE_TAG_COMPONENT(esv::spell_cast, ClientInitiatedComponent, ServerSpellClientInitiated)

struct ExternalDependency
{
    __int64 field_0;
    int field_8;
    char field_C;
};

struct ExternalsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellExternals, "esv::spell_cast::ExternalsComponent")

    Array<ExternalDependency> Externals;
};

struct HitRegister
{
    EntityHandle Target;
    uint64_t ContextType;
};

struct HitRegisterComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellHitRegister, "esv::spell_cast::HitRegisterComponent")

    Array<HitRegister> Hits;
};

struct MovementComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastMovement, "esv::spell_cast::MovementComponent")

    std::optional<glm::vec3> field_0;
    bool IsMoving;
    float Duration;
    float Progress;
    FixedString TextKey;
};

struct MovementInfoComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastMovementInfo, "esv::spell_cast::MovementInfoComponent")

    std::optional<navigation::TargetInfo> Settings;
    std::optional<PathSettings> PathSettings;
};

struct StateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastState, "esv::spell_cast::StateComponent")

    SpellCastPhase Phase;
    int field_4;
    ActionOriginator Originator;
    int StoryActionId;
};

struct ZoneRangeComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastZoneRange, "esv::spell_cast::ZoneRangeComponent")

    int field_0;
    int field_4;
    int field_8;
    int field_C;
};

struct CacheComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastCache, "esv::spell_cast::CacheComponent")

    Array<stats::ActionResourceCost> Costs;
    HashMap<int, bool> TextKeyIndices;
    uint32_t TextKeyIndex;
    int32_t field_54;
    HashMap<FixedString, HashMap<int, Array<IntermediateTarget>>> IntermediateTargets;
    HashMap<FixedString, int> TargetCounts;
    uint32_t MovementTransactionId;
    std::optional<glm::vec3> field_E0;
    bool HasPathfindTemplate;
    bool PhaseFinished;
};

struct PathfindTrajectory
{
    [[bg3::hidden]] Array<void*> Path; // navcloud::NodePath
};

struct ProjectilePathfindCacheComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastProjectilePathfindCache, "esv::spell_cast::ProjectilePathfindCacheComponent")

    Array<EntityHandle> Targets;
    [[bg3::hidden]] Array<PathfindTrajectory> Trajectories;
};

struct UnsheathFallbackTimerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerSpellCastUnsheathFallbackTimer, "esv::spell_cast::UnsheathFallbackTimerComponent")

    std::optional<float> Time;
};

struct SurfaceTrail
{
    SurfaceType SurfaceType;
    float Radius;
};

struct MoveDuringCastUpdateEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastMoveDuringCastUpdateEvent, "esv::spell_cast::MoveDuringCastUpdateEventOneFrameComponent")

    glm::vec3 Position;
    glm::vec3 AdjustedPosition;
    std::optional<SurfaceTrail> SurfaceTrail;
};

struct MovementAndPrecalculationEndEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastMovementAndPrecalculationEndEvent, "esv::spell_cast::MovementAndPrecalculationEndEventOneFrameComponent")

    bool PhaseFinished;
    int field_4;
};

struct RequestTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastRequestTargetTracking, "esv::spell_cast::RequestTargetTrackingOneFrameComponent")

    Guid field_0;
    HashSet<EntityHandle> Targets;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastUpdateTargetTracking, "esv::spell_cast::UpdateTargetTrackingOneFrameComponent")

    Guid TargetHistoryUUID;
    HashSet<EntityHandle> Targets;
};

struct WeaponSetChangeRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellCastWeaponSetChangeRequest, "esv::spell_cast::WeaponSetChangeRequestOneFrameComponent")

    bool IsRanged;
};


struct CastRequestSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerCastRequest, "esv::spell_cast::CastRequestSystem")

    [[bg3::hidden]] void* VMT2;
    Array<CastStartRequest> AnubisCastRequests;
    Array<CastStartRequest> OsirisCastRequests;
    Array<CastStartRequest> NetworkStartRequests;
    Array<CastStartRequest> ReactionStartRequests;
    Array<CastStartRequest> ItemStartRequests;
    Array<CastStartRequest> ActiveRollNodeStartRequests;
    Array<CastStartRequest> JumpStartRequests;
    Array<CastStartRequest> CommandProtocolStartRequests;
    Array<CastStartRequest> PlanCancelRequests;
    Array<CastStartRequest> field_A8;
    Array<CastCancelRequest> AnubisCancelRequests;
    Array<CastCancelRequest> OsirisCancelRequests;
    Array<CastCancelRequest> NetworkCancelRequests;
    Array<CastCancelRequest> field_E8;
    Array<CastCancelRequest> GameplayControllerCancelRequests;
    Array<CastCancelRequest> ReactionCancelRequests;
    Array<CastCancelRequest> CharacterCancelRequests;
    Array<CastCancelRequest> TeleportCancelRequests;
    Array<PreviewSetRequest> NetworkPreviewUpdateRequests;
    Array<CastConfirmRequest> ConfirmRequests;
};


struct Event_Finish
{
    SpellCastFailReason FailReason;
    uint8_t field_1;
};

struct Event_MoveDuringCastInitialize
{
    float Duration;
};

struct Event_MoveDuringCastStart
{
    FixedString TextKey;
};

struct Event_MoveDuringCastUpdate
{
    float Progress;
    glm::vec3 Position;
    glm::vec3 AdjustedPosition;
};

struct Event_MoveDuringCastEnd
{
    glm::vec3 Position;
    glm::vec3 AdjustedPosition;
};

struct Event_Cast
{
    HitDesc Hit;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
};

struct Event_CastHit
{
    FixedString TextKey;
    uint32_t Index;
};

struct Event_CastTextKey
{
    FixedString TextKey;
};

struct Event_TargetHit
{
    HitDesc Hit;
    AttackDesc Attack;
    EntityHandle Target;
    glm::vec3 TargetPosition;
    std::optional<EntityHandle> TargetProxy;
    uint32_t NumConditionRolls;
    EntityHandle Caster;
    std::optional<glm::vec3> SourcePosition;
    uint64_t ContextType;
};

struct Event_SpellRollAbort
{
    HitDesc Hit;
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
};

struct Event_WeaponSetRequest
{
    bool IsRanged;
};

struct Event_PlayAnimation
{
    SpellAnimationCastEvent CastEvent;
};

struct Event_PhaseChange
{
    SpellCastPhase Phase;
};

struct Event_CacheFlagChange
{
    uint32_t Flag;
};

struct Event_UseCostsChange
{
    Array<stats::ActionResourceCost> Costs;
};

struct Event_InitializeRolls
{
    Array<SpellRollData> Rolls;
};

struct Event_AdvanceRoll
{
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    FixedString TextKey;
    uint8_t SpellConditionsType;
};

struct Event_AdvanceReaction
{
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    uint8_t SpellConditionsType;
};

struct Event_FireProjectile
{
    bg3se::spell_cast::ProjectileTargetData Target;
    FixedString Trajectory;
    uint8_t field_34C;
    uint32_t ProjectileIndex;
};

struct Event_ChangeProjectileDelayTimer
{
    float ProjectileDelay;
    int ProjectileIndex;
};

struct Event_PopDelayedProjectile
{
    int ProjectileIndex;
};

struct Event_InitializeIntermediateTargets
{
    uint32_t Index;
    FixedString TextKey;
    Array<IntermediateTarget> Targets;
};

struct Event_AddDelayedCastHit
{
    uint64_t InitialIndex;
    uint32_t IntermediateIndex;
    std::optional<uint32_t> OverallIndex;
    FixedString TextKey;
    uint32_t TextKeyIndex;
};

struct Event_ChangeCastPosition
{
    glm::vec3 Position;
};

struct Event_ChangeCastEndPosition
{
    glm::vec3 Position;
};

struct Event_ChangeMovementTransactionId
{
    uint32_t TransactionId;
};

struct Event_MovementEnd
{
    bool PhaseFinished;
};

struct Event_CreateZone
{
    FixedString TextKey;
    uint32_t TextKeyIndex;
    InitialTarget Target;
};

struct Event_DoTeleport
{
    FixedString TextKey;
    uint32_t TextKeyIndex;
    InitialTarget Target;
};

struct Event_CreateWall
{
    InitialTarget Target;
};

struct Event_TargetsChanged
{
    Array<InitialTarget> Targets;
};

struct Event_CastOptionsChange
{
    uint32_t SetOptions;
    uint32_t ClearOptions;
};

struct Event_TargetsChange
{
    Array<InitialTarget> Targets;
};

struct Event_AddSharedMovement
{
    glm::vec3 Position;
    std::optional<glm::vec3> field_C;
};

struct Event_AddProjectileCache
{
    Array<ProjectileResultsExtraData> Projectiles;
};

struct Event_AddMovementInfo
{
    std::optional<navigation::TargetInfo> TargetInfo;
    std::optional<PathSettings> PathSettings;
};

struct Event_InterruptStart
{
    bg3se::interrupt::InterruptEvent Event;
    HashMap<EntityHandle, HashSet<EntityHandle>> Interruptors;
    HashMap<Guid, bg3se::interrupt::ExecutedDependency> ExecutedDependencies;
};

struct InterruptApplyResultBase
{
    bool HasReplacement;
    HashMap<Guid, bg3se::interrupt::RollAdjustments> RollAdjustments;
    HashMap<bg3se::interrupt::DamageFunctorKey, bg3se::interrupt::DamageRollAdjustments> DamageRollAdjustments;
};

struct Event_InterruptEventChange
{
    bg3se::interrupt::InterruptEvent Event;
    bool HasReplacement;
    bg3se::interrupt::InterruptEvent ReplacementEvent;
    HashMap<EntityHandle, HashSet<EntityHandle>> Interruptors;
    HashMap<bg3se::interrupt::InterruptEvent, HashMap<EntityHandle, HashSet<EntityHandle>>> EventInterruptors;
    InterruptApplyResultBase Results;
    HashMap<EntityHandle, bg3se::interrupt::InterruptUsage> InterruptUsage;
    HashMap<Guid, bg3se::interrupt::ExecutedDependency> ExecutedDependencies;
};

struct Event_InterruptApplyResult : public InterruptApplyResultBase
{
    HashMap<EntityHandle, bg3se::interrupt::InterruptUsage> InterruptUsage;
};

struct Event_InterruptEnd
{
    bool StopInterrupt;
    bg3se::interrupt::InterruptEvent Event;
};

struct Event_PrecalculatedInterruptors
{
    Array<bg3se::interrupt::PrecalculatedAnimationInterruptData> Interruptors;
    bool HasReplacement;
};

struct Event_PushPausedAnimationEvents
{
    Array<bg3se::interrupt::PausedAnimationEvent> Events;
};

struct Event_ChangeStoryActionId
{
    int32_t StoryActionId;
};

struct Event_ZoneRangeUpdate
{
    float field_0;
    float field_4;
    float field_8;
    float field_C;
};

struct Event_PrecalculatedConditionals
{
    Array<FunctorConditional> Conditionals;
};

struct Event_UpdateConditionals
{
    Array<FunctorConditional> Conditionals;
};

struct Event_UpdateInterruptors
{
    HashMap<EntityHandle, HashSet<EntityHandle>> Interruptors;
};


struct SystemEvent
{
    SystemEventType Type;
    EntityHandle Entity;
    std::optional<std::variant<
        Event_Finish,
        Event_MoveDuringCastInitialize,
        Event_MoveDuringCastStart,
        Event_MoveDuringCastUpdate,
        Event_MoveDuringCastEnd,
        Event_Cast,
        Event_CastHit,
        Event_CastTextKey,
        Event_TargetHit,
        Event_SpellRollAbort,
        Event_WeaponSetRequest,
        Event_PlayAnimation,
        Event_PhaseChange,
        Event_CacheFlagChange,
        Event_UseCostsChange,
        Event_InitializeRolls,
        Event_AdvanceRoll,
        Event_AdvanceReaction,
        Event_FireProjectile,
        Event_ChangeProjectileDelayTimer,
        Event_PopDelayedProjectile,
        Event_InitializeIntermediateTargets,
        Event_AddDelayedCastHit,
        Event_ChangeCastPosition,
        Event_ChangeCastEndPosition,
        Event_ChangeMovementTransactionId,
        Event_MovementEnd,
        Event_CreateZone,
        Event_DoTeleport,
        Event_CreateWall,
        Event_TargetsChanged,
        Event_CastOptionsChange,
        Event_TargetsChange,
        Event_AddSharedMovement,
        Event_AddProjectileCache,
        Event_AddMovementInfo,
        Event_InterruptStart,
        Event_InterruptEventChange,
        Event_InterruptApplyResult,
        Event_InterruptEnd,
        Event_PrecalculatedInterruptors,
        Event_PushPausedAnimationEvents,
        Event_ChangeStoryActionId,
        Event_ZoneRangeUpdate,
        Event_PrecalculatedConditionals,
        Event_UpdateConditionals,
        Event_UpdateInterruptors
    >> Args;
};


END_NS()

BEGIN_NS(esv::concentration)

struct ConcentrationChangedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ConcentrationChanged, "esv::concentration::ConcentrationChangedOneFrameComponent")

    SpellId Started;
    SpellId Ended;
    bool Interrupted;
};

struct DamageCheckOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ConcentrationDamageCheck, "esv::concentration::DamageCheckOneFrameComponent")

    Array<HitDesc> Hits;
};

struct OnConcentrationClearedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ConcentrationClearedEvent, "esv::concentration::OnConcentrationClearedEventOneFrameComponent")

    EntityHandle Target;
    EntityHandle Status;
    int16_t SurfaceIndex;
    bool field_12;
};

struct ConcentrationRequest
{
    EntityHandle Caster;
    SpellId SpellId;
    ConcentrationTarget Target;
    bool ClearConcentration{ false };
    bool BreakConcentration{ false };
    bool CheckReset{ false };
    ConcentrationRequestType RequestType{ ConcentrationRequestType::RemoveTarget };
    std::optional<Guid> SpellCastGuid;
};


struct ConcentrationSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerConcentration, "esv::concentration::ConcentrationSystem")

    [[bg3::hidden]] void* VMT_GameEventListener;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* StatusFactory;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* GlobalSwitches;
    [[bg3::hidden]] void* UnitTestHelper;
    uint8_t field_48;
    Array<ConcentrationRequest> ConcentrationRequests;
};


END_NS()
