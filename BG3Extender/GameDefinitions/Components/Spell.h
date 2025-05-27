#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Stats/Common.h>

BEGIN_NS(spell)

struct Modification
{
    struct ModifyAreaRadius
    {
        SpellModificationValueType ValueType;
        float AreaRadius;
    };

    struct ModifyMaximumTargets
    {
        SpellModificationValueType ValueType;
        int32_t MaximumTargets;
    };

    struct ModifyNumberOfTargets
    {
        SpellModificationValueType ValueType;
        int32_t NumberOfTargets;
    };

    struct ModifySavingThrowDisadvantage
    {
        uint8_t SavingThrowDisadvantage;
    };

    struct ModifySpellFlags
    {
        uint8_t SpellFlags;
        uint8_t field_1;
    };

    struct ModifySpellRoll
    {
        STDString Conditions;
        STDString Conditions2;
        int field_30;
    };

    struct ModifyStatusDuration
    {
        SpellModificationValueType ValueType;
        float StatusDuration;
    };

    struct ModifySummonDuration
    {
        SpellModificationValueType ValueType;
        float SummonDuration;
    };

    struct ModifySurfaceDuration
    {
        SpellModificationValueType ValueType;
        float SurfaceDuration;
    };

    struct ModifyTargetRadius
    {
        SpellModificationValueType ValueType;
        float TargetRadius;
    };

    struct ModifyUseCosts
    {
        uint8_t Type;
        Guid Resource;
        STDString Expression;
        int Level;
        Guid Resource2;
    };

    struct ModifyVisuals
    {
        uint8_t Visuals;
    };

    struct ModifyIconGlow
    {
        uint8_t IconGlow;
    };

    struct ModifyTooltipDescription
    {
        uint8_t TooltipDescription;
    };

    using Variant = std::variant<ModifyAreaRadius, ModifyMaximumTargets, ModifyNumberOfTargets, ModifySavingThrowDisadvantage, ModifySpellFlags, ModifySpellRoll, ModifyStatusDuration, ModifySummonDuration, ModifySurfaceDuration, ModifyTargetRadius, ModifyUseCosts, ModifyVisuals, ModifyIconGlow, ModifyTooltipDescription>;

    uint8_t field_0;
    FixedString Source;
    Variant Modification;
    HashSet<SpellId> Spells;
};

struct SpellMeta
{
    SpellMetaId SpellId;
    EntityHandle BoostHandle;
    [[bg3::legacy(SelectionType)]] SpellLearningStrategy LearningStrategy;
    [[bg3::legacy(field_29)]] SpellPrepareType PrepareType;
    [[bg3::legacy(SpellUUID)]] Guid PreferredCastingResource;
    AbilityId SpellCastingAbility;
    SpellCooldownType CooldownType;
    FixedString ContainerSpell;
    [[bg3::legacy(field_48)]] bool LinkedSpellContainer;
};

struct SpellContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellContainer, "eoc::spell::ContainerComponent")

    Array<SpellMeta> Spells;
};

struct PlayerPrepareSpellComponent : public BaseComponent
{
    DEFINE_COMPONENT(PlayerPrepareSpell, "eoc::spell::PlayerPrepareSpellComponent")

    Array<SpellMetaId> Spells;
    uint8_t field_30;
};

struct CCPrepareSpellComponent : public BaseComponent
{
    DEFINE_COMPONENT(CCPrepareSpell, "eoc::spell::CCPrepareSpellComponent")

    Array<SpellMetaId> Spells;
};

struct SpellModificationContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellModificationContainer, "eoc::spell::ModificationContainerComponent")

    HashMap<FixedString, Array<Modification>> Modifications;
};

struct AddedSpellsComponent : public BaseComponent
{
    DEFINE_COMPONENT(AddedSpells, "eoc::spell::AddedSpellsComponent")

    Array<SpellMeta> Spells;
};

struct CooldownData
{
    SpellId SpellId;
    SpellCooldownType CooldownType;
    uint8_t field_29;
    float Cooldown;
    Guid field_30;
};

struct SpellBookCooldownsComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellBookCooldowns, "eoc::spell::BookCooldownsComponent")

    Array<CooldownData> Cooldowns;
};

