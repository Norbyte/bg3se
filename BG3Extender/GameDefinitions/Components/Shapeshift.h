#pragma once

BEGIN_NS(shapeshift)

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

struct StateComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftState, "eoc::shapeshift::StateComponent")

    std::optional<uint8_t> BloodSurfaceType;
    std::optional<FixedString> BloodType;
    [[bg3::legacy(field_C)]] std::optional<TemplateInfo> Template;
};

struct RecoveryAnimationComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftRecoveryAnimation, "eoc::shapeshift::RecoveryAnimationComponent")

    FixedString Animation;
};

struct SourceCacheComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftSourceCache, "eoc::shapeshift::SourceCacheComponent")

    Array<Guid> Source;
};

struct AnimationComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftAnimation, "eoc::shapeshift::AnimationComponent")

    uint32_t field_0;
    uint8_t field_4;
};

struct ReplicatedChangesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftReplicatedChanges, "eoc::shapeshift::ReplicatedChangesComponent")

    std::array<Array<IntOverride>, 7> Abilities;
    Array<IntOverride> HP;
    bool VisualChanged;
    // Removed? 
    // bool ForceFinish;
    uint16_t ItemTooltipFlags;
    TranslatedString ItemDescription;
    int32_t GoldAmount;
    int32_t ItemWeight;
    bool MuteEquipmentSound;
    bool DisableEquipment;
    bool HasWildShapeHotBar;
    bool HasWeightOverride;
    int32_t AC;
};

END_NS()

BEGIN_NS(esv::shapeshift)

using IntOverride = bg3se::shapeshift::IntOverride;
using FloatOverride = bg3se::shapeshift::FloatOverride;

struct HealthReservationComponent : public BaseComponent
{
    DEFINE_COMPONENT(ShapeshiftHealthReservation, "esv::shapeshift::HealthReservationComponent")

    HashMap<Guid, int32_t> Reservations;
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
    std::optional<CanDoActionsFlags> ActionCapabilities;
    std::optional<CanInteractFlags> InteractionsCapabilities;
    std::optional<AwarenessCapabilityFlags> AwarenessCapabilities;
    std::optional<SpeakingCapabilityFlags> SpeakingCapabilities;
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
    ShapeshiftInheritanceType FlagsInheritanceType{ 0 };
    [[bg3::legacy(Flags)]] AttributeFlags AttributeFlags{ 0 };
    ShapeshiftInheritanceType DefaultBoostsInheritanceType{ 0 };
    FixedString DefaultBoosts;
    ShapeshiftInheritanceType PersonalStatusImmunitiesInheritanceType{ 0 };
    FixedString PersonalStatusImmunities;
    ShapeshiftInheritanceType PassivesInheritanceType{ 0 };
    FixedString Passives;
    ShapeshiftInheritanceType ResistancesInheritanceType{ 0 };
    std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
    ShapeshiftInheritanceType DifficultyStatusesInheritanceType{ 0 };
    FixedString DifficultyStatuses;
    ShapeshiftInheritanceType ProficiencyGroupsInheritanceType{ 0 };
    ProficiencyGroupFlags ProficiencyGroup;
    ShapeshiftInheritanceType BloodSurfaceTypeInheritanceType{ 0 };
    ShapeshiftInheritanceType HitBloodTypeInheritanceType{ 0 };
    ShapeshiftInheritanceType DeathActionsInheritanceType{ 0 };
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
    HashMap<EntityHandle, EntityHandle> UpdateRequests;
    [[bg3::hidden]] void* ShapeshiftRulebook;
    [[bg3::hidden]] void* RPGStats;
};


END_NS()
