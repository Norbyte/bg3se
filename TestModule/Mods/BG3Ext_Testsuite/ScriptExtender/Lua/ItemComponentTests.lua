local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestItemComponentGeneric(componentName, expect)
    local stats = Ext.GetCharacter(GUID_LAEZEL).Stats
    local it = stats.Equipment.MeleeMainWeapon.ItemHandle
    local component = it[componentName]

    -- FIXME - add ==, ~= operator for entities, component handles
    -- expect.Entity = ch
    AssertEqualsProperties(expect, component)
end

function TestItemComponentDump()
    local stats = Ext.GetCharacter(GUID_LAEZEL).Stats
    local it = stats.Equipment.MeleeMainWeapon.ItemHandle

    Ext.Json.Stringify(it:GetAllComponents(), true, true)
end


function TestItemCanBeInInventoryComponent()
    local expect = {
        Flags = 65535
    }
    
    TestItemComponentGeneric("CanBeInInventory", expect)
end


function TestItemCombatParticipantComponent()
    -- FIXME - also test when in combat!
    local expect = {
        AiHint = "00000000-0000-0000-0000-000000000000",
        Archetype = "base",
        CombatGroupChangedFlag_M = false,
        CombatGroupId = "",
        CombatHandle = null,
        Flags = 66
    }
    
    TestItemComponentGeneric("CombatParticipant", expect)
end


function TestItemDataComponent()
    local expect = {
        Flags = 0,
        StatsId = "WPN_Longsword",
        StepsType = 0,
        Weight_M = 1.35,
    }
    
    TestItemComponentGeneric("Data", expect)
end


function TestCanBeInInventoryComponent()
    local expect = {
        Flags = 65535
    }
    
    TestItemComponentGeneric("CanBeInInventory", expect)
end


function TestItemDisplayNameComponent()
    local expect = {
        Name = "",
        NameKey = "hfb60d298g3e34g4038g9b1bg9875e92ef98d",
        UnknownKey = "ls::TranslatedStringRepository::s_HandleUnknown"
    }
    
    TestItemComponentGeneric("DisplayName", expect)
end


function TestItemEquipableComponent()
    local expect = {
        Slot_M = 3,
        field_18 = "f85002a2-8e0e-4a49-aa0f-f52e987d3a3a"
    }
    
    TestItemComponentGeneric("Equipable", expect)
end


function TestItemGameObjectVisualComponent()
    local expect = {
        Icon = "Item_WPN_HUM_Longsword_A_0",
        RootTemplateId = "1865323f-b428-4791-a0a9-578841e57463",
        RootTemplateType = 0,
        field_24 = 1.0,
        field_28 = 0
    }
    
    TestItemComponentGeneric("GameObjectVisual", expect)
end


function TestItemInventorySlotComponent()
    local expect = {
        Slot = 3
    }
    
    TestItemComponentGeneric("InventorySlot", expect)
end


function TestItemBoostsComponent()
    -- FIXME - test on an item with boosts ?
    local expect = {
        field_18 = {}
    }
    
    TestItemComponentGeneric("ItemBoosts", expect)
end


function TestItemStatusImmunitiesComponent()
    local expect = {
        PersonalStatusImmunities = {
            BLEEDING = "00000000-0000-0000-0000-0000ffffffff",
            BURNING = "00000000-ffff-ffff-0000-000000000000",
            DEAF = "00000000-ffff-ffff-0000-000000000000",
            SG_Condition = "00000000-ffff-ffff-0000-0000ffffffff",
            SILENCED = "00000000-0000-0000-0000-0000ffffffff"
        }
    }
    
    TestItemComponentGeneric("StatusImmunities", expect)
end


function TestItemUseComponent()
    local expect = {
        Boosts = {},
        Charges = 0,
        ItemComboFlag1 = 0,
        ItemComboFlag2 = 0,
        ItemUseType = 255,
        MaxCharges = 0,
    }
    
    TestItemComponentGeneric("Use", expect)
end


function TestItemValueComponent()
    local expect = {
        Rarity = 0,
        Unique = false,
        Value = 75
    }
    
    TestItemComponentGeneric("Value", expect)
end


function TestItemWeaponComponent()
    local expect = {
        Ability = "Strength",
        DamageRange = 0.0,
        Rolls = {
            Strength = {
                {
                    AmountOfDices = 1,
                    DiceAdditionalValue = 0,
                    DiceValue = 2
                }
            }
        },
        Rolls2 = {
            Strength = {
                {
                    AmountOfDices = 1,
                    DiceAdditionalValue = 0,
                    DiceValue = 3
                }
            }
        },
        WeaponGroup = 2,
        WeaponProperties = 14336,
        WeaponRange = 1.5
    }
    
    TestItemComponentGeneric("Weapon", expect)
end


function TestItemWieldingComponent()
    local expect = {
        -- FIXME - entity equality operator
        -- Owner = Ext.GetCharacter(GUID_LAEZEL)
    }
    
    TestItemComponentGeneric("Wielding", expect)
end


function TestItemComponent()
    local expect = {
        Amount = 1,
        Armor = 0,
        CurrentLevel = "",
        CustomDescription = "",
        CustomDescription2 = "",
        DisableUse = false,
        Flags = 16620,
        Flags2 = 2,
        ForceSynch = 0,
        GUID = "c9d5a764-837c-4a7c-b61b-9859e46ed8a6",
        Generation = null,
        GoldValueOverwrite = -1,
        InUseByCharacterHandle_M = null,
        InheritedForceSynch = 0,
        InventoryHandle = null,
        ItemType = "Common",
        MovingCount = 0,
        PreviousLevel = "",
        StatsId = "WPN_Longsword",
        TeleportTargetOverrideHandle = null,
        TeleportsUseCount = -1,
        TreasureLevel = -1,
        UserId_M = -65536,
        Vitality = -1,
        WeightValueOverwrite = -1
    }
    
    TestItemComponentGeneric("ServerItem", expect)
