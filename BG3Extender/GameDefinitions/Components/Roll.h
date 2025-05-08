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
    EntityHandle field_20;
    STDString StatsExpression;
    uint8_t AdvantageType;
    uint8_t AC;
    uint8_t NaturalRoll;
    uint8_t DiscardedDiceTotal;
    uint8_t DiceAdditionalValue;
    int AdditionalValue;
    uint8_t SpellCheck;
    bool Finished;
    uint8_t field_4E;
    uint8_t field_4F;
    float PassiveRollDelay;
    uint8_t RollContext;
    uint8_t field_55;
    int field_58;
    EntityHandle field_60;
    Array<ResolvedRollBonus> ResolvedRollBonuses;
    Array<FixedRollBonus> FixedRollBonuses;
    uint8_t field_88;
    uint8_t field_89;
    uint8_t field_8A;
    Array<Guid> field_90;
    AbilityId Ability;
    SkillId Skill;
    uint8_t field_A2;
    Roll Roll;
    StatsRollResult Result;
    StatsRollMetadata Metadata;
    STDString field_1B0;
    float PassiveRollDelay2;
    uint8_t field_1CC;
    Guid EntityUuid;
    Guid Entity2Uuid;
};

END_SE()

BEGIN_NS(active_roll)

DEFINE_TAG_COMPONENT(eoc::active_roll, InProgressComponent, RollInProgress)

struct DynamicModifierId
{
    Guid ModifierGuid;
    DynamicModifierType Type;
};


struct ModifierSourceInfo
{
    uint8_t SourceType;
    FixedString Source;
    FixedString StackId;
    int StackPriority;
    EntityHandle field_10;
    __int64 field_18;
    uint8_t field_20;
    EntityHandle field_28;
    EntityHandle field_30;
};


struct Modifier
{
    int AbilityModifier;
    int AbilityModifier2;
    uint8_t AdvantageType;
    uint8_t Advantage;
    uint8_t SuccessFlags;
    DiceSizeId DiceValue;
    uint8_t AmountOfDices;
    BoostType BoostType;
    uint8_t TargetType;
    uint8_t field_F;
    uint8_t field_28;
    TranslatedString field_2C;
    __int64 field_40;
    __int64 field_48;
    ModifierSourceInfo Source;
};

struct StaticModifier
{
    Guid ModifierGuid;
    uint8_t field_10;
    Modifier Modifier;
    uint32_t field_88;
    uint32_t field_8C;
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
    uint8_t TargetType;
    bool Concentration;
    Array<Modifier> Modifiers;
};

struct SpellModifier
{
    Guid ModifierGuid;
    SpellId Spell;
    EntityHandle Source;
    uint8_t TargetType;
    bool Concentration;
    Array<Modifier> Modifiers;
};

struct ToggledPassiveModifier
{
    Guid ModifierGuid;
    FixedString Passive;
    int field_14;
    EntityHandle Source;
    EntityHandle PassiveEntity;
    Array<Modifier> Modifiers;
};

struct ModifiersComponent : public BaseComponent
{
    DEFINE_COMPONENT(RollModifiers, "eoc::active_roll::ModifiersComponent")

    EntityHandle field_0;
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
    EntityHandle Source2;
    Guid RollUuid;
    EntityHandle field_20;
    uint8_t SpellCheck;
    uint8_t field_29;
    EntityHandle Source;
    uint8_t AC;
    uint8_t field_39;
    uint8_t field_3A;
    uint8_t field_3B;
    uint8_t AdvantageType1;
    uint8_t AdvantageType2;
    SkillId Skill;
    AbilityId Ability;
    uint8_t RollType;
    int DiceAdditionalValue;
    uint8_t field_48;
    int NaturalRoll;
    uint8_t RollComponentType0;
};


struct RollFinishedEventOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRollFinishedEvent, "esv::roll::RollFinishedEventOneFrameComponent")

    Array<FinishedEvent> Events;
};

END_NS()
