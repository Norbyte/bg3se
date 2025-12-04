#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>

BEGIN_SE()

struct RequestedRollComponent : public BaseComponent
{
    DEFINE_COMPONENT(RequestedRoll, "eoc::RequestedRollComponent")

    EntityHandle RollEntity;
    Guid RollUuid;
    uint8_t RollComponentType;
    uint8_t RollType;
    [[bg3::legacy(field_20)]] EntityHandle Roller;
    STDString StatsExpression;
    AdvantageBoostType AdvantageType;
    [[bg3::legacy(AC)]] uint8_t DC;
    uint8_t NaturalRoll;
    uint8_t DiscardedDiceTotal;
    uint8_t DiceAdditionalValue;
    int AdditionalValue;
    uint8_t SpellCheck;
    bool Finished;
    [[bg3::legacy(field_4E)]] bool Canceled;
    uint8_t field_4F;
    float PassiveRollDelay;
    uint8_t RollContext;
    [[bg3::legacy(field_55)]] bool FromScript;
    [[bg3::legacy(field_58)]] int DialogId;
    [[bg3::legacy(field_60)]] EntityHandle Subject;
    Array<ResolvedRollBonus> ResolvedRollBonuses;
    Array<FixedRollBonus> FixedRollBonuses;
    [[bg3::legacy(field_88)]] bool RolledComponentType0;
    uint8_t field_89;
    uint8_t field_8A;
    [[bg3::legacy(field_90)]] Array<Guid> ExcludedBonusesTags;
    AbilityId Ability;
    SkillId Skill;
    [[bg3::legacy(field_A2)]] bool ConsumedInspirationPoint;
    Roll Roll;
    StatsRollResult Result;
    StatsRollMetadata Metadata;
    STDString field_1B0;
    [[bg3::legacy(PassiveRollDelay2)]] float RollDelayRemaining;
    [[bg3::legacy(field_1CC)]] bool RequestStop;
    Guid EntityUuid;
    Guid Entity2Uuid;
};

END_SE()

BEGIN_NS(active_roll)

DEFINE_TAG_COMPONENT(eoc::active_roll, InProgressComponent, RollInProgress)

struct DynamicModifierId
{
    Guid ModifierGuid;
    DynamicModifierType Type{ DynamicModifierType::None };
};


struct ModifierSourceInfo
{
    BoostSourceType SourceType{ BoostSourceType::Undefined };
    FixedString Cause;
    FixedString StackId;
    int StackPriority{ 0 };
    [[bg3::legacy(field_10)]] EntityHandle Equipment;
    [[bg3::legacy(field_18)]] ComponentHandle Status;
    uint8_t field_20{ 0 };
    EntityHandle field_28;
    EntityHandle Source;
};


struct Modifier
{
    [[bg3::legacy(AbilityModifier)]] int Value{ 0 };
    [[bg3::legacy(AbilityModifier2)]] int TotalValue{ 0 };
    AdvantageBoostType AdvantageType{ AdvantageBoostType::None };
    uint8_t Advantage{ 0 };
    CriticalHitSuccessFlags SuccessFlags{ 0 };
    DiceSizeId DiceValue{ DiceSizeId::Default };
    uint8_t AmountOfDices{ 0 };
    BoostType BoostType{ 0 };
    uint8_t TargetType{ 0 };
    [[bg3::legacy(field_F)]] RollCritical CriticalHitType{ RollCritical::None };
    uint8_t field_28{ 0 };
    [[bg3::legacy(field_2C)]] TranslatedString AdvantageReason;
    Guid field_40;
    ModifierSourceInfo Source;
};

struct StaticModifier
{
    Guid ModifierGuid;
    [[bg3::legacy(field_10)]] bool Disabled{ false };
    Modifier Modifier;
    uint8_t field_88{ 0 };
};

struct ConsumableModifier
{
    Guid ModifierGuid;
    EntityHandle Source;
    EntityHandle Item;
    Array<Modifier> Modifiers;
};

struct ItemSpellModifier
{
    Guid ModifierGuid;
    SpellId Spell;
    EntityHandle Source;
    EntityHandle Item;
    uint8_t TargetType{ 0 };
    bool Concentration{ false };
    Array<Modifier> Modifiers;
};

struct SpellModifier
{
    Guid ModifierGuid;
    SpellId Spell;
    EntityHandle Source;
    uint8_t TargetType{ 0 };
    bool Concentration{ false };
    Array<Modifier> Modifiers;
};

struct ToggledPassiveModifier
{
    Guid ModifierGuid;
    FixedString Passive;
    int field_14{ 0 };
    EntityHandle Source;
    EntityHandle PassiveEntity;
    Array<Modifier> Modifiers;
};

struct ModifiersComponent : public BaseComponent
{
    DEFINE_COMPONENT(RollModifiers, "eoc::active_roll::ModifiersComponent")

    [[bg3::legacy(field_0)]] EntityHandle Entity;
    Array<DynamicModifierId> DynamicModifiers;
    uint8_t field_18;
    uint8_t field_19;
    Array<StaticModifier> StaticModifiers;
    Array<DynamicModifierId> DynamicModifiers2;
    Array<ConsumableModifier> ConsumableModifiers;
    Array<ItemSpellModifier> ItemSpellModifiers;
    Array<SpellModifier> SpellModifiers;
    Array<ToggledPassiveModifier> ToggledPassiveModifiers;
    Array<DynamicModifierId> DynamicModifiers3;
};

END_NS()


BEGIN_NS(esv::active_roll)

struct InProgressComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRollInProgress, "esv::active_roll::InProgressComponent")

    glm::vec3 Position;
    EntityHandle Entity;
};

struct StartRequestOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRollStartRequest, "esv::active_roll::StartRequestOneFrameComponent")

    Guid field_0;
    uint8_t field_10;
};

