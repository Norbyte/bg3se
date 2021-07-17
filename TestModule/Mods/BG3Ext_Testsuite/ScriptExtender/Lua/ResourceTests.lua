function TestActionResources()
    local res = Ext.StaticData.GetGuidResource("45ff0f48-b210-4024-972f-b64a44dc6985", "ActionResource")
    local expect = {
        ResourceUUID = "45ff0f48-b210-4024-972f-b64a44dc6985",
        Description = "ActionResourceDefinitions_New_Stat_2_Description",
        DiceType = 8,
        DisplayName = "ActionResourceDefinitions_New_Stat_2_DisplayName",
        Error = "ActionResourceDefinitions_New_Stat_2_Error",
        IsSpellResource = false,
        MaxLevel = 0,
        MaxValue = 0.0,
        Name = "ReactionActionPoint",
        PartyActionResource = false,
        ReplenishType = 2,
        ShowOnActionResourcePanel = false,
        UpdatesSpellPowerLevel = false,
    }

    AssertEqualsProperties(expect, res)
end

function TestActionResourceGroupDefinitions()
    local res = Ext.StaticData.GetGuidResource("03b17647-161a-42e1-9660-5ba517e80ad2", "ActionResourceGroup")
    local expect = {
        ResourceUUID = "03b17647-161a-42e1-9660-5ba517e80ad2",
        Name = "SpellSlotsGroup",
        ActionResourceDefinitions = {"d136c5d9-0ff0-43da-acce-a74a07f8d6bf","e9127b70-22b7-42a1-b172-d02f828f260a"}
    }

    AssertEqualsProperties(expect, res)
end

function TestClassDescriptionResources()
    local res = Ext.StaticData.GetGuidResource("c54d7591-b305-4f22-b2a7-1bf5c4a3470a", "ClassDescription")
    local expect = {
        ResourceUUID = "c54d7591-b305-4f22-b2a7-1bf5c4a3470a",
        BaseHp = 0,
        CanLearnSpells = false,
        CharacterCreationPose = "07e81afc-95c4-452b-ac68-5cbb74710cca",
        ClassEquipment = "",
        Description = "ClassDescriptions_New_Stat_11_Description",
        DisplayName = "ClassDescriptions_New_Stat_11_DisplayName",
        HasGod = false,
        HpPerLevel = 0,
        LearningStrategy = 1,
        MagicType = 2,
        MustPrepareSpells = true,
        Name = "LightDomain",
        ParentGuid = "114e7aee-d1d4-4371-8d90-8a2080592faf",
        PrimaryAbility = 5,
        ProgressionTableUUID = "6ec6caad-2dfa-4661-8edd-5c4eca0e044c",
        SoundMagicType = 1,
        SpellCastingAbility = 5
    }

    AssertEqualsProperties(expect, res)
end

function TestTagResources()
    local res = Ext.StaticData.GetGuidResource("2fa3f042-2b50-41b2-8045-39ddd4421ca9", "Tag")
    local expect = {
        ResourceUUID = "2fa3f042-2b50-41b2-8045-39ddd4421ca9",
        -- Redacted since EA4
        -- Description = "|You lived in seclusion -either in a sheltered community such as a monastery, or entirely alone- for a formative part of your life.|",
        Description = "",
        DisplayDescription = "hd8084ad7gda28g4aa6gb724gf0b067863713",
        DisplayName = "h49a1cf3bg32eeg40d6g8d95g82dfb950ea5b",
        Icon = "",
        Name = "HERMIT",
        Categories = 515
    }

    AssertEqualsProperties(expect, res)
end

function TestFactionResources()
    local res = Ext.StaticData.GetGuidResource("7c6404dc-5241-a019-f48f-d92c9da80494", "Faction")
    local expect = {
        ResourceUUID = "7c6404dc-5241-a019-f48f-d92c9da80494",
        ParentGuid = "a863d0cc-60b9-b450-f90d-8a4dc192ecb9",
        -- Redacted since EA4
        -- Faction = "Animal_Cat",
        Faction = ""
    }

    AssertEqualsProperties(expect, res)
