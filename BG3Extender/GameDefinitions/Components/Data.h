#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Progression.h>

BEGIN_SE()

struct GenericPropertyTag
{
    uint8_t Type;
    Guid Entity;
};

struct Bound : public ProtectedGameObject<Bound>
{
    EntityHandle Entity;
    LegacyRefMap<AIBoundType, BoundData> AIBounds;
    [[bg3::hidden]] AiGrid* AiGrid;
    [[bg3::hidden]] void* Parent;
    HashSet<AiTilePos> GridPoints;
    glm::vec3 Translate;
    glm::quat RotationQuat;
    float Scale;
    [[bg3::readonly]] int16_t MetaDataIndex;
    [[bg3::readonly]] uint16_t CollidingRefCount;
    glm::vec3 TranslateOverride;
    BoundBaseFlags BaseFlags;
    uint8_t field_89;
    BoundFlags Flags;
    FixedString OwnerPlatform;
    [[bg3::hidden]] void* field_90;
};

struct BoundComponent : public BaseProxyComponent
{
    DEFINE_COMPONENT(Bound, "eoc::BoundComponent")

    Bound Bound;
};

struct CustomStatsComponent : public BaseComponent
{
    DEFINE_COMPONENT(CustomStats, "eoc::CustomStatsComponent")

    LegacyMap<FixedString, int> Stats;
};

struct DetachedComponent : public BaseComponent
{
    DEFINE_COMPONENT(Detached, "eoc::DetachedComponent")

    uint32_t Flags;
};

struct HealthComponent : public BaseComponent
{
    DEFINE_COMPONENT(Health, "eoc::HealthComponent")

    int Hp;
    int MaxHp;
    int TemporaryHp;
    int MaxTemporaryHp;
    Guid field_10;
    [[bg3::legacy(field_20)]] bool IsInvulnerable;
};

struct DifficultyCheckComponent : public BaseComponent
{
    DEFINE_COMPONENT(DifficultyCheck, "eoc::DifficultyCheckComponent")

    [[bg3::legacy(field_0)]] Array<int32_t> AbilityDC;
    [[bg3::legacy(field_10)]] Array<int32_t> AbilityModifiers;
    Array<AbilityId> Abilities;
    Array<uint32_t> field_30;
    int field_40;
    int field_44;
};

struct BodyTypeComponent : public BaseComponent
{
    DEFINE_COMPONENT(BodyType, "eoc::BodyTypeComponent")

    uint8_t BodyType;
    uint8_t BodyType2;
};

struct ObjectSizeComponent : public BaseComponent
{
    DEFINE_COMPONENT(ObjectSize, "eoc::ObjectSizeComponent")

    int8_t Size;
    [[bg3::legacy(field_1)]] int8_t SoundSize;
};

struct BaseStatsComponent : public BaseComponent
{
    DEFINE_COMPONENT(BaseStats, "eoc::BaseStatsComponent")

    std::array<int, 7> BaseAbilities;
};

struct ExpertiseComponent : public BaseComponent
{
    DEFINE_COMPONENT(Expertise, "eoc::expertise::ExpertiseComponent")

    HashSet<SkillId> Expertise;
};

struct StatusImmunitiesComponent : public BaseComponent
{
    DEFINE_COMPONENT(StatusImmunities, "eoc::StatusImmunitiesComponent")

    HashMap<FixedString, Guid> PersonalStatusImmunities;
};

struct BackgroundComponent : public BaseComponent
{
    DEFINE_COMPONENT(Background, "eoc::BackgroundComponent")

    Guid Background;
};
    
struct GodComponent : public BaseComponent
{
    DEFINE_COMPONENT(God, "eoc::god::GodComponent")

    Guid God;
    std::optional<Guid> GodOverride;
};
    
struct ProficiencyComponent : public BaseComponent
{
    DEFINE_COMPONENT(Proficiency, "eoc::stats::proficiency::ProficiencyComponent")

    [[bg3::legacy(field_0)]] ProficiencyGroupFlags Flags;
};

