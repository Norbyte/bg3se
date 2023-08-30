function TestStatAttributes()
    local character = Ext.Stats.Get("MindFlayer")

    -- ConstantInt test
    AssertEquals(character.Vitality, 71)
    character.Vitality = 55
    AssertEquals(character.Vitality, 55)

    -- AttributeFlags test
    AssertEquals(character.Flags, {"FloatingWhileMoving"})
    character.Flags = {"Floating"}
    AssertEquals(character.Flags, {"Floating"})
    character.Flags = {"Floating", "FloatingWhileMoving"}
    AssertEquals(character.Flags, {"Floating", "FloatingWhileMoving"})
    character.Flags = {}
    AssertEquals(character.Flags, {})

    -- StatusIDs test
    AssertEquals(character.PersonalStatusImmunities, "PRONE_FALLEN;CRIPPLED")
    character.PersonalStatusImmunities = "TEST"
    AssertEquals(character.PersonalStatusImmunities, "TEST")
    character.PersonalStatusImmunities = ""
    AssertEquals(character.PersonalStatusImmunities, "")

    -- Enumeration test (ArmorType)
    AssertEquals(character.ArmorType, "Cloth")
    character.ArmorType = "StuddedLeather"
    AssertEquals(character.ArmorType, "StuddedLeather")
    character.ArmorType = "None"
    AssertEquals(character.ArmorType, "None")

    -- Flags test (ProficiencyGroupFlags)
    --AssertEquals(character['Proficiency Group'], {"LightArmor","MediumArmor","HeavyArmor"})
    --character['Proficiency Group'] = {"LightArmor","HeavyArmor"}
    --AssertEquals(character['Proficiency Group'], {"LightArmor","HeavyArmor"})
    --character['Proficiency Group'] = {}
    --AssertEquals(character['Proficiency Group'], {})

    local weapon = Ext.Stats.Get("WPN_Sling")

    -- ConstantFloat test
    AssertEquals(weapon.ValueScale, 0.5)
    weapon.ValueScale = 2.5
    AssertEquals(weapon.ValueScale, 2.5)

    -- GUID test
    AssertEquals(weapon.Projectile, "b64b516c-1afd-4f8d-b624-4c9caf06f1c2")
    weapon.Projectile = "a086b825-8f17-4f0e-855b-178f8cdc515a"
    AssertEquals(weapon.Projectile, "a086b825-8f17-4f0e-855b-178f8cdc515a")

    -- FixedString test
    AssertEquals(weapon.BoostsOnEquipMainHand, "UnlockSpell(Projectile_Headcrack)")
    weapon.BoostsOnEquipMainHand = "UnlockSpell(TEST)"
    AssertEquals(weapon.BoostsOnEquipMainHand, "UnlockSpell(TEST)")
    weapon.BoostsOnEquipMainHand = ""
    AssertEquals(weapon.BoostsOnEquipMainHand, "")

    -- RollConditions test
    local spell1 = Ext.Stats.Get("Target_Claws_IntellectDevourer")
    AssertEquals(spell1.SpellRoll, "Attack(AttackType.MeleeUnarmedAttack)")
    spell1.SpellRoll = "Attack(TEST)"
    AssertEquals(spell1.SpellRoll, "Attack(TEST)")
    spell1.SpellRoll = ""
    -- FIXME - AssertEquals(spell1.SpellRoll, "")

    local spell2 = Ext.Stats.Get("Target_FindFamiliar_Cat_Ritual")
    local requirements2 = {
        {
            Not = true,
            Param = -1,
            Requirement = "Combat"
        }
    }
    AssertEquals(spell2.Requirements, requirements2)

    local requirements3 = {
        { -- Level 10
            Not = false,
            Param = 10,
            Requirement = "Level"
        }
    }
    spell2.Requirements = requirements3
    AssertEquals(spell2.Requirements, requirements3)

    spell2.Requirements = {}
    AssertEquals(spell2.Requirements, {})

    local spell4 = Ext.Stats.Get("Target_TripAttack")
    AssertEquals(spell4.TargetConditions, "Character() and not Self()")
    spell4.TargetConditions = "TEST"
    AssertEquals(spell4.TargetConditions, "TEST")
    spell4.TargetConditions = ""
    AssertEquals(spell4.TargetConditions, "")

    -- StatsFunctors test
    local spell5 = Ext.Stats.Get("Target_Curse_Gnoll")
    local functors1 = { -- ApplyStatus(GNOLL_CURSE,100,2)
        {
            Duration = 12.0,
            IsSelf = false,
            Param1 = -1,
            Param2 = -1,
            PropertyContext = { "AOE", "Target" },
            StatsConditions = "",
            StatusId = "GNOLL_CURSE",
            StringParam = "",
            Type = "ApplyStatus"
        }
    }
    AssertEquals(spell5.SpellSuccess, functors1)
    local functors2 = { -- ApplyStatus(TEST,100,1)
        {
            Condition = "wtf()",
            Duration = 6.0,
            IsSelf = false,
            Param1 = -1,
            Param2 = -1,
            PropertyContext = { "Target" },
            StatsConditions = "WTF",
            StatusId = "TEST",
            StringParam = "",
            Type = "ApplyStatus"
        }
    }
    spell5.SpellSuccess = functors2
    AssertEquals(spell5.SpellSuccess, functors2)
    spell5.SpellSuccess = {}
    AssertEquals(spell5.SpellSuccess, {})

    -- FIXME - test for adding StatFunctors and RollConditions where it was empty previously
    -- FIXME - test for clearing StatFunctors and RollConditions
end

-- Tests if re-assigning the current value to an attribute results in the same output
-- (i.e. if we support reading/writing all currently used stats attribute values in game)
function TestStatAttributeReassignment()
    local modifierLists = {
        "Character", "Armor", "Object", "Weapon", "SpellData", "StatusData", "PassiveData"
    }

    for i,modifierList in ipairs(modifierLists) do
        local stats = Ext.Stats.GetStats(modifierList)
        local attributes = Ext.Stats.GetModifierAttributes(modifierList)

        -- MemorizationRequirements is a buggy/nonexistent property in Modifiers.txt
        attributes.MemorizationRequirements = nil

        for j,statName in ipairs(stats) do
            local stat = Ext.Stats.Get(statName)
            for attribute,type in pairs(attributes) do
                local value = stat[attribute]
                stat[attribute] = value
                local value2 = stat[attribute]
                AssertEquals(value, value2)
            end
        end
    end
end

RegisterTests("Stats", {
    "TestStatAttributes",
    "TestStatAttributeReassignment"
})