end

function TestRaceResources()
    local res = Ext.StaticData.GetGuidResource("6c038dcb-7eb5-431d-84f8-cecfaf1c0c5a", "Race")
    local expect = {
        ResourceUUID = "6c038dcb-7eb5-431d-84f8-cecfaf1c0c5a",
        Description = "Races_New_Stat_1_Description",
        DisplayName = "Races_New_Stat_1_DisplayName",
        Name = "Elf",
        ParentGuid = "899d275e-9893-490a-9cd5-be856794929f",
        ProgressionTableUUID = "eb9db11d-1091-49fa-9006-2f52bbb76f6a",
        RaceSoundSwitch = "Elf",
        -- FIXME - EyeColors, etc. colors
    }

    AssertEqualsProperties(expect, res)
end

function TestOriginResources()
    local res = Ext.StaticData.GetGuidResource("fb3bc4c3-49eb-4944-b714-d0cb357bb635", "Origin")
    local expect = {
        ResourceUUID = "fb3bc4c3-49eb-4944-b714-d0cb357bb635",
        -- FIXME - Field not mapped? - AppearanceLocked = true,
        AvailableInCharacterCreation = false,
        BackgroundUUID = "1252a86e-9baa-4ccb-b8ce-2378ae006f0b",
        ClassEquipmentOverride = "EQ_Laezel",
        ClassUUID = "721dfac3-92d4-41f5-b773-b7072a86232f",
        CloseUpA = "",
        CloseUpB = "",
        Description = "Origins_New_Stat_2_Description",
        DisplayName = "Origins_New_Stat_2_DisplayName",
        Gender = 1,
        GlobalTemplate = "58a69333-40bf-8358-1d17-fff240d7fb12",
        GodUUID = "00000000-0000-0000-0000-000000000000",
        LockClass = false,
        Name = "Laezel",
        Overview = "",
        Passives = "DeathSavingThrows",
        RaceUUID = "bdf9b779-002c-4077-b377-8ea7c1faa795",
        SubClassUUID = "e668c6f1-5149-4b10-ab7e-3637ed444066",
        SubRaceUUID = "00000000-0000-0000-0000-000000000000",
        -- FIXME - Field not mapped? - Unique = true,
        VoiceTableUUID = "46069209-0879-403f-85c7-fd23aab0a56f",
    }

    AssertEqualsProperties(expect, res)
end

function TestBackgroundResources()
    local res = Ext.StaticData.GetGuidResource("229775c9-3044-4779-a3bb-532c39238e03", "Background")
    local expect = {
        ResourceUUID = "229775c9-3044-4779-a3bb-532c39238e03",
        Description = "Backgrounds_New_Stat_2_Description",
        DisplayName = "Backgrounds_New_Stat_2_DisplayName",
        Passives = "Background_Entertainer",
        Tags = {"bd3cfefd-5fe2-47ea-bd3c-e99eaad9b714"},
    }

    AssertEqualsProperties(expect, res)
end

function TestGodResources()
    local res = Ext.StaticData.GetGuidResource("614180f6-1887-4a54-920b-b0428ebca99e", "God")
    local expect = {
        ResourceUUID = "614180f6-1887-4a54-920b-b0428ebca99e",
        Description = "Gods_New_Stat_2_Description",
        DisplayName = "Gods_New_Stat_2_DisplayName",
        Name = "Tempus",
        Tags = {"f6b88f18-328f-41c3-a579-e8a9b99c410b", "c1ff8a7f-c430-4671-bc4e-098f874ae302"},
    }

    AssertEqualsProperties(expect, res)
end