struct ProficiencyGroupComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProficiencyGroup, "eoc::stats::proficiency::ProficiencyGroupComponent")

    [[bg3::legacy(field_0)]] ProficiencyGroupFlags Flags;
};


struct FloatingComponent : public BaseComponent
{
    DEFINE_COMPONENT(Floating, "eoc::FloatingComponent")

    int field_18;
    int field_1C;
};

struct VoiceComponent : public BaseComponent
{
    DEFINE_COMPONENT(Voice, "eoc::VoiceComponent")

    Guid Voice;
};

struct CharacterCreationStatsComponent : public BaseComponent
{
    DEFINE_COMPONENT(CharacterCreationStats, "eoc::CharacterCreationStatsComponent")

    Guid Race;
    Guid SubRace;
    uint8_t BodyType;
    uint8_t BodyShape;
    STDString Name;
    std::array<int32_t, 7> Abilities;
    uint8_t field_5C;
};

struct DisarmableComponent : public BaseComponent
{
    DEFINE_COMPONENT(Disarmable, "eoc::DisarmableComponent")

    Guid field_0;
    uint8_t field_10;
    uint8_t field_11;
};

struct IsSummonComponent : public BaseComponent
{
    DEFINE_COMPONENT(IsSummon, "eoc::summon::IsSummonComponent")

    [[bg3::legacy(Owner_M)]] EntityHandle Owner;
    [[bg3::legacy(field_8)]] EntityHandle Summoner;
    Guid field_10;
    EntityHandle field_20;
    FixedString field_28;
};

struct SummonContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(SummonContainer, "eoc::summon::ContainerComponent")

    [[bg3::legacy(field_18)]] HashMap<FixedString, Array<EntityHandle>> ByTag;
    HashSet<EntityHandle> Characters;
    HashSet<EntityHandle> Items;
};

struct StealthComponent : public BaseComponent
{
    DEFINE_COMPONENT(Stealth, "eoc::StealthComponent")

    bool SeekHiddenFlag;
    glm::vec3 Position;
    float SeekHiddenTimeout;
    float field_14;
    int field_18;
    float field_1C;
    float field_20;
};

DEFINE_TAG_COMPONENT(eoc, ClientControlComponent, ClientControl)
DEFINE_TAG_COMPONENT(ls, IsGlobalComponent, IsGlobal)
DEFINE_TAG_COMPONENT(ls, SavegameComponent, Savegame)

struct DisabledEquipmentComponent : public BaseComponent
{
    DEFINE_COMPONENT(DisabledEquipment, "eoc::DisabledEquipmentComponent")

    bool ShapeshiftFlag;
};

struct LootingStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(LootingState, "eoc::LootingStateComponent")

    EntityHandle Looter_M;
    uint8_t State;
    int field_24;
};

struct LootComponent : public BaseComponent
{
    DEFINE_COMPONENT(Loot, "eoc::LootComponent")

    uint8_t Flags;
    uint8_t InventoryType;
};

struct LockComponent : public BaseComponent
{
    DEFINE_COMPONENT(Lock, "eoc::lock::LockComponent")

    FixedString Key_M;
    int LockDC;
    Guid field_8;
    Array<Guid> field_18;
};

struct KeyComponent : public BaseComponent
{
    DEFINE_COMPONENT(Key, "eoc::lock::KeyComponent")

    FixedString Key;
};

struct SummonLifetimeComponent : public BaseComponent
{
    DEFINE_COMPONENT(SummonLifetime, "eoc::summon::LifetimeComponent")

    std::variant<uint8_t, float> Lifetime;
};

struct OriginTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(OriginTag, "eoc::OriginTagComponent")

    Array<Guid> Tags;
};

struct OriginPassivesComponent : public BaseComponent
{
    DEFINE_COMPONENT(OriginPassives, "eoc::OriginPassivesComponent")

    Array<stats::PassivePrototype*> Passives;
};

struct ClassTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(ClassTag, "eoc::ClassTagComponent")

    Array<Guid> Tags;
};

struct BackgroundTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(BackgroundTag, "eoc::BackgroundTagComponent")

    Array<Guid> Tags;
};