struct StartSpellRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerRollStartSpellRequest, "esv::active_roll::StartSpellRequestOneFrameComponent")

    SpellId Spell;
    [[bg3::legacy(field_28)]] uint32_t Flags;
    [[bg3::legacy(field_30)]] EntityHandle Caster;
    Array<bg3se::spell_cast::InitialTarget> Targets;
    ActionOriginator Originator;
    [[bg3::legacy(field_68)]] EntityHandle Source;
    EntityHandle field_70;
    std::optional<glm::vec3> Position;
    [[bg3::legacy(field_88)]] int StoryActionId;
    STDString NetGUID;
    uint8_t field_A8;
};

END_NS()


BEGIN_NS(esv::roll)

struct FinishedEvent
{
    uint8_t RollContext;
    [[bg3::legacy(Source2)]] EntityHandle Subject;
    Guid RollUuid;
    [[bg3::legacy(field_20)]] EntityHandle Roll;
    uint8_t SpellCheck;
    [[bg3::legacy(field_29)]] bool Canceled;
    [[bg3::legacy(Source)]] EntityHandle Roller;
    [[bg3::legacy(AC)]] uint32_t DC;
    [[bg3::legacy(AdvantageType1)]] bool Advantage;
    [[bg3::legacy(AdvantageType2)]] bool Disadvantage;
    SkillId Skill;
    AbilityId Ability;
    uint8_t RollType;
    int DiceAdditionalValue;
    [[bg3::legacy(field_48)]] bool ConsumedInspirationPoint;
    int NaturalRoll;
    bool RollComponentType0;
};


struct RollFinishedEventOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRollFinishedEvent, "esv::roll::RollFinishedEventOneFrameComponent")

    Array<FinishedEvent> Events;
};

struct DeathSavingThrowRequest : public ProtectedGameObject<DeathSavingThrowRequest>
{
    EntityHandle Entity;
    [[bg3::hidden]] void* StatsExpression;
    std::variant<int, Guid> DC;
};

struct RollSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerRoll, "esv::RollSystem")

    [[bg3::hidden]] void* VMT2;
    [[bg3::hidden]] void* field_18;
    [[bg3::hidden]] void* VMT_GameEventListener;
    [[bg3::hidden]] void* VMT_DialogListener;
    [[bg3::hidden]] UnknownSignal RollFinishedSignal;
    [[bg3::hidden]] void* StatsSystem;
    [[bg3::hidden]] void* ActionResourceSystem;
    [[bg3::hidden]] void* EocServer;
    [[bg3::hidden]] void* RollManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* DynamicStatsExpressionManager;
    [[bg3::hidden]] void* DifficultyClassManager;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* LevelMapValuesProvider;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* RPGStats;
    [[bg3::hidden]] void* RollSystemDataProvider;
    HashMap<EntityHandle, int32_t> ActorJoinsDialog; // -> dialogId
    HashSet<EntityHandle> field_F0;
    HashSet<EntityHandle> DestroyRoll;
    HashSet<EntityHandle> CancelRoll;
    HashSet<EntityHandle> RestartRoll2;
    HashSet<EntityHandle> RestartRoll;
    HashSet<Guid> FinishRoll;
    HashSet<Guid> StopRoll;
    Array<DeathSavingThrowRequest> DeathSavingThrows;
    bool SessionUnloadStarting;
    [[bg3::hidden]] void* field_258_StatsExpressionInternal;
    HashMap<EntityHandle, EntityHandle> EntityToRollEntity;
};

END_NS()


BEGIN_NS(eoc::roll::stream)

struct PRNGState
{
    uint64_t Frame;
    float FailDebt;
    float SuccessDebt;
    uint64_t Seed;
    uint64_t NumRolls;
};

struct Stream
{
    HashSet<EntityHandle> CombatParticipants;
    FixedString RandomCastUuid;
    Guid CombatGuid;
    bool IsAlly;
    FixedString PlayerUuid;
    RollStreamType StreamType;
    PRNGState State;
};

struct RollManager : public ProtectedGameObject<RollManager>
{
    [[bg3::hidden]] void* VMT;
    Array<Stream> Streams;
    uint64_t LastFrame;
    uint64_t BaseSeed;
    RollModeType RollModeType;
    uint8_t CustomRollExpectedValue;
    bool HasChanges;
    stats::RPGStats* Stats;
};

struct RollModeTypeRequest
{
    uint8_t CustomRollExpectedValue;
    RollModeType RollModeType;

    inline bool operator == (RollModeTypeRequest const& o) const
    {
        return o.RollModeType == RollModeType;
    }
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t HashMapHash<eoc::roll::stream::RollModeTypeRequest>(eoc::roll::stream::RollModeTypeRequest const& v)
{
    return (uint64_t)v.RollModeType;
}

END_SE()

BEGIN_NS(esv::roll::stream)

using namespace eoc::roll::stream;

struct StreamsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRollStreams, "esv::roll::stream::StreamsComponent")

    Array<Stream> Streams;
    uint64_t LastFrame;
    uint64_t BaseSeed;
    RollModeType RollModeType;
    uint8_t CustomRollExpectedValue;
};

struct CombatEventData
{
    Guid CombatGuid;
    EntityHandle Entity;
};

struct SaveSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerRollSave, "esv::roll::stream::SaveSystem")

    [[bg3::hidden]] void* VMT2;
    RollManager* RollManager;
    [[bg3::hidden]] void* CombatSystem;
    Array<CombatEventData> CombatEvents;
    HashSet<RollModeTypeRequest> RollModeTypeRequests;
    [[bg3::hidden]] UnknownFunction SignalCollection;
    bool RequestInitializeGlobals;
    bool RequestClearStreams;
};

END_NS()
