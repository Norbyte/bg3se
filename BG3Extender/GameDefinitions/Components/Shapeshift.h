#pragma once

BEGIN_NS(esv::shapeshift)

struct HealthReservationComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftHealthReservation, "esv::shapeshift::HealthReservationComponent")

    HashMap<Guid, int32_t> Reservations;
};

struct IntOverride
{
    int Value;
    ShapeshiftChangeType Type;
};

struct FloatOverride
{
    float Value;
    ShapeshiftChangeType Type;
};

struct RootTemplateOverride
{
    FixedString RootTemplate;
    uint8_t field_114;
};

struct State
{
    [[bg3::legacy(field_0)]] Guid StateGuid;
    std::array<std::optional<IntOverride>, 7> Abilities;
    std::optional<IntOverride> Vitality;
    std::optional<IntOverride> VitalityBoost;
    std::optional<FixedString> HotBar;
    std::optional<FloatOverride> Weight;
    std::optional<uint32_t> StepsType;
    std::optional<Guid> Race;
    std::optional<uint8_t> Gender;
    std::optional<bool> UseShapeshiftIdentity;
    uint8_t field_B4{ 0 };
    bool KillEntityAtZeroHP{ false };
    Array<Guid> TagsAdd;
    Array<Guid> TagsRemove;
    bool RemoveOldTags{ false };
    std::optional<uint16_t> ActionCapabilities;
    std::optional<uint16_t> InteractionsCapabilities;
    std::optional<uint16_t> AwarenessCapabilities;
    std::optional<uint16_t> SpeakingCapabilities;
    std::optional<STDString> ChangeScript;
    std::optional<RootTemplateOverride> RootTemplate;
    std::optional<TranslatedString> DisplayName;
    std::optional<TranslatedString> Title;
    std::optional<FixedString> Icon;
    std::optional<FloatOverride> Scale;
    std::optional<EntityHandle> field_158;
    bool ApplyVisualInCC{ false };
    bool MuteEquipmentSound{ false };
    bool OverrideSpeaker{ false };
    HashMap<ShapeshiftSpellSource, Array<FixedString>> SpellsAdd;
    Array<FixedString> SpellsRemove;
    bool RemovePrevSpells{ false };
    std::optional<Guid> AiHint;
    std::optional<FixedString> CombatArchetype;
    std::optional<int32_t> Armor;
    std::optional<int32_t> ArmorType;
    std::optional<AbilityId> UnarmedAttackAbility;
    std::optional<AbilityId> UnarmedRangedAttackAbility;
    uint8_t field_1FC{ 0 };
    uint8_t field_1FD{ 0 };
    std::optional<bool> CanSeeThrough;
    std::optional<bool> CanShootThrough;
    std::optional<uint8_t> ShootThroughType;
    std::optional<bool> WalkThrough;
    Array<SurfacePathInfluence> SurfacePathInfluences;
    uint8_t FlagsInheritanceType{ 0 };
    uint32_t Flags{ 0 };
    uint8_t DefaultBoostsInheritanceType{ 0 };
    FixedString DefaultBoosts;
    uint8_t PersonalStatusImmunitiesInheritanceType{ 0 };
    FixedString PersonalStatusImmunities;
    uint8_t PassivesInheritanceType{ 0 };
    FixedString Passives;
    uint8_t ResistancesInheritanceType{ 0 };
    std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
    uint8_t DifficultyStatusesInheritanceType{ 0 };
    FixedString DifficultyStatuses;
    uint8_t ProficiencyGroupsInheritanceType{ 0 };
    ProficiencyGroupFlags ProficiencyGroup;
    uint8_t BloodSurfaceTypeInheritanceType{ 0 };
    uint8_t HitBloodTypeInheritanceType{ 0 };
    uint8_t DeathActionsInheritanceType{ 0 };
    std::optional<AbilityId> SpellCastingAbility;
    std::optional<Guid> DummyClass;
    bool BlockLevelUp{ false };
};

struct StatesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerShapeshiftStates, "esv::shapeshift::StatesComponent")

    int field_0;
    Array<State> States;
};

struct AddRequest
{
    Guid StateGuid;
    std::optional<TemplateInfo> Template;
    EntityHandle TargetTemplate;
    bool field_28{ false };
    Guid ShapeshiftRule;
    bool ApplyToDead{ false };
};

struct System : public BaseSystem
{
    DEFINE_SYSTEM(ServerShapeshift, "esv::shapeshift::System")

    [[bg3::hidden]] UnknownSignal field_10;
    HashMap<EntityHandle, Array<AddRequest>> RequestAddStates;
    HashMap<EntityHandle, Array<Guid>> RequestDeleteStates;
    HashSet<EntityHandle> RequestDeleteAllStates;
    [[bg3::hidden]] void* ShapeshiftRulebook;
    [[bg3::hidden]] void* RPGStats;
};


END_NS()