struct OriginAppearanceTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(OriginAppearanceTag, "eoc::OriginAppearanceTagComponent")

    Array<Guid> Tags;
};

struct VoiceTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(VoiceTag, "eoc::VoiceTagComponent")

    Array<Guid> Tags;
};

struct GodTagComponent : public BaseComponent
{
    DEFINE_COMPONENT(GodTag, "eoc::god::TagComponent")

    Array<Guid> Tags;
};

struct BackgroundPassivesComponent : public BaseComponent
{
    DEFINE_COMPONENT(BackgroundPassives, "eoc::BackgroundPassivesComponent")

    Array<stats::PassivePrototype*> field_18;
};

struct GlobalShortRestDisabledComponent : public BaseComponent
{
    DEFINE_COMPONENT(GlobalShortRestDisabled, "eoc::GlobalShortRestDisabledComponent")
};

struct GlobalLongRestDisabledComponent : public BaseComponent
{
    DEFINE_COMPONENT(GlobalLongRestDisabled, "eoc::GlobalLongRestDisabledComponent")
};

struct StoryShortRestDisabledComponent : public BaseComponent
{
    DEFINE_COMPONENT(StoryShortRestDisabled, "eoc::StoryShortRestDisabledComponent")
};

struct FleeCapabilityComponent : public BaseComponent
{
    DEFINE_COMPONENT(FleeCapability, "eoc::FleeCapabilityComponent")

    FleeErrorFlags Flags;
    [[bg3::legacy(field_1C)]] float FleeDistance;
    [[bg3::legacy(field_20)]] float CurrentFleeDistance;
};

struct CanDoRestComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanDoRest, "eoc::CanDoRestComponent")

    RestFlags Flags;
    [[bg3::legacy(RestErrorFlags1)]] RestErrorFlags LongRestErrorFlags;
    [[bg3::legacy(RestErrorFlags2)]] RestErrorFlags ShortRestErrorFlags;
};

struct CanModifyHealthComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanModifyHealth, "eoc::CanModifyHealthComponent")

    uint16_t Flags;
};

struct CanBeLootedComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanBeLooted, "eoc::CanBeLootedComponent")

    uint16_t Flags;
};

struct CanBeDisarmedComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanBeDisarmed, "eoc::CanBeDisarmedComponent")

    uint16_t Flags;
};

DEFINE_TAG_COMPONENT(eoc, CanTriggerRandomCastsComponent, CanTriggerRandomCasts)

struct CanDoActionsComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanDoActions, "eoc::CanDoActionsComponent")

    CanDoActionsFlags Flags;
};

struct ActionUseConditionsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ActionUseConditions, "eoc::action::ActionUseConditionsComponent")

    Array<int32_t> Conditions;
};


struct CanMoveComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanMove, "eoc::CanMoveComponent")

    CanMoveFlags Flags;
    uint16_t field_4;
    uint8_t field_6;
};

struct CanSenseComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanSense, "eoc::CanSenseComponent")

    uint16_t Flags;
};

struct ConcentrationComponent : public BaseComponent
{
    DEFINE_COMPONENT(Concentration, "eoc::concentration::ConcentrationComponent")

    [[bg3::legacy(field_0)]] EntityHandle Caster;
    [[bg3::legacy(field_8)]] Array<ConcentrationTarget> Targets;
    SpellId SpellId;
};

struct DarknessComponent : public BaseComponent
{
    DEFINE_COMPONENT(Darkness, "eoc::DarknessComponent")

    uint8_t Sneaking;
    uint8_t Obscurity;
    [[bg3::legacy(field_2)]] bool SneakingConesVisibility;
    [[bg3::legacy(field_3)]] bool CharacterSneaking;
    [[bg3::legacy(field_4)]] bool ActuallySneaking;
    [[bg3::legacy(field_8)]] float GracePeriod;
    [[bg3::legacy(field_C)]] float GraceFrames;
};

struct DualWieldingComponent : public BaseComponent
{
    DEFINE_COMPONENT(DualWielding, "eoc::DualWieldingComponent")

