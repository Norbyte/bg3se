local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestComponentGeneric(componentName, expect)
    local ch = Ext.GetCharacter(GUID_LAEZEL)
    local component = ch[componentName]

    -- FIXME - add ==, ~= operator for entities, component handles
    -- expect.Entity = ch
    AssertEqualsProperties(expect, component)
end

function TestCharacterComponentDump()
    local c = Ext.GetCharacter(GUID_LAEZEL)

    Ext.Json.Stringify(c:GetAllComponents(), true, true)
end


function TestAnubisComponent()
    local expect = {
        field_18 = "ORI_Laezel"
    }
    
    TestComponentGeneric("Anubis", expect)
end


function TestAddedSpellsComponent()
    -- TODO - test with a real spell list
    local expect = {
        Spells = {}
    }
    
    TestComponentGeneric("AddedSpells", expect)
end


function TestAvailableLevelComponent()
    local expect = {
        Level = 1
    }
    
    TestComponentGeneric("AvailableLevel", expect)
end


function TestBaseHpComponent()
    local expect = {
        Vitality = 1,
        VitalityBoost = 0
    }
    
    TestComponentGeneric("BaseHp", expect)
end


function TestCanBeLootedComponent()
    local expect = {
        Flags = 0
    }
    
    TestComponentGeneric("CanBeLooted", expect)
end


function TestCanDeflectProjectilesComponent()
    local expect = {
        Flags = 0
    }
    
    TestComponentGeneric("CanDeflectProjectiles", expect)
end


function TestCanDoActionsComponent()
    local expect = {
        Flags = 0xffff
    }
    
    TestComponentGeneric("CanDoActions", expect)
end


function TestCanInteractComponent()
    local expect = {
        Flags = 0xffff
    }
    
    TestComponentGeneric("CanInteract", expect)
end


function TestCanModifyHealthComponent()
    local expect = {
        Flags = 0xffff
    }
    
    TestComponentGeneric("CanModifyHealth", expect)
end


function TestCanMoveComponent()
    local expect = {
        Flags = 0xffff
    }
    
    TestComponentGeneric("CanMove", expect)
end


function TestCanSenseComponent()
    local expect = {
        Flags = 65527
    }
    
    TestComponentGeneric("CanSense", expect)
end


function TestCanSpeakComponent()
    local expect = {
        Flags = 0xffff
    }
    
    TestComponentGeneric("CanSpeak", expect)
end


function TestCanTravelComponent()
    local expect = {
        field_18 = 0,
        field_1A = 1,
        field_1C = 224
    }
    
    TestComponentGeneric("CanTravel", expect)
end


function TestCombatParticipantComponent()
    -- FIXME - also test when in combat!
    local expect = {
        AiHint = "00000000-0000-0000-0000-000000000000",
        Archetype = "melee",
        CombatGroupChangedFlag_M = false,
        CombatGroupId = "",
        CombatHandle = null,
        Flags = 3
    }
    
    TestComponentGeneric("CombatParticipant", expect)
end


function TestConcentrationComponent()
    -- FIXME - also test when concentrating
    local expect = {
        SpellId = {
            ProgressionSource = "00000000-0000-0000-0000-000000000000",
            OriginatorPrototype = "",
            Prototype = "",
            SourceType = "Sentinel"
        }
    }
    
    TestComponentGeneric("Concentration", expect)
end


function TestDarknessComponent()
    local expect = {
        field_18 = 0,
        field_19 = 1,
        field_1A = 0,
        field_1B = 0,
        field_1C = 0,
        -- Always changing depending on lighting?
        -- field_1D = 97,
        field_20 = 0
    }
    
    TestComponentGeneric("Darkness", expect)
end


function TestDataComponent()
    local expect = {
        Flags = 0,
        StatsId = "POC_Player_Fighter",
        StepsType = 0,
        Weight_M = 45.0
    }
    
    TestComponentGeneric("Data", expect)
end


function TestDisplayNameComponent()
    local expect = {
        Name = "",
        NameKey = "h4a8991d6g1618g41ecga5b8g9f59b82c2237",
        UnknownKey = "ls::TranslatedStringRepository::s_HandleUnknown"
    }
    
    TestComponentGeneric("DisplayName", expect)
