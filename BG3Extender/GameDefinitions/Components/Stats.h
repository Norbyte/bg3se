#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct ArmorComponent : public BaseComponent
{
    DEFINE_COMPONENT(Armor, "eoc::ArmorComponent")

    int ArmorType;
    int ArmorClass;
    int AbilityModifierCap;
    uint8_t ArmorClassAbility;
    EEquipmentType EquipmentType;
};

struct ArmorSetStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ArmorSetState, "eoc::armor_set::StateComponent")

    ArmorSetState State;
};

struct AttributeFlagsComponent : public BaseComponent
{
    DEFINE_COMPONENT(AttributeFlags, "eoc::AttributeFlagsComponent")

    uint32_t AttributeFlags;
};

struct BaseHpComponent : public BaseComponent
{
    DEFINE_COMPONENT(BaseHp, "eoc::BaseHpComponent")

    int Vitality;
    int VitalityBoost;
};

struct ClassInfo
{
    Guid ClassUUID;
    Guid SubClassUUID;
    int Level;
};

struct ClassesComponent : public BaseComponent
{
    DEFINE_COMPONENT(Classes, "eoc::ClassesComponent")

    Array<ClassInfo> Classes;
};

struct DataComponent : public BaseComponent
{
    DEFINE_COMPONENT(Data, "eoc::DataComponent")

    int32_t Weight;
    FixedString StatsId;
    uint32_t StepsType;
};

struct EncumbranceStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(EncumbranceState, "eoc::encumbrance::StateComponent")

    uint32_t State;
};

struct EncumbranceStatsComponent : public BaseComponent
{
    DEFINE_COMPONENT(EncumbranceStats, "eoc::encumbrance::StatsComponent")

    [[bg3::legacy(field_0)]] int UnencumberedWeight;
    [[bg3::legacy(field_4)]] int EncumberedWeight;
    [[bg3::legacy(field_8)]] int HeavilyEncumberedWeight;
};

struct EquipableComponent : public BaseComponent
{
    DEFINE_COMPONENT(Equipable, "eoc::EquipableComponent")

    [[bg3::legacy(field_18)]] Guid EquipmentTypeID;
    ItemSlot Slot;
};

struct HearingComponent : public BaseComponent
{
    DEFINE_COMPONENT(Hearing, "eoc::HearingComponent")

    float Hearing;
};

struct EocLevelComponent : public BaseComponent
{
    DEFINE_COMPONENT(EocLevel, "eoc::LevelComponent")

    int Level;
};

struct ResistancesComponent : public BaseComponent
{
    DEFINE_COMPONENT(Resistances, "eoc::ResistancesComponent")

    std::array<ResistanceBoostFlags, 14> Resistances;
    uint8_t field_E;
    int AC;
    std::array<uint16_t, 14> PerDamageTypeHealthThresholds;
    std::array<uint32_t, 14> PerDamageTypeHealthThresholds2;
};

struct StatsComponent : public BaseComponent
{
    DEFINE_COMPONENT(Stats, "eoc::StatsComponent")

    int InitiativeBonus;
    std::array<int, 7> Abilities;
    std::array<int, 7> AbilityModifiers;
    std::array<int, 18> Skills;
    int ProficiencyBonus;
    AbilityId SpellCastingAbility;
    int field_8C;
    int field_90;
    int ArmorType;
    int ArmorType2;
    AbilityId UnarmedAttackAbility;
    AbilityId RangedAttackAbility;
};

struct UseComponent : public BaseComponent
{
    DEFINE_COMPONENT(Use, "eoc::UseComponent")

    Array<stats::Requirement> Requirements;
    int Charges;
    int MaxCharges;
    ItemUseType ItemUseType;
    [[bg3::legacy(field_19)]] uint8_t ItemUseBlocked;
    [[bg3::legacy(field_1A)]] uint8_t CanCombine;
    [[bg3::legacy(field_1B)]] uint8_t CombineFlag;
    Array<BoostDescription> Boosts;
    Array<BoostDescription> BoostsOnEquipMainHand;
    Array<BoostDescription> BoostsOnEquipOffHand;
};