    [[bg3::legacy(Melee)]] bool MeleeToggledOn;
    bool MeleeUI;
    [[bg3::legacy(field_1A)]] bool HasMeleeBoosts;
    [[bg3::legacy(Ranged)]] bool RangedToggledOn;
    bool RangedUI;
    [[bg3::legacy(field_1D)]] bool HasRangedBoosts;
    [[bg3::legacy(DisableDualWielding)]] bool ToggledOn;
};

struct GravityDisabledUntilMovedComponent : public BaseComponent
{
    DEFINE_COMPONENT(GravityDisabledUntilMoved, "eoc::GravityDisabledUntilMovedComponent")

    Transform Transform;
};

DEFINE_TAG_COMPONENT(eoc, GravityDisabledComponent, GravityDisabled)
DEFINE_TAG_COMPONENT(eoc::tag, AvatarComponent, Avatar)
DEFINE_TAG_COMPONENT(eoc::tag, HasExclamationDialogComponent, HasExclamationDialog)
DEFINE_TAG_COMPONENT(eoc::tag, TraderComponent, Trader)
DEFINE_TAG_COMPONENT(eoc::ambush, AmbushingComponent, Ambushing)
DEFINE_TAG_COMPONENT(eoc::trade, CanTradeComponent, CanTrade)
DEFINE_TAG_COMPONENT(eoc::falling, IsFallingComponent, IsFalling)
DEFINE_TAG_COMPONENT(eoc::heal, MaxIncomingComponent, HealMaxIncoming)
DEFINE_TAG_COMPONENT(eoc::heal, MaxOutgoingComponent, HealMaxOutgoing)

struct InteractionFilterComponent : public BaseComponent
{
    DEFINE_COMPONENT(InteractionFilter, "eoc::InteractionFilterComponent")

    HashSet<Guid> field_0;
    uint8_t field_30;
    uint8_t field_31;
};

struct InvisibilityComponent : public BaseComponent
{
    DEFINE_COMPONENT(Invisibility, "eoc::InvisibilityComponent")

    uint8_t field_0;
    glm::vec3 field_4;
    uint8_t field_10;
};

DEFINE_TAG_COMPONENT(eoc, IsInTurnBasedModeComponent, IsInTurnBasedMode)

struct ItemBoostsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ItemBoosts, "eoc::ItemBoostsComponent")

    Array<EntityHandle> Boosts;
};

struct RaceComponent : public BaseComponent
{
    DEFINE_COMPONENT(Race, "eoc::RaceComponent")

    Guid Race;
};

struct CanTravelComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanTravel, "eoc::CanTravelComponent")

    TravelFlags Flags;
    uint16_t field_2;
    TravelErrorFlags ErrorFlags;
};

struct MovementComponent : public BaseComponent
{
    DEFINE_COMPONENT(Movement, "eoc::MovementComponent")

    glm::vec3 Direction;
    float Acceleration;
    float Speed;
    float Speed2;
};

struct ObjectInteractionComponent : public BaseComponent
{
    DEFINE_COMPONENT(ObjectInteraction, "eoc::ObjectInteractionComponent")

    Array<EntityHandle> Interactions;
};

struct CanDeflectProjectilesComponent : public BaseComponent
{
    DEFINE_COMPONENT(CanDeflectProjectiles, "eoc::CanDeflectProjectilesComponent")

    uint16_t Flags;
};

struct ActiveSkeletonSlotsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ActiveSkeletonSlots, "ls::ActiveSkeletonSlotsComponent")

    Array<FixedString> Slots;
};

struct SomeSharedServerClientObjId
{
    __int64 field_0;
    int field_8;
};


struct SomeSharedServerClientObjId2 : public SomeSharedServerClientObjId
{
    __int64 field_10;
    int field_18;
};

struct ApprovalRatingsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ApprovalRatings, "eoc::approval::RatingsComponent")

    HashMap<EntityHandle, int> Ratings;
    HashSet<Guid> field_70;
};