function TestProgressionResources()
    local res = Ext.StaticData.GetGuidResource("051cd419-b843-4996-807e-d0ac78084ef0", "Progression")
    local expect = {
        ResourceUUID = "051cd419-b843-4996-807e-d0ac78084ef0",
        AllowImprovement = true,
        Boosts = "ActionResource(SpellSlot,1,2)",
        Level = 4,
        Name = "Cleric",
        PassivesAdded = "",
        PassivesRemoved = "",
        ProgressionType = 0,
        -- Selectors = "SelectSpells(2f43a103-5bf1-4534-b14f-663decc0c525,1,0,,,,AlwaysPrepared)",
        SelectSpells = {"2f43a103-5bf1-4534-b14f-663decc0c525"},
        TableUUID = "64474b62-b4f5-46b3-b94a-c676c6da3116",
    }

    AssertEqualsProperties(expect, res)
end

function TestProgressionDescriptionResources()
    local res = Ext.StaticData.GetGuidResource("6f0ab4cc-d7d7-4ace-81b8-3d28c43354bf", "ProgressionDescription")
    local expect = {
        ResourceUUID = "6f0ab4cc-d7d7-4ace-81b8-3d28c43354bf",
        Description = "ProgressionDescriptions_New_Stat_113_Description",
        DisplayName = "ProgressionDescriptions_New_Stat_113_DisplayName",
        ExactMatch = "",
        Hidden = true,
        ParamMatch = "",
        PassivePrototype = "Backstab",
        ProgressionId = "00000000-0000-0000-0000-000000000000",
        ProgressionTableId = "00000000-0000-0000-0000-000000000000",
        SelectorId = "",
        Type = "Attribute",
    }

    AssertEqualsProperties(expect, res)
end

function TestGossipDefinitions()
    local res = Ext.StaticData.GetGuidResource("1abea2b5-4402-4d8e-b4e8-0052e4a9837c", "Gossip")
    local expect = {
        ResourceUUID = "1abea2b5-4402-4d8e-b4e8-0052e4a9837c",
        DialogUUID = "83cde7db-c4bf-28d4-c44b-74431b418092",
        Name = "PB_Shadowheart_Laezel_Gith3",
        Priority = 80,
        Type = "PartyBanter",
        ConditionFlags = {"83208bb5-4eb9-4394-845d-954d3ec3febb"},
    }

    AssertEqualsProperties(expect, res)
end

function TestEquipmentTypeResources()
    local res = Ext.StaticData.GetGuidResource("896f8a20-7dda-4bff-b726-08c3dacccc7b", "EquipmentType")
    local expect = {
        ResourceUUID = "896f8a20-7dda-4bff-b726-08c3dacccc7b",
        BoneMainSheathed = "Dummy_Sheath_Upper_L",
        BoneMainUnsheathed = "Dummy_L_Hand",
        BoneOffHandSheathed = "Dummy_Sheath_Upper_L",
        BoneOffHandUnsheathed = "Dummy_L_Hand",
        BoneVersatileSheathed = "Dummy_Sheath_Upper_R",
        BoneVersatileUnsheathed = "Dummy_R_Hand",
        Name = "Battleaxe",
        SoundAttackType = "Slashing",
        SoundEquipmentType = "Axe",
        SourceBoneSheathed = "Dummy_Sheath",
        SourceBoneVersatileSheathed = "Dummy_Sheath_Versatile",
        SourceBoneVersatileUnsheathed = "Dummy_Attachment_Versatile",
        WeaponType_OneHanded = "Slashing1H",
        WeaponType_TwoHanded = "Generic2H",
    }

    AssertEqualsProperties(expect, res)
end

function TestFlagResources()
    local res = Ext.StaticData.GetGuidResource("00bc7d69-5e0f-1054-db30-b429f31514f7", "Flag")
    local expect = {
        ResourceUUID = "00bc7d69-5e0f-1054-db30-b429f31514f7",
        -- Redacted since EA4
        -- Description = "Player is aware the Duke was in the tavern",
        Description = "",
        Name = "PLA_Tavern_Knows_DukeInTavern",
        Usage = 5,
    }

    AssertEqualsProperties(expect, res)