struct ValueComponent : public BaseComponent
{
    DEFINE_COMPONENT(Value, "eoc::ValueComponent")

    int Value;
    uint8_t Rarity;
    bool Unique;
};

struct WeaponComponent : public BaseComponent
{
    DEFINE_COMPONENT(Weapon, "eoc::WeaponComponent")

    LegacyRefMap<AbilityId, Array<RollDefinition>> Rolls;
    LegacyRefMap<AbilityId, Array<RollDefinition>> Rolls2;
    float WeaponRange;
    float DamageRange;
    stats::Functors* WeaponFunctors;
    uint32_t WeaponProperties;
    uint8_t WeaponGroup;
    AbilityId Ability;
    [[bg3::legacy(field_38)]] Array<StatsExpressionWithMetadata> DamageValues;
    DiceSizeId DamageDice;
    DiceSizeId VersatileDamageDice;
};

struct BaseStatsSystem : public BaseSystem
{
    static constexpr nullptr_t SystemName = nullptr;

    [[bg3::hidden]] UnknownSignal ObjectHealed;
    [[bg3::hidden]] UnknownSignal CurrentHitPointsChanged;
    [[bg3::hidden]] UnknownSignal CurrentTemporaryHitPointsChanged;
    [[bg3::hidden]] UnknownSignal ObjectHit;
    [[bg3::hidden]] UnknownSignal ObjectDestroyed;
    [[bg3::hidden]] UnknownSignal LevelChanged;
    [[bg3::hidden]] UnknownSignal ObjectThrown;
    [[bg3::hidden]] UnknownSignal MaxSpellsChanged;
    [[bg3::hidden]] void* ThothSharedContext;
    [[bg3::hidden]] void* ClassDescriptions;
    [[bg3::hidden]] void* StreamRollManager;
    [[bg3::hidden]] void* SpellPrototypes;
    [[bg3::hidden]] void* StatusPrototypes;
    [[bg3::hidden]] void* LevelMapValueProvider;
    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* RpgStats;
};

END_SE()

BEGIN_NS(esv)

struct [[bg3::hidden]] StatsRollStreamProvider
{
    void* VMT;
    ecs::EntityWorld* EntityWorld;
    void* EocServer;
    void* StreamRollManager;
    void* FactionContainer;
    void* GlobalSwitches;
};

struct UseBoostUpdateRequest
{
    EntityHandle Boost;
    BoostSource Source;
    Guid field_20;
    uint64_t field_30;
    bool IsNew;
};

struct StatsEntityDirtyData
{
    HashSet<EntityHandle> Boosts;
    int SetCurrentHP;
    int field_34;
    __int64 field_38;
    Array<UseBoostUpdateRequest> UseBoostRequests;
};

struct StatsSystem : public BaseStatsSystem
{
    DEFINE_SYSTEM(ServerStats, "esv::StatsSystem")

    [[bg3::hidden]] UnknownSignal EquipmentUpdated;
    [[bg3::hidden]] UnknownSignal WeightChanged;
    [[bg3::hidden]] UnknownSignal DirtyFlagsChanged;
    [[bg3::hidden]] void* BoostSystem;
    [[bg3::hidden]] void* RpgStats;
    [[bg3::hidden]] void* EocServer;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* GlobalSwitches;
    [[bg3::hidden]] void* WeightCategories;
    [[bg3::hidden]] StatsRollStreamProvider RollStream;
    HashMap<EntityHandle, StatsDirtyFlags> CalculationRequests;
    HashMap<EntityHandle, StatsDirtyFlags> DeferredCalculationRequests;
    LegacyRefMap<EntityHandle, LegacyRefMap<uint64_t, StatsEntityDirtyData>> EntityDirtyData;
    HashMap<EntityHandle, Guid> SetEquipmentType;
    HashSet<EntityHandle> EncumbranceChanged;
    HashSet<EntityHandle> ReloadStats;
    HashSet<EntityHandle> StatsApplied;
    HashSet<EntityHandle> EquipmentModified;
    HashMap<EntityHandle, HashSet<ItemSlot>> EquipmentChanged;
    [[bg3::hidden]] UnknownFunction field_398;
};

END_NS()