struct AttitudeIdentifier
{
    [[bg3::legacy(field_0)]] EntityHandle Character;
    [[bg3::legacy(field_8)]] IdentityType Identity;
    [[bg3::legacy(field_10)]] Guid Race;
    [[bg3::legacy(field_20)]] BodyType BodyType;

    inline bool operator == (AttitudeIdentifier const& o) const
    {
        return Character == o.Character
            && Identity == o.Identity
            && Race == o.Race
            && BodyType == o.BodyType;
    }
};

struct AttitudesToPlayersComponent : public BaseComponent
{
    DEFINE_COMPONENT(AttitudesToPlayers, "eoc::attitude::AttitudesToPlayersComponent")

    HashMap<AttitudeIdentifier, int> Attitudes;
};

template <>
inline uint64_t HashMapHash<AttitudeIdentifier>(AttitudeIdentifier const& v)
{
    return HashMulti(v.Character, v.Identity, v.Race, v.BodyType);
}

struct PlatformTransformData
{
    Transform Transform;
    bool FireEvents{ true };
};

struct PlatformTransformChangeRequest
{
    Transform Transform;
    bool FireEvents{ true };
    EntityHandle Platform;
};

struct PlatformSystem : public BaseSystem
{
    static constexpr nullptr_t SystemName = nullptr;

    [[bg3::hidden]] UnknownSignal qword10;
    LegacyRefMap<FixedString, EntityHandle> PlatformEntities;
    [[bg3::hidden]] UnknownFunction field_38;
    [[bg3::hidden]] void* TransformSystem;
    Array<PlatformTransformChangeRequest> TransformChanged;
    Array<EntityHandle> UpdateSubgrids;
};

END_SE()

BEGIN_NS(eoc::background)

struct GoalRecord
{
    Guid Goal;
    Guid Entity;
    FixedString CategoryId;
    uint64_t field_28;
};

struct GoalsComponent : public BaseComponent
{
    DEFINE_COMPONENT(BackgroundGoals, "eoc::background::GoalsComponent")

    HashMap<Guid, Array<GoalRecord>> Goals;
};

END_NS()

BEGIN_NS(eoc::exp)

struct ExperienceComponent : public BaseComponent
{
    DEFINE_COMPONENT(Experience, "eoc::exp::ExperienceComponent")

    int CurrentLevelExperience;
    int NextLevelExperience;
    int TotalExperience;
    uint8_t field_28;
};

struct AvailableLevelComponent : public BaseComponent
{
    DEFINE_COMPONENT(AvailableLevel, "eoc::exp::AvailableLevelComponent")

    int Level;
};

DEFINE_TAG_COMPONENT(eoc::exp, CanLevelUpComponent, CanLevelUp)

END_NS()

BEGIN_NS(eoc::hotbar)

struct Slot
{
    EntityHandle Item;
    SpellId SpellId;
    FixedString Passive;
    uint32_t Slot;
    bool IsNew{ false };
};

struct Bar
{
    uint8_t Index{ 0 };
    uint8_t field_1{ 0 };
    Array<Slot> Elements;
    uint8_t Width{ 1 };
    uint32_t Height{ 1 };
    STDString field_20;
};

struct ContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(HotbarContainer, "eoc::hotbar::ContainerComponent")

    HashMap<FixedString, Array<Bar>> Containers;
    FixedString ActiveContainer;
};

struct HotbarCurrentDecksComponent : public BaseComponent
{
    DEFINE_COMPONENT(HotbarDecks, "eoc::hotbar::CurrentDecksComponent")

    HashMap<FixedString, int32_t> Decks;
};

END_NS()

BEGIN_NS(eoc::calendar)

struct DaysPassedComponent : public BaseComponent
{
    DEFINE_COMPONENT(CalendarDaysPassed, "eoc::calendar::DaysPassedComponent")

    int Days;
};

struct StartingDateComponent : public BaseComponent
{
    DEFINE_COMPONENT(CalendarStartingDate, "eoc::calendar::StartingDateComponent")

    int Day;
    int Year;
};

END_NS()

BEGIN_NS(eoc::improvised_weapon)