end


function TestDualWieldingComponent()
    -- FIXME - check when dual wielding?
    local expect = {
        field_18 = 0,
        field_1A = 0,
        field_1B = 0,
        field_1D = 0,
        field_1E = 1
    }
    
    TestComponentGeneric("DualWielding", expect)
end


function TestFleeCapabilityComponent()
    local expect = {
        field_18 = 0,
        field_1C = 0.0,
        field_20 = 0.0
    }
    
    TestComponentGeneric("FleeCapability", expect)
end


function TestGameObjectVisualComponent()
    local expect = {
        Icon = "1088f4ee-579b-b49e-72cc-7ecc5fc5be79-EQ_Laezel_(Icon_Githyanki_Female)",
        RootTemplateId = "58a69333-40bf-8358-1d17-fff240d7fb12",
        RootTemplateType = 1,
        field_24 = 1.0,
        field_28 = 0
    }
    
    TestComponentGeneric("GameObjectVisual", expect)
end


function TestGenderComponent()
    local expect = {
        Gender1 = "Female",
        Gender2 = "Female"
    }
    
    TestComponentGeneric("Gender", expect)
end


function TestHealthComponent()
    -- TODO: PerDamageTypeHealthThresholds, PerDamageTypeModifiers
    local expect = {
        CannotDamage_M  = false,
        CurrentHealth = 12,
        CurrentHealth_M = -1,
        MaxHealth = 12,
        MaxHealth_M = 0
    }
    
    TestComponentGeneric("Health", expect)
end


function TestHotbarContainerComponent()
    -- Only test one slot
    local expect = {
        Item = null,
        Passive = "",
        SpellId =
        {
            OriginatorPrototype = "Target_Dip",
            ProgressionSource=  "00000000-0000-0000-0000-000000000000",
            Prototype = "Target_Dip",
            SourceType = "SpellSet"
        }
    }
    
    local component = Ext.GetCharacter(GUID_LAEZEL).HotbarContainer
    local hotbarElement = component.Containers.DefaultBarContainer[1].Elements[2]

    AssertEqualsProperties(expect, hotbarElement)
end


function TestIconComponent()
    local expect = {
        Icon = "1088f4ee-579b-b49e-72cc-7ecc5fc5be79-EQ_Laezel_(Icon_Githyanki_Female)"
    }
    
    TestComponentGeneric("Icon", expect)
end


function TestLevelComponent()
    local expect = {
        LevelName = "TUT_Avernus_C"
    }
    
    TestComponentGeneric("Level", expect)
end


function TestLootComponent()
    local expect = {
        Flags = 1,
        InventoryType = 11
    }
    
    TestComponentGeneric("Loot", expect)
end


function TestMovementComponent()
    local expect = {
        field_18 = -0.0,
        field_1C = -0.0,
        field_20 = -1.0,
        field_24 = 12.0,
        field_28 = 0,
        field_2C = 0,
    }
    
    TestComponentGeneric("Movement", expect)
end


function TestNetComponent()
    local expect = {
        field_18 = ""
    }
    
    TestComponentGeneric("Net", expect)
end


function TestPathingComponent()
    -- FIXME - map more fields
    local expect = {
        CurveUUID = "1df9c9ee-8dd8-ebeb-3b1a-83d31f141f0a"
    }
    
    TestComponentGeneric("Pathing", expect)
end


function TestRaceComponent()
    local expect = {
        Race = "bdf9b779-002c-4077-b377-8ea7c1faa795"
    }
    
    TestComponentGeneric("Race", expect)
end


function TestRelationComponent()
    local expect = {
        field_18 = "bee8449b-d682-93c8-e7da-9e4bad369476",
        field_28 = "bee8449b-d682-93c8-e7da-9e4bad369476",
        field_38 = null
    }
    
    TestComponentGeneric("Relation", expect)
end


function TestSenseComponent()
    local expect = {
        FOV = 90.0,
        Hearing = 18.0,
        Sight = 18.5
    }
    
    TestComponentGeneric("Sense", expect)