struct LearnedSpellsComponent : public BaseComponent
{
    DEFINE_COMPONENT(LearnedSpells, "eoc::spell::LearnedSpellsComponent")

    HashMap<Guid, HashSet<FixedString>> field_18;
    HashSet<SpellSchoolId> SpellSchools;
};

struct SpellAiConditionsComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellAiConditions, "eoc::spell::AiConditionsComponent")

    HashMap<FixedString, AiActionConditions> Conditions;
};

struct CastRequirements
{
    [[bg3::legacy(field_0)]] uint8_t CastContext;
    [[bg3::legacy(field_4)]] CastRequirementFlags Requirements;
};

struct SpellData
{
    SpellId Id;
    [[bg3::legacy(SpellUUID)]] Guid PreferredCastingResource;
    int32_t UsedCharges;
    int32_t NumCharges;
    SpellCooldownType CooldownType;
    [[bg3::legacy(field_41)]] bool Charged;
    [[bg3::legacy(field_42)]] SpellPrepareType PrepareType;
    AbilityId SpellCastingAbility;
    Array<CastRequirements> CastRequirements;
};

struct BookComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellBook, "eoc::spell::BookComponent")

    EntityHandle field_0;
    Array<SpellData> Spells;
};

struct BookPreparesComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellBookPrepares, "eoc::spell::BookPreparesComponent")

    Array<SpellMetaId> PreparedSpells;
    HashMap<Guid, int> field_30;
    HashMap<Guid, int> field_88;
};

END_NS()

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

struct SpellRollCastEventData
{
    [[bg3::legacy(field_0)]] FixedString CastKey;
    HitDesc Hit;
};

struct SpellRollTargetInfo
{
    uint64_t Flags;
    glm::vec3 Position;
};

struct SpellRollData
{
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    [[bg3::legacy(Hits)]] Array<SpellRollCastEventData> Casts;
    HashMap<FixedString, int32_t> NameToCastIndex;
    int NextReaction;
    SpellMetaConditionType SpellConditionsType;
    std::optional<SpellRollTargetInfo> TargetInfo;
};

struct RollsComponent : public BaseComponent
{
    DEFINE_COMPONENT(SpellCastRolls, "eoc::spell_cast::RollsComponent")

    Array<SpellRollData> Rolls;
};

struct BaseTarget
{
    virtual ~BaseTarget();
    virtual glm::vec3 GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const;
    virtual glm::vec3 LEGACY_GetTargetPosition(ecs::EntityWorld& world) const;
    virtual std::optional<glm::vec3> GetEntityPosition(ecs::WorldView& world) const;
    virtual bool IsAlive(ecs::WorldView&) const;
    virtual bool IsValid() const;

    glm::vec3 DoGetTargetPosition(ecs::EntityWorld* world, SpellType type, glm::vec3 const* pos) const;
    std::optional<glm::vec3> DoGetPosition(EntityHandle entity, ecs::EntityWorld* world, SpellType type) const;

    EntityHandle Target;
    [[bg3::legacy(TargetOverride)]] std::optional<EntityHandle> TargetProxy;
    std::optional<glm::vec3> Position;
};

struct InitialTarget : public BaseTarget
{
    glm::vec3 GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const override;
    glm::vec3 LEGACY_GetTargetPosition(ecs::EntityWorld& world) const override;
    bool IsAlive(ecs::WorldView&) const override;
    bool IsValid() const override;

    SpellType TargetingType;
    std::optional<BaseTarget> Target2;
};

struct IntermediateTarget : public BaseTarget
{
    [[bg3::legacy(Target2)]] InitialTarget InitialTarget;
    uint8_t field_A0;
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

BEGIN_NS(esv::spell)

struct OnDamageSpell
{
    FixedString Spell;
    int field_4;
    uint8_t field_8;
};

struct OnDamageSpellsComponent : public BaseComponent
{
    DEFINE_COMPONENT(OnDamageSpells, "esv::spell::OnDamageSpellsComponent")

    Array<OnDamageSpell> Spells;
};

struct NewSpellsAddedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(NewSpellsAddedEvent, "esv::spell::NewSpellsAddedEventOneFrameComponent")

    Array<SpellMetaId> Spells;
};

struct LearnedSpell
{
    FixedString SpellId;
    Guid ProgressionSource;
};