struct WieldedComponent : public BaseComponent
{
    DEFINE_COMPONENT(ImprovisedWeaponWielded, "eoc::improvised_weapon::WieldedComponent")

    EntityHandle Wielder;
    uint8_t field_8;
    uint8_t field_9;
};

struct WieldingComponent : public BaseComponent
{
    DEFINE_COMPONENT(ImprovisedWeaponWielding, "eoc::improvised_weapon::WieldingComponent")

    EntityHandle Weapon;
};

DEFINE_TAG_COMPONENT(eoc::improvised_weapon, CanBeWieldedComponent, CanBeWielded)

END_NS()

BEGIN_NS(eoc::pickup)

struct PickUpRequestComponent : public BaseComponent
{
    DEFINE_COMPONENT(PickUpRequest, "eoc::pickup::PickUpRequestComponent")

    Guid field_0;
    uint8_t State;
};

DEFINE_TAG_COMPONENT(eoc::pickup, PickUpExecutingComponent, PickUpExecuting)

END_NS()

BEGIN_NS(eoc::rest)

struct LongRestStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(LongRestState, "eoc::rest::LongRestState")

    LongRestState State;
    bool WaitingForOthers;
    bool FinishConfirmed;
    bool Finishing;
    RestCancelReason CancelReason;
    FixedString field_8;
    EntityHandle Requester;
};

struct LongRestTimeline : public BaseComponent
{
    DEFINE_COMPONENT(LongRestTimeline, "eoc::rest::LongRestTimeline")

    Guid Timeline;
};

struct LongRestTimers : public BaseComponent
{
    DEFINE_COMPONENT(LongRestTimers, "eoc::rest::LongRestTimers")

    int field_0;
};

struct LongRestUsers : public BaseComponent
{
    DEFINE_COMPONENT(LongRestUsers, "eoc::rest::LongRestUsers")

    bool RequestRestore;
    HashMap<UserId, LongRestUserState> States;
    HashSet<UserId> FinishConfirmation;
};

struct RestingEntities : public BaseComponent
{
    DEFINE_COMPONENT(RestingEntities, "eoc::rest::RestingEntities")

    HashSet<EntityHandle> ScriptFinished;
    HashSet<EntityHandle> RestRequested;
    HashSet<EntityHandle> RestStarted;
    HashSet<EntityHandle> Party;
    bool HasSurfaces;
};


DEFINE_TAG_COMPONENT(eoc::rest, ShortRestComponent, ShortRest)
DEFINE_TAG_COMPONENT(eoc::rest, LongRestInScriptPhase, LongRestInScriptPhase)

END_NS()

BEGIN_NS(esv::rest)

struct PendingTypeComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRestPendingType, "esv::rest::PendingTypeComponent")

    RestCancelReason CancelReason;
    HashMap<EntityHandle, int> Supplies;
};

struct RestTypeChosenEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerRestTypeChosenEvent, "esv::rest::RestTypeChosenEventOneFrameComponent")

    RestCancelReason CancelReason;
    HashMap<EntityHandle, int> Supplies;
    bool UserDecision;
};

struct ShortRestResultEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerShortRestResultEvent, "esv::rest::ShortRestResultEventOneFrameComponent")

    bool Rested;
    RestErrorFlags ErrorFlags;
};


DEFINE_TAG_COMPONENT(esv::rest, ShortRestConsumeResourcesComponent, ShortRestConsumeResources)

struct LongRestSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerLongRest, "esv::rest::LongRestSystem")

    EntityHandle RestRequester;
    bool RestConfirmed;
    bool Pending;
    bool RequestSyncedFinish;
    bool RequestReloadStory;
    bool RequestUserStateRestore;
    bool RequestLongRest;
    Array<bool> AutoConfirmation;
    HashSet<Guid> SetTimeline;
    Array<FixedString> ScriptDeny;
    HashSet<EntityHandle> Finish;
    HashSet<UserId> FinishConfirmation;
    [[bg3::hidden]] void* LongRestUnitTestHelper;
    [[bg3::hidden]] void* EocServer;
    uint8_t UserDecisionRestType;
    HashMap<EntityHandle, int> RestSupplies;
    bool UserDecision;
    std::optional<bool> SetPlayersAreDreaming;
};