end


function TestSenseComponent()
    local expect = {
        FOV = 90.0,
        Hearing = 18.0,
        Sight = 18.5
    }
    
    TestComponentGeneric("Sense", expect)
end


function TestServerDisplayNameListComponent()
    local expect = {
        Names = {
            {
                Name = "",
                NameKey = "h4a8991d6g1618g41ecga5b8g9f59b82c2237"
                -- TODO - 2 Unknowns
            }
        }
    }
    
    TestComponentGeneric("ServerDisplayNameList", expect)
end


function TestServerExperienceGaveOutComponent()
    local expect = {
        Experience = 0
    }
    
    TestComponentGeneric("ServerExperienceGaveOut", expect)
end


function TestServerIconListComponent()
    local expect = {
        Icons = {
            {
                Icon = "1088f4ee-579b-b49e-72cc-7ecc5fc5be79-EQ_Laezel_(Icon_Githyanki_Female)",
                field_4  = 2185232388
            }
        },
        field_30 = 1
    }
    
    TestComponentGeneric("ServerIconList", expect)
end


function TestServerOsirisTagComponent()
    local expect = {
        Tags = {
            "b5682d1d-c395-489c-9675-1f9b0c328ea5",
            "bad00ba2-8a49-450c-8387-af47681717f1",
            "d2f86ec3-c41f-47e1-8acd-984872a4d7d5"
        }
    }
    
    TestComponentGeneric("ServerOsirisTag", expect)
end


function TestServerRaceTagComponent()
    local expect = {
        Tags = {
            "677ffa76-2562-4217-873e-2253d4720ba4",
            "7fbed0d4-cabc-4a9d-804e-12ca6088a0a8"
        }
    }
    
    TestComponentGeneric("ServerRaceTag", expect)
end


function TestServerSafePositionComponent()
    local expect = {
        Position = { 0.0, 0.0, 0.0 },
        field_24 = false
    }
    
    TestComponentGeneric("ServerSafePosition", expect)
end


function TestServerToggledPassivesComponent()
    local expect = {
        Passives = {
            AttackOfOpportunity = true
        }
    }
    
    TestComponentGeneric("ServerToggledPassives", expect)
end


function TestServerTriggerStateComponent()
    local expect = {
        Triggers = {
            "d6136d6a-38b4-43f0-ac12-7b24b4599f0f",
            "7e474b28-8ff0-4c7b-800d-e43eee0e7e2e"
        }
    }
    
    TestComponentGeneric("ServerTriggerState", expect)
end


function TestSightComponent()
    local expect = {
        Sight = 2.0
    }
    
    TestComponentGeneric("Sight", expect)
end


function TestSpellBookComponent()
    -- Only test one slot
    local expect = {
        CooldownType = "OncePerTurnNoRealtime",
        Id = {
            OriginatorPrototype = "Projectile_Jump",
            ProgressionSource = "00000000-0000-0000-0000-000000000000",
            Prototype = "Projectile_Jump",
            SourceType = "SpellSet"
        },
        SpellCastingAbility = "Intelligence",
        SpellUUID = "d136c5d9-0ff0-43da-acce-a74a07f8d6bf",
        field_38 = -1,
        field_41 = 1,
        field_42 = 0
    }
    
    local component = Ext.GetCharacter(GUID_LAEZEL).SpellBook
    local spell = component.Spells[1]

    AssertEqualsProperties(expect, spell)
end


function TestSpellBookPreparesComponent()
    -- Only test one slot
    local expect = {
        OriginatorPrototype = "Projectile_Jump",
        ProgressionSource = "00000000-0000-0000-0000-000000000000",
        SourceType = "SpellSet"
    }
    
    local component = Ext.GetCharacter(GUID_LAEZEL).SpellBookPrepares
    local spell = component.PreparedSpells[1]

    AssertEqualsProperties(expect, spell)
end