end



function TestItemTemplate()
    local stats = Ext.GetCharacter(GUID_LAEZEL).Stats
    local it = stats.Equipment.MeleeMainWeapon.ItemHandle
    local template = it.ServerItem.CurrentTemplate
    
    local expect = {
        AllowReceiveDecalWhenAnimated = false,
        AllowSummonTeleport = false,
        AltSpeaker = "",
        Amount = 1,
        AnubisConfigName = "",
        BloodType = "",
        CameraOffset = { 0.0, 0.0, 0.0 },
        CanBeMoved = true,
        CanBePickedUp = true,
        CanClickThrough = false,
        CanClimbOn = false,
        CanShootThrough = true,
        CastShadow = true,
        CollideWithCamera = false,
        CombatComponent = {
                AiHint = "00000000-0000-0000-0000-000000000000",
                Alignment = "376e33c4-dd15-4c46-a586-87d8b421a0c7",
                Archetype = "base",
                CanFight = false,
                CanJoinCombat = true,
                CombatGroupID = "",
                CombatName = "",
                IsBoss = false,
                IsInspector = true,
                StartCombatRange = -1.0,
                StayInAiHints = false,
                Unknown = false,
                Unknown2 = false
        },
        CoverAmount = false,
        DefaultState = "",
        Description = "h0e835080g139eg4f43g8a5cgc42ca3be203e",
        Destroyed = false,
        DisarmDC = -1,
        DisplayName = "hfb60d298g3e34g4038g9b1bg9875e92ef98d",
        EquipSound = "da469251-95cf-5387-6295-5b1d5ce5a41f",
        EquipmentTypeID = "f85002a2-8e0e-4a49-aa0f-f52e987d3a3a",
        FadeChildren = {},
        FadeGroup = "",
        FadeIn = false,
        Fadeable = false,
        FileName = "C:/Program Files (x86)/Steam/steamapps/common/Baldurs Gate 3/Data/Public/Shared/RootTemplates/_merged.lsf",
        Flag = 0,
        Flags = 0,
        FreezeGravity = false,
        GameMasterSpawnSubSection = -1,
        GenerateRunningDeepWater = false,
        GroupID = 0,
        HLOD = "00000000-0000-0000-0000-000000000000",
        HardcoreOnly = false,
        HasGameplayValue = false,
        HasParentModRelation = false,
        HierarchyOnlyFade = false,
        Hostile = false,
        Icon = "Item_WPN_HUM_Longsword_A_0",
        Id = "1865323f-b428-4791-a0a9-578841e57463",
        IgnoreGenerics = false,
        ImpactSound = "439fcec3-6032-f489-6e80-260d26fab23d",
        InventoryList = { "Empty" },
        InventoryMoveSound = "f55dc72a-13b8-6e3d-7500-559a5d1f3031",
        InventoryType = 0,
        IsBlocker = false,
        IsDecorative = false,
        IsDeleted = false,
        IsForcedType = false,
        IsGlobal = false,
        IsHuge = false,
        IsInteractionDisabled = false,
        IsKey = false,
        IsPlatformOwner = false,
        IsPointerBlocker = false,
        IsPortalProhibitedToPlayers = false,
        IsPublicDomain = false,
        IsReflecting = false,
        IsShadowProxy = false,
        IsSourceContainer = false,
        IsSurfaceBlocker = false,
        IsSurfaceCloudBlocker = false,
        IsTrap = false,
        Key = "",
        LevelName = "",
        LevelOverride = -1,
        LightChannel = 0,
        LockDC = -1,
        LongHair = "",
        LoopSound = "",
        MaxStackAmount = 1,
        MeshProxy = "",
        Name = "",
        NonUniformScale = false,
        NotHardcore = false,
        OnUseDescription = "ls::TranslatedStringRepository::s_HandleUnknown",
        Opacity = 0.0,
        Owner = "",
        ParentTemplateId = "344ac552-11ff-4150-8bb4-7eed8fb78588",
        PhysicsFollowAnimation = false,
        PhysicsOpenTemplate = "",
        PhysicsTemplate = "0671a245-3533-6589-a71b-e5948c9c6826",
        PickupSound = "28ea7a9f-b66a-3410-7985-d7cd5cb896f0",
        ReceiveDecal = false,
        RenderChannel = 4,
        SeeThrough = false,
        ShortHair = "",
        SoundAttenuation = -1,
        SoundInitEvent = "",
        SoundObjectIndex = 255,
        Speaker = "",
        SpeakerGroupList = {},
        Stats = "WPN_Longsword",
        StoryItem = false,
        TemplateName = "",
        Tooltip = 2,
        Transform = {
                Scale = { 1.0, 1.0, 1.0 },
                Translate = { 0.0, 0.0, 0.0 },
        },
        TreasureLevel = -1,
        TreasureOnDestroy = false,
        UnequipSound = "503428af-9061-40a7-b9c4-64c8ec78fff3",
        Unimportant = false,
        UnknownDescription = "ls::TranslatedStringRepository::s_HandleUnknown",
        UnknownDisplayName = "ls::TranslatedStringRepository::s_HandleUnknown",
        UseOnDistance = false,
        UsePartyLevelForTreasureLevel = false,
        UseRemotely = false,
        UseSound = "",
        VisualTemplate = "c1319fce-3d47-ac75-e502-a3e965500038",
        Wadable = false,
        WalkOn = false,
        WalkThrough = true
    }

    AssertEqualsProperties(expect, template)
end


-- FIXME:
-- BoostsContainer
-- ServerDisplayNameList -> multiple names?