struct ShortRestSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerShortRest, "esv::rest::ShortRestSystem")

    [[bg3::hidden]] UnknownSignal qword10;
    [[bg3::hidden]] UnknownSignal qword28;
    HashMap<EntityHandle, bool> UIRequest;
    HashMap<EntityHandle, bool> ReConRequest;
    HashMap<EntityHandle, bool> FunctorRequest;
};

END_NS()

BEGIN_NS(heal)

DEFINE_TAG_COMPONENT(eoc::heal, BlockComponent, HealBlock)

END_NS()

BEGIN_NS(esv)

struct GodSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerGod, "esv::god::GodSystem")

    [[bg3::hidden]] void* GodManager;
    HashMap<EntityHandle, std::optional<Guid>> GodOverrides;
};

struct RestoreRequest
{
    EntityHandle Entity;
    bool field_8{ true };
};

struct RestoreSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerRestore, "esv::restore::RestoreSystem")

    Array<RestoreRequest> PartyRestore;
};

struct BodyTypeSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerBodyType, "esv::BodyTypeSystem")

    [[bg3::hidden]] UnknownFunction qword10;
    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* ShapeshiftSystem;
    Array<EntityHandle> Create;
    Array<EntityHandle> Update;
    Array<EntityHandle> CharacterCreationUpdate;
};

struct EvaluateDualWieldingRequest
{
    DualWieldingRequestType Type{ DualWieldingRequestType::ToggleDualWielding };
    EntityHandle Entity;
    StatsItemSlot Slot{ StatsItemSlot::Sentinel };
};

struct DualWieldingSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerDualWielding, "esv::DualWieldingSystem")

    [[bg3::hidden]] void* ActionResourceSystem;
    [[bg3::hidden]] void* BoostSystem;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] UnknownFunction qword28;
    Array<EvaluateDualWieldingRequest> Requests;
};

struct SetTeleportFlagRequest
{
    bool SnapToGrid{ true };
    glm::vec3 OriginTeleportPos;
};

struct FallingSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerFalling, "esv::FallingSystem")

    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* FallingUnitTestHelper;
    HashSet<EntityHandle> ResetFallHeight;
    HashSet<EntityHandle> StartFalling;
    HashSet<EntityHandle> StopFalling;
    HashMap<EntityHandle, float> SetLifetime;
    HashMap<EntityHandle, SetTeleportFlagRequest> SetTeleportFlag;
    HashMap<EntityHandle, bool> DeferredStopFalling;
};

struct DisplayNameRequest
{
    EntityHandle Entity;
    TranslatedString DisplayName;
    DisplayNameType Type{ DisplayNameType::Unknown };
};

struct DisplayTitleRequest
{
    EntityHandle Entity;
    TranslatedString DisplayName;
    DisplayTitleType Type{ DisplayTitleType::Transform };
};

struct DisplayNameSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerDisplayName, "esv::DisplayNameSystem")

    Array<DisplayNameRequest> SetDisplayName;
    Array<DisplayTitleRequest> SetDisplayTitle;
    HashSet<EntityHandle> DirtyName;
    HashSet<EntityHandle> DirtyTitle;
};

struct SetGravityActiveRequest
{
    bool Active{ true };
    EntityHandle Instigator;
};

struct GravitySystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerGravity, "esv::GravitySystem")

    [[bg3::hidden]] UnknownFunction qword10;
    [[bg3::hidden]] void* GravitySystemHelper;
    HashMap<EntityHandle, SetGravityActiveRequest> SetGravityActive;
    HashMap<EntityHandle, GravityType> SetGravityType;
    Array<EntityHandle> InstigatorTagChange;
    Array<EntityHandle> InstigatorTagChange2;
};

struct LeaderSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerLeader, "esv::LeaderSystem")

    HashMap<EntityHandle, HashSet<EntityHandle>> AddFollower;
    HashMap<EntityHandle, HashSet<EntityHandle>> RemoveFollower;
};

