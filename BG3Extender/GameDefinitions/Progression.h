#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct LevelUpUpgrades
{
    struct Selector
    {
        [[bg3::legacy(field_0)]] uint8_t Type;
        Guid Class;
        Guid Subclass;
        [[bg3::legacy(field_28)]] int Level;
        [[bg3::legacy(field_2C)]] bool Multiclass;
        [[bg3::legacy(field_30)]] int DefinitionIndex;
    };

    struct FeatSelector : public Selector
    {
        Guid Feat;
        [[bg3::legacy(Array_b8)]] Array<AbilityId> AbilityImprovements;
        STDString FeatName;
        int field_80;
    };

    struct SkillSelector : public Selector
    {
        Guid Skill;
        [[bg3::legacy(Array_b8)]] Array<SkillId> Proficiencies;
        STDString field_60;
        int field_80;
    };
        
    struct SkillExpertiseSelector : public Selector
    {
        [[bg3::legacy(field_38)]] Guid Skill;
        uint8_t field_48;
        [[bg3::legacy(Array_b8)]] Array<SkillId> Expertise;
        STDString field_60;
        int field_80;
    };
        
    struct AbilityBonusSelector : public Selector
    {
        Guid AbilityBonus;
        [[bg3::legacy(Array_b8)]] Array<AbilityId> Bonuses;
        [[bg3::legacy(Array_i32)]] Array<uint32_t> BonusAmounts;
        [[bg3::legacy(field_60)]] STDString BonusType;
        int field_80;
    };
        
    struct SpellSelector : public Selector
    {
        struct StringPair
        {
            FixedString From;
            FixedString To;
        };

        Guid SpellList;
        Array<FixedString> Spells;
        [[bg3::legacy(Array_FS2)]] Array<StringPair> ReplaceSpells;
        [[bg3::legacy(field_78)]] STDString SelectorId;
    };
        
    struct PassiveSelector : public Selector
    {
        struct StringPair
        {
            FixedString From;
            FixedString To;
        };

        [[bg3::legacy(SpellList)]] Guid PassiveList;
        [[bg3::legacy(Spells)]] Array<FixedString> Passives;
        [[bg3::legacy(Array_FS2)]] Array<StringPair> ReplacePassives;
        [[bg3::legacy(field_78)]] STDString SelectorId;
        int field_80;
    };
        
    struct EquipmentSelector : public Selector
    {
        [[bg3::legacy(field_38)]] Guid EquipmentList;
        [[bg3::legacy(Array_FS)]] Array<FixedString> Equipment;
        [[bg3::legacy(field_60)]] STDString SelectorId;
        int field_80;
    };

    Array<FeatSelector> Feats;
    Array<AbilityBonusSelector> AbilityBonuses;
    Array<SkillSelector> Skills;
    [[bg3::legacy(Unknowns2), bg3::legacy(SkillBonuses)]] Array<SkillExpertiseSelector> SkillExpertise;
    Array<SpellSelector> Spells;
    [[bg3::legacy(Spells2)]] Array<PassiveSelector> Passives;
    [[bg3::legacy(Unknowns4)]] Array<EquipmentSelector> Equipment;
};

struct LevelUpData
{
    Guid Class;
    Guid SubClass;
    Guid Feat;
    Guid AccessorySet;
    std::array<int, 7> Abilities;
    LevelUpUpgrades Upgrades;
    [[bg3::legacy(field_B0)]] Array<SpellMetaId> Spells;
};

END_SE()