end

function TestFeatResources()
    local res = Ext.StaticData.GetGuidResource("661eee63-ff91-4f29-9f21-3a974c9d6fe5", "Feat")
    local expect = {
        ResourceUUID = "661eee63-ff91-4f29-9f21-3a974c9d6fe5",
        CanBeTakenMultipleTimes = false,
        Name = "DefensiveDuelist",
        PassivesAdded = "DefensiveDuelist",
        PassivesRemoved = "",
        Requirements = "",
        SelectAbilities = {},
        SelectSkills = {},
        SelectSpells = {},
        SelectPassives = {},
        AddSpells = {},
    }

    AssertEqualsProperties(expect, res)
end

function TestFeatDescriptionResources()
    local res = Ext.StaticData.GetGuidResource("95777965-bb2b-4ad5-8cc8-3c0c407c4bc4", "FeatDescription")
    local expect = {
        ResourceUUID = "95777965-bb2b-4ad5-8cc8-3c0c407c4bc4",
        Description = "FeatDescriptions_New_Stat_3_Description",
        DisplayName = "FeatDescriptions_New_Stat_3_DisplayName",
        ExactMatch = "DualWielder",
        FeatId = "f692f7b5-ffd5-4942-91a1-a71ebb2f5e7c",
        Hidden = false,
        ParamMatch = "",
        PassivePrototype = "",
        SelectorId = "",
        Type = "",
    }

    AssertEqualsProperties(expect, res)
end

function TestGuidResourceLayout()
    local types = {
        "ActionResource", "ActionResourceGroup", "ClassDescription", "Tag", "Faction", "Race", "Origin", 
        "Background", "God", "Progression", "ProgressionDescription", "Gossip", "EquipmentType",
        "Flag", "Feat", "FeatDescription"
    }

    for i,type in ipairs(types) do
        local resources = Ext.StaticData.GetAllGuidResources(type)
        for j,uuid in ipairs(resources) do
            local res = Ext.StaticData.GetGuidResource(uuid, type)
            AssertEquals(res.ResourceUUID, uuid)
        end
    end
end

function TestResourceEnumeration()
    local types = {
        "Visual", "VisualSet", "Animation", "AnimationSet", "Texture", "Material", "Physics", "Effect", 
        "Script", "Sound", "Lighting", "Atmosphere", "AnimationBlueprint", "MeshProxy", "MaterialSet", 
        "BlendSpace", "FCurve", "Timeline", "Dialog", "VoiceBark", "TileSet", "IKRig", "Skeleton", 
        "VirtualTexture", "TerrainBrush", "ColorList", "ColorPreset", "CharacterVisual", "MaterialPreset", 
        "SkinPreset", "ClothCollider"
    }

    for i,type in ipairs(types) do
        local resources = Ext.StaticData.GetAllResources(type)
        for j,uuid in ipairs(resources) do
            local res = Ext.StaticData.GetResource(uuid, type)
            AssertEquals(res.UUID, uuid)
        end
    end
end

function TestResourceLayout()
    local res = Ext.StaticData.GetResource("c277276f-3a23-4bc8-9c20-710e14117598", "AnimationBlueprint")
    local expect = {
        SourceFile = "C:/Program Files (x86)/Steam/steamapps/common/Baldurs Gate 3/Data/Editor/Mods/Shared/Assets/Blueprints/Characters_Common.lsabp",
        PackageName = "Shared/Assets/Characters/Humans/Male_Body",
        ModName = "Shared",
        UUID = "c277276f-3a23-4bc8-9c20-710e14117598",
        Name = "HUM_M_Base",
        Tags = {},
        IsLocalized = false,
        IsActive = true,
        IsDirty = false,
        IsOriginal = true,
    }

    AssertEqualsProperties(expect, res)
end