struct TeleportPartyRequest
{
    std::variant<EntityHandle, FixedString> LevelOrEntity;
    STDString Event;
    FixedString Movie;
};

struct PartyTeleportSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerPartyTeleport, "esv::PartyTeleportSystem")

    [[bg3::hidden]] void* GameEventListenerVMT;
    [[bg3::hidden]] void* EocServer;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* GameControl;
    bool DelayedTeleport;
    TeleportPartyRequest Teleport;
};

struct PingRequestData
{
    EntityHandle Source;
    EntityHandle Target;
    glm::vec3 Position;
    bool field_1C{ false };
};

struct UIPingRequestData
{
    int16_t PingId;
    EntityHandle Source;
    EntityHandle Target;
    glm::vec3 Position;
    bool field_24{ false };
};

struct OsirisPingRequestSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerOsirisPingRequestSingleton, "esv::OsirisPingRequestSingletonComponent")

    Array<PingRequestData> Pings;
};

struct PingCooldownSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerPingCooldownSingleton, "esv::PingCooldownSingletonComponent")

    HashMap<int16_t, float> Cooldowns;
};

struct PingRequestSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerPingRequestSingleton, "esv::PingRequestSingletonComponent")

    Array<PingRequestData> Pings;
};

struct PingRequestSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerPingRequest, "esv::PingRequestSystem")

    [[bg3::hidden]] void* RPGStats;
    Array<UIPingRequestData> UIPings;
    Array<PingRequestData> Pings;
};

struct PlatformSetAuthorityTransformRequest
{
    bool FireEvents{ true };
    Transform Transform;
    EntityHandle Platform;
};

struct PlatformSystem : public bg3se::PlatformSystem
{
    DEFINE_SYSTEM(ServerPlatform, "esv::PlatformSystem")

    [[bg3::hidden]] void* GameEventListener;
    [[bg3::hidden]] void* field_B0;
    [[bg3::hidden]] void* NetEventListener;
    HashMap<EntityHandle, bg3se::death::DeathData> DestroyRequests;
    Array<PlatformSetAuthorityTransformRequest> SetAuthorityTransform;
    [[bg3::hidden]] void* AnubisConfigSystem;
    [[bg3::hidden]] void* LevelInstanceAttachRequestSystem;
    [[bg3::hidden]] void* LevelInstanceLoadRequestSystem;
    [[bg3::hidden]] void* LevelManager;
    bool Activated;
};

END_NS()

BEGIN_NS(esv::approval)

struct RatingsChangedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerRatingsChanged, "esv::approval::RatingsChangedOneFrameComponent")

    EntityHandle Subject;
    EntityHandle Avatar;
    int FinalRating;
    int Adjustment;
    int RealAdjustment;
    ApprovalReactionScope ReactionScope;
    Guid field_20;
};

struct RatingChangeRequest
{
    EntityHandle Subject;
    EntityHandle Avatar;
    int Adjustment{ 0 };
    ApprovalReactionScope ReactionScope{ ApprovalReactionScope::Global };
    Guid field_18;
    Guid field_28;
};

struct RatingRemoveRequest
{
    EntityHandle Subject;
    EntityHandle Avatar;
};

struct RatingSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerRating, "esv::approval::RatingSystem")

    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* GlobalTemplateManager;
    [[bg3::hidden]] void* CacheTemplateManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] UnknownFunction qword30;
    [[bg3::hidden]] void* RatingSystemHelper;
    Array<RatingChangeRequest> ChangeRating;
    Array<RatingRemoveRequest> RemoveRating;
    HashSet<EntityHandle> CreateRatingsComponent;
};

END_NS()

BEGIN_NS(esv::attitude)

struct UpdateRequest
{
    EntityHandle Avatar;
    int Attitude;
};

struct UpdateSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerAttitude, "esv::attitude::UpdateSystem")

    Array<EntityHandle> CreateAttitudesComponent;
    HashMap<EntityHandle, Array<UpdateRequest>> OsirisAddAttitude;
    HashMap<EntityHandle, Array<UpdateRequest>> ReConAddAttitude;
};

END_NS()
