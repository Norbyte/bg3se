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

struct AttackSpellOverrideComponent : public BaseComponent
{
    DEFINE_COMPONENT(AttackSpellOverride, "eoc::spell::AttackSpellOverrideComponent")

    HashMap<uint8_t, FixedString> AttackSpells;
};

struct ScriptedExplosionComponent : public BaseComponent
{
    DEFINE_COMPONENT(ScriptedExplosion, "eoc::spell::ScriptedExplosionComponent")

    FixedString Projectile;
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

