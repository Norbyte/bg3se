#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct PassiveContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(PassiveContainer, "eoc::PassiveContainerComponent")

    Array<EntityHandle> Passives;
};

struct PassiveComponent : public BaseComponent
{
    DEFINE_COMPONENT(Passive, "eoc::PassiveComponent")

    [[bg3::legacy(field_0)]] PassiveSourceType Type;
    FixedString PassiveId;
    [[bg3::legacy(field_8)]] EntityHandle Source;
    [[bg3::legacy(field_10)]] EntityHandle Item;
    [[bg3::legacy(field_18)]] bool ToggledOn;
    [[bg3::legacy(field_19)]] bool Disabled;
    uint32_t field_1C;
};

struct PassiveUsageCount
{
    uint16_t field_0{ 0 };
    uint16_t field_2{ 0 };
    uint16_t field_4{ 0 };
    uint16_t field_6{ 0 };
    uint16_t field_8{ 0 };
};

struct UsageCountComponent : public BaseComponent
{
    DEFINE_COMPONENT(PassiveUsageCount, "eoc::passive::UsageCountComponent")

    HashMap<FixedString, PassiveUsageCount> Passives;
};

END_SE()

BEGIN_NS(esv::passive)

struct ToggledPassivesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerToggledPassives, "esv::passive::ToggledPassivesComponent")

    HashMap<FixedString, bool> Passives;
};

struct BoostsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerPassiveBoosts, "esv::passive::BoostsComponent")

    [[bg3::legacy(field_0)]] Array<Guid> Boosts;
    std::optional<std::variant<stats::AttackTargetContextData, stats::AttackPositionContextData, stats::MoveContextData, stats::SourceContextData, stats::TargetContextData, stats::NearbyAttackedContextData, stats::NearbyAttackingContextData, stats::EquipContextData>> ContextData;
};

struct PassiveBaseComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerPassiveBase, "esv::passive::BaseComponent")

    [[bg3::legacy(field_0)]] Array<FixedString> Passives;
};

struct PersistentDataComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerPassivePersistentData, "esv::passive::PersistentDataComponent")

    float field_0;
    float field_4;
};

struct ScriptPassivesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerScriptPassives, "esv::passive::ScriptPassivesComponent")

    HashSet<FixedString> Passives;
};

struct RequestTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(PassiveRequestTargetTracking, "esv::passive::RequestTargetTrackingOneFrameComponent")

    Guid field_0;
    HashSet<EntityHandle> Targets;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(PassiveUpdateTargetTracking, "esv::passive::UpdateTargetTrackingOneFrameComponent")

    Guid field_0;
    HashSet<EntityHandle> Targets;
};

struct PostponedConditionalRoll
{
    uint64_t RollIndex{ 0 };
    stats::ConditionId Condition;
    ConditionRoll Roll;
    uint8_t field_148{ 0 };
};

struct ConditionalRollInterruptEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(PassiveConditionalRollInterruptEvent, "esv::passive::ConditionalRollInterruptEventOneFrameComponent")

    [[bg3::legacy(field_0)]] Guid RollUuid;
    [[bg3::legacy(field_10)]] EntityHandle Target;
    [[bg3::legacy(field_18)]] EntityHandle Source;
    [[bg3::legacy(field_20)]] EntityHandle Passive;
    [[bg3::legacy(field_28)]] FixedString PassiveId;
    Array<PostponedConditionalRoll> PostponedRolls;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::passive, PassivesUpdatedEventOneFrameComponent, PassivesUpdatedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::passive, UsageCountIncrementedEventOneFrameComponent, PasssiveUsageCountIncrementedEvent)

struct BasicRollInfo : public StatsRoll
{
    ConditionRollType RollType{ 0 };
    int DC{ 0 };
};

struct ExecutePassiveRequest
{
    EntityHandle Passive;
    std::variant<stats::AttackTargetContextData, stats::AttackPositionContextData, stats::MoveContextData, stats::SourceContextData, stats::TargetContextData, stats::NearbyAttackedContextData, stats::NearbyAttackingContextData, stats::EquipContextData> Context;
    Guid RollGuid;
    uint8_t field_338{ 0 };
};

struct ChangePassiveRequest
{
    EntityHandle Entity;
    FixedString Passive;
    EntityHandle Source;
    PassiveSourceType PassiveSourceType{ PassiveSourceType::Debug };
    uint8_t PassiveChange{ 0 };
};

struct PassiveSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerPassive, "esv::PassiveSystem")

    [[bg3::hidden]] void* TurnOrderSystem;
    [[bg3::hidden]] void* FTBZoneSystem;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* DarknessSystem;
    [[bg3::hidden]] void* StatsRollNotificationSystem;
    [[bg3::hidden]] void* FeatManager;
    [[bg3::hidden]] void* RPGStats;
    [[bg3::hidden]] void* Passives;
    [[bg3::hidden]] void* StatusFactory;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* ProgressionManager;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    Array<ExecutePassiveRequest> ExecutePassives;
    Array<ChangePassiveRequest> ChangePassive;
    HashSet<EntityHandle> field_A0;
    HashSet<EntityHandle> field_D0;
    HashSet<EntityHandle> TogglePassives;
    HashSet<EntityHandle> LongRest;
    HashSet<EntityHandle> WieldingAddPassives;
    HashSet<EntityHandle> AddPassives;
    HashMap<EntityHandle, HashSet<EntityHandle>> WieldingChangedPassives;
    HashSet<EntityHandle> WieldingRemovePassives;
    HashSet<EntityHandle> RemovePassives;
    HashSet<EntityHandle> ReloadStatsPassives;
    HashSet<EntityHandle> UsageCountReset;
    HashSet<EntityHandle> ObscurityChanged;
    HashSet<EntityHandle> SurfaceEntered;
    HashSet<EntityHandle> RemovedPassives;
    HashSet<EntityHandle> AddedPassives;
    HashMap<EntityHandle, Array<SpellCooldownType>> ResetUsageCount;
    HashMap<Guid, BasicRollInfo> ActiveRollPassiveRequests;
    bool ReloadAllStatusPassives;
    bool ReloadPassivesBoosts;
    [[bg3::hidden]] UnknownFunction field_408;
};

END_NS()
