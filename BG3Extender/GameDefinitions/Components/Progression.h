#pragma once

BEGIN_NS(progression)

struct SpellDefinition
{
    FixedString SpellId;
    SpellPrepareType PrepareType{ SpellPrepareType::AlwaysPrepared };
    Guid SpellList;
    AbilityId CastingAbility{ AbilityId::None };
    SpellCooldownType CooldownType{ SpellCooldownType::Default };
    Guid Class;
};

struct AbilityImprovementsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionAbilityImprovements, "eoc::progression::AbilityImprovementsComponent")

    Array<AbilityId> Abilities;
    std::array<int32_t, 7> AbilityImprovements;
};

struct FeatComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionFeat, "eoc::progression::FeatComponent")

    Guid Feat;
    int field_10;
    std::optional<int> ConditionId;
    Array<SkillId> Proficiencies;
    Array<SkillId> Expertise;
    Array<SpellDefinition> AddSpells;
    Array<FixedString> AddPassives;
    std::array<int32_t, 7> AbilityImprovements;
};

struct SpellsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionSpells, "eoc::progression::SpellsComponent")

    Array<SpellDefinition> AddSpells;
    Array<SpellDefinition> RemoveSpells;
};

struct SkillsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionSkills, "eoc::progression::SkillsComponent")

    Array<SkillId> Proficiencies;
    Array<SkillId> Expertise;
};

struct PassivesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionPassives, "eoc::progression::PassivesComponent")

    Array<FixedString> AddPassives;
    Array<FixedString> RemovePassives;
};

struct ReplicatedFeatComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionReplicatedFeat, "eoc::progression::ReplicatedFeatComponent")

    Guid Feat;
    uint8_t field_10;
    int field_14;
};

struct LevelUpComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelUp, "eoc::progression::LevelUpComponent")

    [[bg3::legacy(field_18)]] Array<LevelUpData> LevelUps;
};

struct ContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionContainer, "eoc::ProgressionContainerComponent")

    Array<Array<EntityHandle>> Progressions;
};

struct MetaComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionMeta, "eoc::progression::MetaComponent")

    uint8_t field_18;
    Guid Progression;
    [[bg3::legacy(Race)]] Guid Source;
    [[bg3::legacy(field_40)]] int ClassLevel;
    [[bg3::legacy(field_44), bg3::legacy(ProgressionType)]] SpellSourceType SpellSourceType;
    [[bg3::legacy(field_48)]] int Level;
    EntityHandle Owner;
    [[bg3::legacy(field_50)]] HashMap<Guid, Guid> MulticlassSpellSlotOverride;
};

END_NS()

BEGIN_NS(esv::progression)

struct ChangedContainersComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionChangedContainers, "esv::progression::ChangedContainersComponent")

    HashMap<EntityHandle, HashMap<EntityHandle, Array<EntityHandle>>> Changes;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::progression, LevelUpChangedOneFrameComponent, ProgressionLevelUpChanged)

struct ManagementSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerProgression, "esv::progression::ManagementSystem")

    HashSet<EntityHandle> DestroyedProgressions;
    HashMap<EntityHandle, int> ProgressionUpdates;
    [[bg3::hidden]] void* ImmutableDataHeadmaster;
    [[bg3::hidden]] void* RPGStats;
    [[bg3::hidden]] CriticalSection CS_;
};

END_NS()