struct SpellsLearnedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(SpellsLearnedEvent, "esv::spell::SpellsLearnedEventOneFrameComponent")

    Array<LearnedSpell> Spells;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::spell, BookChangedOneFrameComponent, SpellBookChanged)


struct SpellUnprepareInfo : public SpellMetaId
{
    uint8_t field_20{ 0 };
};

struct SpellSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerSpell, "esv::SpellSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] UnknownSignal SpellPreparedSignal;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* StatsSystem;
    [[bg3::hidden]] void* BoostSystem;
    [[bg3::hidden]] void* ActionResourceSystem;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* ActionResourceTypes;
    [[bg3::hidden]] void* LevelMaps;
    [[bg3::hidden]] void* ShapeshiftSystem;
    [[bg3::hidden]] void* LearningSystem;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* FeatManager;
    [[bg3::hidden]] void* RulesetModifierOptions;
    [[bg3::hidden]] void* GlobalSwitches;
    [[bg3::hidden]] void* Random;
    [[bg3::hidden]] UnknownFunction field_B0;
    HashSet<EntityHandle> UpdateItemSpells;
    HashSet<EntityHandle> InvalidateSpells;
    HashSet<EntityHandle> ReloadSpellbook;
    HashMap<EntityHandle, Array<bg3se::spell::SpellMeta>> AddSpells;
    HashMap<EntityHandle, Array<SpellMetaId>> RemoveSpell;
    HashSet<EntityHandle> RemoveAllDebugSpells;
    HashMap<EntityHandle, Array<SpellMetaId>> PrepareSpell;
    HashMap<EntityHandle, Array<SpellUnprepareInfo>> UnprepareSpell;
    HashMap<EntityHandle, Array<SpellMetaId>> PlayerPrepareSpell;
    HashMap<EntityHandle, Array<SpellMetaId>> PlayerUnprepareSpell;
    HashSet<EntityHandle> CCPrepareSpells;
    HashSet<EntityHandle> PrepareChanges;
    HashSet<EntityHandle> RefreshPreparedAmounts;
    HashSet<EntityHandle> RefreshPrepareMaxAmounts;
    HashSet<EntityHandle> UpdateEquipmentSpells;
    HashSet<EntityHandle> field_420;
};

struct SpellCooldownSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerSpellCooldown, "esv::SpellCooldownSystem")

    [[bg3::hidden]] UnknownFunction field_10;
    [[bg3::hidden]] void* CharacterManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* RollManager;
    HashMap<EntityHandle, HashMap<SpellId, Guid>> StartCooldown;
    HashMap<EntityHandle, HashSet<SpellCooldownType>> ResetCooldowns;
    HashMap<EntityHandle, float> UpdateCooldown;
    HashSet<EntityHandle> ResetAllCooldowns;
    HashSet<EntityHandle> RoundCooldowns;
    HashMap<EntityHandle, bool> RechargeSpells_EH_bool;
    HashMap<EntityHandle, Array<SpellId>> UseRechargeSpell;
};

struct LearnRequest
{
    Guid Class;
    FixedString SpellId;
};

struct LearningSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerSpellLearning, "esv::spell::LearningSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* BoostSystem;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] UnknownFunction field_48;
    HashSet<EntityHandle> SavantChanged;
    HashSet<EntityHandle> RemoveLearnedSpells;
    HashMap<EntityHandle, Array<LearnRequest>> LearnRequests;
};


END_NS()

BEGIN_NS(esv::spell_cast)

struct CastStartRequest
{
    SpellId Spell;
    SpellCastOptions CastOptions{ 0 };
    EntityHandle Caster;
    Array<bg3se::spell_cast::InitialTarget> Targets;
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
    std::optional<std::variant<std::optional<navigation::TargetInfo>, std::optional<PathSettings>, uint8_t /* SpellTargetingState */, std::optional<glm::vec3>, bg3se::spell_cast::BaseTarget, bool, int>> Param;
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
    HashMap<FixedString, HashMap<int, Array<bg3se::spell_cast::IntermediateTarget>>> IntermediateTargets;
    HashMap<FixedString, int> TargetCounts;
    uint32_t MovementTransactionId;
    int32_t field_DC;
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

END_NS()