function TestSpellContainerComponent()
    -- Only test one slot
    local expect = {
        CooldownType = "Default",
        ItemHandle = null,
        SelectionType = "Singular",
        SpellCastingAbility = "Intelligence",
        SpellId = {
            OriginatorPrototype = "Target_Dip",
            ProgressionSource = "00000000-0000-0000-0000-000000000000",
            SourceType = "SpellSet"
        },
        SpellUUID = "d136c5d9-0ff0-43da-acce-a74a07f8d6bf",
        field_29 = 0,
        field_44 = "",
        field_48 = 0
    }
    
    local component = Ext.GetCharacter(GUID_LAEZEL).SpellContainer
    local spell = component.Spells[2]

    AssertEqualsProperties(expect, spell)
end


function TestStatsComponent()
    local expect = {
        Abilities = {
            1, 0, 17, 13, 14, 11, 12, 8
        },
        ArmorType = 0,
        ArmorType_Breast = 9,
        Classes = {
            {
                Class = "721dfac3-92d4-41f5-b773-b7072a86232f",
                SubClass = "00000000-0000-0000-0000-000000000000",
                Priority = 1,
            }
        },
        Equipment = {
            Boots = {
                -- ItemHandle = "Entity (03000001000000af)",
                Slot = "Boots",
                field_10 = 1,
                -- field_11 = 230
            },
            Breast = {
                -- ItemHandle = "Entity (03000001000000ac)",
                Slot = "Breast",
                field_10 = 1,
                -- field_11 = 230
            },
            MeleeMainWeapon = {
                -- ItemHandle = "Entity (03000001000000b1)",
                Slot = "MeleeMainWeapon",
                field_10 = 1,
                -- field_11 = 230
            },
            RangedMainWeapon = {
                -- ItemHandle = "Entity (03000001000000b0)",
                Slot = "RangedMainWeapon",
                field_10 = 1,
                -- field_11 = 230
            }
        },
        Flanked = false,
        IsRanged = 1,
        Level = 1,
        MaxCarryWeight = 170000,
        ProficiencyBonus = 2,
        RangedAttackAbility = "Dexterity",
        Size = 1,
        Skills = {
            -1, -1, -1, -1, -1, 1, 1, 1, 0, 0,
            0, 0, 0, 3, 1, 1, 1, 1
        },
        SpellCastingAbility = "Intelligence",
        SpellDC = 10,
        UnarmedAttackAbility = "Strength",
        WeaponActionDC = 4,
        field_38 = 0,
        field_3C = 5,
        field_40 = 1,
        field_44 = 4,
        field_48 = 0,
        field_4C = 1,
        field_AC = 0
    }
    
    TestComponentGeneric("Stats", expect)

    -- Check item handle validity
    local stats = Ext.GetCharacter(GUID_LAEZEL).Stats
    local wpn = stats.Equipment.MeleeMainWeapon.ItemHandle.Data
    AssertEquals(wpn.StatsId, "WPN_Longsword")
end


function TestStatusImmunitiesComponent()
    -- FIXME - test with non-empty list
    local expect = {
        PersonalStatusImmunities = {}
    }
    
    TestComponentGeneric("StatusImmunities", expect)
end


function TestSurfacePathInfluencesComponent()
    -- Only test one slot
    local expect = {
        Influence = 100,
        IsCloud = false,
        SurfaceType = "BloodElectrified"
    }
    
    local component = Ext.GetCharacter(GUID_LAEZEL).SurfacePathInfluences
    local influence = component.PathInfluences[1]

    AssertEqualsProperties(expect, influence)
end


function TestTransformComponent()
    local expect = {
        Transform  = {
                Scale = { 1.0, 1.0, 1.0 },
                Translate = {
                    -82.25,
                    8.9506282806396484,
                    -408.75
                }
        }
    }
    
    TestComponentGeneric("Transform", expect)
end



-- FIXME - ActionResources
-- BoostsContainer
-- LearnedSpells
-- ObjectInteraction
-- Physics
-- ProgressionContainer
-- ServerPickpocket
-- ServerReplicationDependencyOwner
-- ServerTemplateTag
-- SpellAiConditions
-- SpellBookCooldowns
-- Tag
-- TurnBased
-- UnsheathInfo
