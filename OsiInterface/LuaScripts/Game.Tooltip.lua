local math = math
local table = table
local debug = debug
local pairs = pairs
local type = type
local setmetatable = setmetatable
local xpcall = xpcall
local Ext = Ext

Game.Tooltip = {}

_ENV = Game.Tooltip
if setfenv ~= nil then
    setfenv(1, Game.Tooltip)
end

TooltipItemIds = {
    "ItemName","ItemWeight","ItemGoldValue","ItemLevel","ItemDescription","ItemRarity","ItemUseAPCost","ItemAttackAPCost","StatBoost",
    "ResistanceBoost","AbilityBoost","OtherStatBoost","VitalityBoost","ChanceToHitBoost","DamageBoost","APCostBoost","APMaximumBoost",
    "APStartBoost","APRecoveryBoost","CritChanceBoost","ArmorBoost","ConsumableDuration","ConsumablePermanentDuration","ConsumableEffect",
    "ConsumableDamage","ExtraProperties","Flags","ItemRequirement","WeaponDamage","WeaponDamagePenalty","WeaponCritMultiplier","WeaponCritChance",
    "WeaponRange","Durability","CanBackstab","AccuracyBoost","DodgeBoost","EquipmentUnlockedSkill","WandSkill","WandCharges","ArmorValue",
    "ArmorSlotType","Blocking","NeedsIdentifyLevel","IsQuestItem","PriceToIdentify","PriceToRepair","PickpocketInfo","Engraving",
    "ContainerIsLocked","SkillName","SkillIcon","SkillSchool","SkillTier","SkillRequiredEquipment","SkillAPCost","SkillCooldown",
    "SkillDescription","SkillProperties","SkillDamage","SkillRange","SkillExplodeRadius","SkillCanPierce","SkillCanFork","SkillStrikeCount",
    "SkillProjectileCount","SkillCleansesStatus","SkillMultiStrikeAttacks","SkillWallDistance","SkillPathSurface","SkillPathDistance",
    "SkillHealAmount","SkillDuration","ConsumableEffectUknown","Reflection","SkillAlreadyLearned","SkillOnCooldown","SkillAlreadyUsed",
    "AbilityTitle","AbilityDescription","TalentTitle","TalentDescription","SkillMPCost","MagicArmorValue","WarningText","RuneSlot",
    "RuneEffect","Equipped","ShowSkillIcon","SkillbookSkill","Tags","EmptyRuneSlot","StatName","StatsDescription","StatsDescriptionBoost",
    "StatSTRWeight","StatMEMSlot","StatsPointValue","StatsTalentsBoost","StatsTalentsMalus","StatsBaseValue","StatsPercentageBoost",
    "StatsPercentageMalus","StatsPercentageTotal","StatsGearBoostNormal","StatsATKAPCost","StatsCriticalInfos","StatsAPTitle","StatsAPDesc",
    "StatsAPBase","StatsAPBonus","StatsAPMalus","StatsTotalDamage","TagDescription","StatusImmunity","StatusBonus","StatusMalus","StatusDescription",
    "Title","SurfaceDescription","Duration","Fire","Water","Earth","Air","Poison","Physical","Sulfur","Heal","Splitter","ArmorSet"
}
TooltipItemTypes = {}

for i,type in pairs(TooltipItemIds) do
    TooltipItemTypes[type] = i
end

local _Label = {"Label", "string"}
local _Value = {"Value", "string"}
local _NumValue = {"Value", "number"}
local _Icon = {"Icon", "string"}
local _Warning = {"Warning", "string"}
local _Unused = {nil, nil}
local BoostSpec = {_Label, _NumValue, _Unused}

TooltipSpecs = {
    ItemName = {_Label},
    ItemWeight = {_Label, _Unused},
    ItemGoldValue = {_Label},
    ItemLevel = {_Label, _NumValue, _Unused},
    ItemDescription = {_Label},
    ItemRarity = {_Label},
    ItemUseAPCost = {_Label, _NumValue, {"RequirementMet", "boolean"}},
    ItemAttackAPCost = {_Label, _NumValue, _Warning, {"RequirementMet", "boolean"}},
    StatBoost = BoostSpec,
    ResistanceBoost = BoostSpec,
    AbilityBoost = BoostSpec,
    OtherStatBoost = {_Label, _Value, _Unused, _Unused},
    VitalityBoost = BoostSpec,
    ChanceToHitBoost = BoostSpec,
    DamageBoost = BoostSpec,
    APCostBoost = BoostSpec,
    APMaximumBoost = BoostSpec,
    APStartBoost = BoostSpec,
    APRecoveryBoost = BoostSpec,
    CritChanceBoost = BoostSpec,
    ArmorBoost = BoostSpec,
    ConsumableDuration = {_Label, _Unused, _Unused, _Value},
    ConsumablePermanentDuration = {_Label, _Value},
    ConsumableEffect = {_Label, _Unused, _Value, _Unused},
    ConsumableDamage = {_Unused, {"MinDamage", "number"}, {"MaxDamage", "number"}, {"DamageType", "number"}, _Label},
    ExtraProperties = {_Label, _Unused, _Unused, _Unused, _Unused},
    Flags = {_Label, _Unused, _Unused},
    ItemRequirement = {_Label, _Unused, {"RequirementMet", "boolean"}},
    WeaponDamage = {{"MinDamage", "number"}, {"MaxDamage", "number"}, _Label, {"DamageType", "number"}, _Unused},
    WeaponDamagePenalty = {_Label},
    WeaponCritMultiplier = {_Label, _Unused, _Unused, _Unused, _Value},
    WeaponCritChance = {_Label, _Value, _Unused, _Unused},
    WeaponRange = {_Label, _Unused, _Value, _Unused},
    Durability = {_Label, _NumValue, {"Max", "number"}, _Unused, _Unused},
    CanBackstab = {_Label, _Unused},
    AccuracyBoost = {_Label, _NumValue, _Unused},
    DodgeBoost = {_Label, _NumValue, _Unused},
    EquipmentUnlockedSkill = {_Label, _Value, {"Icon", "number"}},
    WandSkill = {_Label, _Value, _Icon, _Warning},
    WandCharges = {_Label, {"Value", "number"}, {"MaxValue", "number"}, _Unused, _Unused},
    ArmorValue = {_Label, _NumValue, _Unused, _Unused},
    ArmorSlotType = {_Label, _Unused, _Unused},
    Blocking = {_Label, _NumValue, _Unused, _Unused},
    NeedsIdentifyLevel = {_Label, _Unused, _Unused},
    IsQuestItem = {},
    PriceToIdentify = {_Label, _Value, _Unused},
    PriceToRepair = {_Label, _Value, _Unused},
    PickpocketInfo = {_Label, _Unused},
    Engraving = {_Label, _Unused},
    ContainerIsLocked = {_Label, _Unused},
    Tags = {_Label, _Value, _Warning},
    SkillName = {_Label},
    SkillIcon = {_Label},
    SkillSchool = {_Label, {"Icon", "number"}},
    SkillTier = {_Label, _Unused},
    SkillRequiredEquipment = {_Label, {"RequirementMet", "boolean"}},
    SkillAPCost = {_Label, _NumValue, _Warning, {"RequirementMet", "boolean"}},
    SkillCooldown = {_Label, _NumValue, _Warning, _Unused, {"ValueText", "string"}},
    SkillDescription = {_Label},
    SkillDamage = {_Label, {"MinValue", "number"}, {"MaxValue", "number"}, {"DamageType", "number"}},
    SkillRange = {_Value, _Unused, _Label},
    SkillExplodeRadius = {_Label, _Unused, _Value},
    SkillCanPierce = {_Label, _Value},
    SkillCanFork = {_Label, _Value, _Unused, _Unused, _Unused},
    SkillStrikeCount = {_Label, _Value, _Unused},
    SkillProjectileCount = {_Label, _Value, _Unused},
    SkillCleansesStatus = {_Label, _Value, _Unused},
    SkillMultiStrikeAttacks = {_Label, _Value, _Unused, _Unused},
    SkillWallDistance = {_Label, _Value, _Unused},
    SkillPathSurface = {_Label, _Value, _Unused},
    SkillPathDistance = {_Label, _Value, _Unused},
    SkillHealAmount = {_Label, _Unused, _Unused, _Value},
    SkillDuration = {_Label, _NumValue, _Unused, _Warning},
    ConsumableEffectUknown = {_Label, _Unused},
    Reflection = {_Label},
    SkillAlreadyLearned = {_Label},
    SkillOnCooldown = {_Label},
    SkillAlreadyUsed = {_Label},

    AbilityTitle = {_Label},
    AbilityDescription = {{"AbilityId", "number"}, {"Description", "string"}, {"Description2", "string"}, {"CurrentLevelEffect", "string"}, {"NextLevelEffect", "string"}},
    
    TalentTitle = {_Label},
    TalentDescription = {{"TalentId", "number"}, {"Description", "string"}, {"Requirement", "string"}, {"IncompatibleWith", "string"}, {"Selectable", "boolean"}, {"Unknown", "boolean"}},

    SkillMPCost = {_Label, _NumValue, {"RequirementMet", "boolean"}},
    MagicArmorValue = {_Label, _NumValue, _Unused, {"RequirementMet", "boolean"}},
    WarningText = {_Label},
    RuneSlot = {_Label, _Value, _Unused},
    RuneEffect = {{"Unknown1", "number"}, {"Rune1", "string"}, {"Rune2", "string"}, {"Rune3", "string"}, _Label, {"Label2", "string"}},
    Equipped = {{"EquippedBy", "string"}, _Label, {"Slot", "string"}},
    ShowSkillIcon = {_Unused},
    SkillbookSkill = {_Label, _Value, {"Icon", "number"}},
    EmptyRuneSlot = {_Label, _Value, _Unused},

    StatName = {_Label},
    StatsDescription = {_Label},
    StatsDescriptionBoost = {_Label, _NumValue},

    StatSTRWeight = {_Label},
    StatMEMSlot = {_Label},
    StatsPointValue = {_Label},
    StatsTalentsBoost = {_Label},
    StatsTalentsMalus = {_Label},
    StatsBaseValue = {_Label},
    StatsPercentageBoost = {_Label},
    StatsPercentageMalus = {_Label},
    StatsPercentageTotal = {_Label, _NumValue},
    StatsGearBoostNormal = {_Label},
    StatsATKAPCost = {_Label},
    StatsCriticalInfos = {_Label},
    StatsAPTitle = {_Label},
    StatsAPDesc = {_Label},
    StatsAPBase = {_Label},
    StatsAPBonus = {_Label},
    StatsAPMalus = {_Label},
    StatsTotalDamage = {_Label},

    TagDescription = {_Label, {"Image", "number"}},

    StatusImmunity = {_Label},
    StatusBonus = {_Label},
    StatusMalus = {_Label},
    StatusDescription = {_Label},

    Title = {_Label},
    SurfaceDescription = {_Label},
    Duration = {_Label},

    Fire = {_Label},
    Water = {_Label},
    Earth = {_Label},
    Air = {_Label},
    Poison = {_Label},
    Physical = {_Label},
    Sulfur = {_Label},
    Heal = {_Label},

    Splitter = {}
}

TooltipStatAttributes = {
    [0x0] = "Strength",
    [0x1] = "Finesse",
    [0x2] = "Intelligence",
    [0x3] = "Constitution",
    [0x4] = "Memory",
    [0x5] = "Wits",
    [0x6] = "Damage",
    [0x7] = "Armor",
    [0x8] = "MagicArmor",
    [0x9] = "CriticalChance",
    [0xA] = "Accuracy",
    [0xB] = "Dodge",
    [0xC] = "Vitality",
    [0xD] = "APRecovery",
    [0xE] = "Source",
    [0x11] = "Sight",
    [0x12] = "Hearing",
    [0x14] = "Movement",
    [0x15] = "Initiative",
    [0x17] = "PiercingResistance",
    [0x18] = "PhysicalResistance",
    [0x19] = "CorrosiveResistance",
    [0x1A] = "MagicResistance",
    [0x1B] = "ShadowResistance",
    [0x1C] = "FireResistance",
    [0x1D] = "WaterResistance",
    [0x1E] = "EarthResistance",
    [0x1F] = "AirResistance",
    [0x20] = "PoisonResistance",
    [0x21] = "CustomResistance",
    [0x24] = "Experience",
    [0x25] = "NextLevelExperience",
    [0x26] = "MaxAP",
    [0x27] = "StartAP",
    [0x28] = "APRecovery2",
    [0x2A] = "MinDamage",
    [0x2B] = "MaxDamage",
    [0x2C] = "LifeSteal",
    [0x2D] = "Gain",
}

--- @param ui UIObject
--- @param name string MainTimeline property name to fetch
--- @return table
function TableFromFlash(ui, name)
    local value
    local idx = 0
    local tbl = {}

    repeat
        value = ui:GetValue(name, nil, idx)
        idx = idx + 1
        if value ~= nil then
            table.insert(tbl, value)
        end
    until value == nil

    return tbl
end

--- @param ui UIObject
--- @param name string MainTimeline property name to write
--- @param tbl table Table to convert to Flash
function TableToFlash(ui, name, tbl)
    for i,value in pairs(tbl) do
        ui:SetValue(name, value, i-1)
    end
end

--- @param ui UIObject Tooltip UI object
--- @param propertyName string Flash property name (tooltip_array, tooltipCompare_array, etc.)
--- @param tooltipArray table Tooltip array
--- @param originalTooltipArray table Unmodified tooltip array
function ReplaceTooltipArray(ui, propertyName, tooltipArray, originalTooltipArray)
    TableToFlash(ui, propertyName, tooltipArray)

    if #tooltipArray < #originalTooltipArray then
        -- Pad out the tooltip array with dummy values
        for i=#tooltipArray,#originalTooltipArray do
            ui:SetValue(propertyName, TooltipItemTypes.IsQuestItem, i)
        end
    end
end

function ParseTooltipElement(tt, index, spec, typeName)
    if #tt - index + 1 < #spec then
        Ext.PrintError("Not enough fields to parse spec @" .. index)
        return
    end

    local element = {Type = typeName}
    for i,field in pairs(spec) do
        local val = tt[index + i - 1]
        if field[1] ~= nil then
            element[field[1]] = val
        end
        if field[2] ~= nil and type(val) ~= field[2] then
            Ext.PrintWarning("Type of field " .. typeName .. "." .. field[1] .. " differs: " .. type(val) .. " vs " .. field[2] .. ":", val)
        end
    end

    return index + #spec, element
end

function ParseTooltipSkillProperties(tt, index)
    local element = {
        Type = "SkillProperties",
        Properties = {},
        Resistances = {}
    }

    local numProps = tt[index + 1]
    index = index + 2

    for i=1,numProps do
        local prop = {
            Label = tt[index],
            Warning = tt[index + 1]
        }
        index = index + 2
        table.insert(element.Properties, prop)
    end
    
    local numResistances = tt[index]
    index = index + 1

    for i=1,numResistances do
        local resist = {
            Label = tt[index],
            Value = tt[index + 1]
        }
        index = index + 2
        table.insert(element.Resistances, resist)
    end

    return index, element
end

function ParseTooltipArmorSet(tt, index)
    local element = {
        Type = "ArmorSet",
        GrantedStatuses = {},
        GrantedStatuses2 = {}
    }

    element.SetName = tt[index]
    element.FoundPieces = tt[index + 1]
    element.TotalPieces = tt[index + 2]
    element.SetDescription = tt[index + 3]
    local numStatuses = tt[index + 4]
    index = index + 5

    for i=1,numStatuses do
        local prop = {
            Label = tt[index],
            IconIndex = tt[index + 1]
        }
        index = index + 2
        table.insert(element.GrantedStatuses, prop)
    end
    
    local numStatuses2 = tt[index]
    index = index + 1

    for i=1,numStatuses2 do
        local resist = {
            Label = tt[index],
            IconIndex = tt[index + 1]
        }
        index = index + 2
        table.insert(element.GrantedStatuses2, resist)
    end

    return index, element
end

--- @param tt table Flash tooltip array
--- @return table
function ParseTooltipArray(tt)
    local index = 1
    local element
    local elements = {}

    while index <= #tt do
        local id = tt[index]
        index = index + 1

        if TooltipItemIds[id] == nil then
            Ext.PrintError("Encountered unknown tooltip item type: ", id)
            return elements
        end

        local typeName = TooltipItemIds[id]
        if typeName == "SkillProperties" then
            index, element = ParseTooltipSkillProperties(tt, index)
        elseif typeName == "ArmorSet" then
            index, element = ParseTooltipArmorSet(tt, index)
        else
            local spec = TooltipSpecs[typeName]
            if spec == nil then
                Ext.PrintError("No spec available for tooltip item type: ", typeName)
                return elements
            end

            index, element = ParseTooltipElement(tt, index, spec, typeName)
            if element == nil then
                return elements
            end
        end

        table.insert(elements, element)
    end

    return elements
end

function EncodeTooltipElement(tt, spec, element)
    for i,field in pairs(spec) do
        local name = field[1]
        local fieldType = field[2]
        local val = element[name]
        if name == nil then
            table.insert(tt, "")
        else
            if fieldType ~= nil and type(val) ~= fieldType then
                Ext.PrintWarning("Type of field " .. element.Type .. "." .. name .. " differs: " .. type(val) .. " vs " .. fieldType .. ":", val)
                val = nil
            end

            if val == nil then
                if fieldType == "boolean" then
                    val = false
                elseif fieldType == "number" then
                    val = 0
                else
                    val = ""
                end
            end

            table.insert(tt, val)
        end
    end
end

function EncodeTooltipSkillProperties(tt, element)
    local properties = element.Properties or {}
    table.insert(tt, "")
    table.insert(tt, #properties)
    for i,prop in pairs(properties) do
        table.insert(tt, prop.Label or "")
        table.insert(tt, prop.Warning or "")
    end

    local resistances = element.Resistances or {}
    table.insert(tt, #resistances)
    for i,prop in pairs(resistances) do
        table.insert(tt, prop.Label or "")
        table.insert(tt, prop.Value or "")
    end
end

function EncodeTooltipArmorSet(tt, element)
    local statuses = element.GrantedStatuses or {}
    local statuses2 = element.GrantedStatuses2 or {}

    table.insert(tt, element.SetName or "")
    table.insert(tt, element.FoundPieces or 0)
    table.insert(tt, element.TotalPieces or 0)
    table.insert(tt, element.SetDescription or "")

    table.insert(tt, #statuses)
    for i,status in pairs(statuses) do
        table.insert(tt, status.Label or "")
        table.insert(tt, status.IconIndex or "")
    end

    table.insert(tt, #statuses2)
    for i,status in pairs(statuses2) do
        table.insert(tt, status.Label or "")
        table.insert(tt, status.IconIndex or "")
    end
end

--- @param tt table Flash tooltip array
--- @return table
function EncodeTooltipArray(elements)
    local tt = {}
    for i,element in pairs(elements) do
        local type = TooltipItemTypes[element.Type]
        if type == nil then
            Ext.PrintWarning("Couldn't encode tooltip element with unknown type:", element.Type)
        else
            if element.Type == "SkillProperties" then
                table.insert(tt, type)
                EncodeTooltipSkillProperties(tt, element)
            elseif element.Type == "ArmorSet" then
                table.insert(tt, type)
                EncodeTooltipArmorSet(tt, element)
            else
                local spec = TooltipSpecs[element.Type]
                if spec == nil then
                    Ext.PrintWarning("No encoder found for tooltip element type:", element.Type)
                else
                    table.insert(tt, type)
                    EncodeTooltipElement(tt, spec, element)
                end
            end
        end
    end

    return tt
end

function DebugTooltipEncoding(ui)
    local tooltipArray = TableFromFlash(ui, "tooltip_array")
    local tooltipArray2 = {}

    for i,s in pairs(tooltipArray) do
        if s ~= nil and type(s) == "number" and TooltipItemIds[s] ~= nil then
            s = "TYPE: " .. TooltipItemIds[s]
        end

        tooltipArray2[i] = s
    end

    Ext.Print("tooltip_array: " .. Ext.JsonStringify(tooltipArray2))
    local parsed = ParseTooltipArray(tooltipArray)
    Ext.Print("Parsed: " .. Ext.JsonStringify(parsed))
    local encoded = EncodeTooltipArray(parsed)
    local parsed2 = ParseTooltipArray(encoded)
    Ext.Print("Encoding matches: ", Ext.JsonStringify(parsed2) == Ext.JsonStringify(parsed))
end

TooltipHooks = {
    NextRequest = nil,
    SessionLoaded = false,
    InitializationRequested = false,
    Initialized = false,
    GlobalListeners = {},
    TypeListeners = {},
    ObjectListeners = {},
}

function TooltipHooks:Init()
    if self.Initialized then
        return
    end

    for i = 1,150 do
        local ui = Ext.GetUIByType(i)
        if ui ~= nil then
            ui:CaptureExternalInterfaceCalls()
            ui:CaptureInvokes()
        end
    end

    local onReqTooltip = function (...)
        self:OnRequestTooltip(...)
    end
    Ext.RegisterUINameCall("showSkillTooltip", onReqTooltip)
    Ext.RegisterUINameCall("showStatusTooltip", onReqTooltip)
    Ext.RegisterUINameCall("showItemTooltip", onReqTooltip)
    Ext.RegisterUINameCall("showStatTooltip", onReqTooltip)
    Ext.RegisterUINameCall("showAbilityTooltip", onReqTooltip)
    Ext.RegisterUINameCall("showTalentTooltip", onReqTooltip)

    Ext.RegisterUINameInvokeListener("addFormattedTooltip", function (...)
        self:OnRenderTooltip(...)
    end)
    Ext.RegisterUINameInvokeListener("addStatusTooltip", function (...)
        self:OnRenderTooltip(...)
    end)

    Ext.RegisterUITypeCall(104, "showTooltip", function (...)
        self:OnRequestExamineUITooltip(...)
    end)

    Ext.RegisterListener("UIObjectCreated", function (ui)
        ui:CaptureExternalInterfaceCalls()
        ui:CaptureInvokes()
    end)

    self.Initialized = true
end

--- @param ui UIObject
function TooltipHooks:OnRequestTooltip(ui, method, arg1, arg2, arg3, ...)
    local request = {}
    local isCharSheet = ui:GetTypeId() == 119

    if method == "showSkillTooltip" then
        request.Type = 'Skill'
        request.Character = Ext.DoubleToHandle(arg1)
        request.Skill = arg2
    elseif method == "showStatusTooltip" then
        request.Type = 'Status'
        request.Character = Ext.DoubleToHandle(arg1)
        request.Status = Ext.DoubleToHandle(arg2)
    elseif method == "showItemTooltip" then
        if arg1 == nil then
            -- Item handle will be nil when it's being dragged
            return
        end

        request.Type = 'Item'
        request.Item = Ext.DoubleToHandle(arg1)
    elseif method == "showStatTooltip" then
        request.Type = 'Stat'
        if isCharSheet then
            request.Character = ui:GetPlayerHandle()
            request.Stat = arg1
        else
            request.Character = Ext.DoubleToHandle(arg1)
            request.Stat = arg2
        end

        local stat = TooltipStatAttributes[request.Stat]
        if stat ~= nil then
            request.Stat = stat
        else
            Ext.PrintWarning("Requested tooltip for unknown stat ID " .. request.Stat)
        end
    elseif method == "showAbilityTooltip" then
        request.Type = 'Ability'
        if isCharSheet then
            request.Character = ui:GetPlayerHandle()
            request.Ability = arg1
        else
            request.Character = Ext.DoubleToHandle(arg1)
            request.Ability = arg2
        end

        request.Ability = Ext.EnumIndexToLabel("AbilityType", request.Ability)
    elseif method == "showTalentTooltip" then
        request.Type = 'Talent'
        if isCharSheet then
            request.Character = ui:GetPlayerHandle()
            request.Talent = arg1
        else
            request.Character = Ext.DoubleToHandle(arg1)
            request.Talent = arg2
        end

        request.Talent = Ext.EnumIndexToLabel("TalentType", request.Talent)
    else
        Ext.PrintError("Unknown tooltip request method?", method)
        return
    end

    if request.Character ~= nil then
        request.Character = Ext.GetCharacter(request.Character)
    end

    if request.Status ~= nil then
        request.Status = Ext.GetStatus(request.Character.Handle, request.Status)
    end

    if request.Item ~= nil then
        request.Item = Ext.GetItem(request.Item)
    end

    if self.NextRequest ~= nil then
        Ext.PrintWarning("Previous tooltip request not cleared in render callback?")
    end

    self.NextRequest = request
end

--- @param ui UIObject
function TooltipHooks:OnRequestExamineUITooltip(ui, method, typeIndex, id, ...)
    local request = {
        Character = Ext.GetCharacter(ui:GetPlayerHandle())
    }

    if typeIndex == 1 then
        request.Type = 'Stat'
        request.Stat = TooltipStatAttributes[id]

        if request.Stat == nil then
            Ext.PrintWarning("Requested tooltip for unknown stat ID " .. id)
        end
    elseif typeIndex == 2 then
        request.Type = 'Ability'
        request.Ability = Ext.EnumIndexToLabel("AbilityType", id)
    elseif typeIndex == 3 then
        request.Type = 'Talent'
        request.Talent = Ext.EnumIndexToLabel("TalentType", id)
    elseif typeIndex == 7 then
        request.Type = 'Status'
        request.Status = Ext.GetStatus(request.Character.Handle, Ext.DoubleToHandle(id))
    else
        return
    end

    if self.NextRequest ~= nil then
        Ext.PrintWarning("Previous tooltip request not cleared in render callback?")
    end

    self.NextRequest = request
end

--- @param ui UIObject
--- @param item EclItem
--- @param offHand boolean
--- @return string|nil
function TooltipHooks:GetCompareItem(ui, item, offHand)
    local owner = ui:GetPlayerHandle()
    if owner == nil then
        owner = item:GetOwnerCharacter()
    end

    if owner == nil then
        Ext.PrintError("Tooltip compare render failed: Couldn't find owner of item")
        return nil
    end

    --- @type EclCharacter
    local char = Ext.GetCharacter(owner)

    if item.Stats.ItemSlot == "Weapon" then
        if offHand then
            return char:GetItemBySlot("Shield")
        else
            return char:GetItemBySlot("Weapon")
        end
    elseif item.Stats.ItemSlot == "Ring" or item.Stats.ItemSlot == "Ring2" then
        if offHand then
            return char:GetItemBySlot("Ring2")
        else
            return char:GetItemBySlot("Ring")
        end
    else
        return char:GetItemBySlot(item.Stats.ItemSlot)
    end
end

--- @param ui UIObject
function TooltipHooks:OnRenderTooltip(ui, method, ...)
    if self.NextRequest == nil then
        Ext.PrintError("Got tooltip render request, but did not find original tooltip info!")
        return
    end

    local req = self.NextRequest
    self:OnRenderSubTooltip(ui, "tooltip_array", req, method, ...)

    if req.Type == "Item" then
        local reqItem = req.Item

        if ui:GetValue("tooltipCompare_array", nil, 0) ~= nil then
            local compareItem = self:GetCompareItem(ui, reqItem, false)
            if compareItem ~= nil then
                req.Item = Ext.GetItem(compareItem)
                self:OnRenderSubTooltip(ui, "tooltipCompare_array", req, method, ...)
                req.Item = reqItem
            else
                Ext.PrintError("Tooltip compare render failed: Couldn't find item to compare")
            end
        end

        if ui:GetValue("tooltipOffHand_array", nil, 0) ~= nil then
            local compareItem = self:GetCompareItem(ui, reqItem, true)
            if compareItem ~= nil then
                req.Item = Ext.GetItem(compareItem)
                self:OnRenderSubTooltip(ui, "tooltipOffHand_array", req, method, ...)
                req.Item = reqItem
            else
                Ext.PrintError("Tooltip compare render failed: Couldn't find off-hand item to compare")
            end
        end
    end

    self.NextRequest = nil
end

function TooltipHooks:OnRenderSubTooltip(ui, propertyName, req, method, ...)
    local tt = TableFromFlash(ui, propertyName)
    local params = ParseTooltipArray(tt)
    if params ~= nil then
        local tooltip = TooltipData:Create(params)
        if req.Type == "Stat" then
            self:NotifyListeners("Stat", req.Stat, req, tooltip, req.Character, req.Stat)
        elseif req.Type == "Skill" then
            self:NotifyListeners("Skill", req.Skill, req, tooltip, req.Character, req.Skill)
        elseif req.Type == "Ability" then
            self:NotifyListeners("Ability", req.Ability, req, tooltip, req.Character, req.Ability)
        elseif req.Type == "Talent" then
            self:NotifyListeners("Talent", req.Talent, req, tooltip, req.Character, req.Talent)
        elseif req.Type == "Status" then
            self:NotifyListeners("Status", req.Status.StatusId, req, tooltip, req.Character, req.Status)
        elseif req.Type == "Item" then
            self:NotifyListeners("Item", nil, req, tooltip, req.Item)
        else
            Ext.PrintError("Unknown tooltip type? ", req.Type)
        end

        newTooltip = EncodeTooltipArray(tooltip.Data)
        if newTooltip ~= nil then
            ReplaceTooltipArray(ui, propertyName, newTooltip, tt)
        end
    end
end

function TooltipHooks:NotifyListeners(type, name, request, tooltip, ...)
    local args = {...}
    table.insert(args, tooltip)
    self:NotifyAll(self.TypeListeners[type], table.unpack(args))
    if name ~= nil and self.ObjectListeners[type] ~= nil then
        self:NotifyAll(self.ObjectListeners[type][name], table.unpack(args))
    end

    self:NotifyAll(self.GlobalListeners, request, tooltip)
end

function TooltipHooks:NotifyAll(listeners, ...)
    for i,callback in pairs(listeners or {}) do
        local status, err = xpcall(callback, debug.traceback, ...)
        if not status then
            Ext.PrintError("Error during tooltip callback: ", err)
        end
    end
end

function TooltipHooks:RegisterListener(type, name, listener)
    if not self.Initialized then
        if self.SessionLoaded then
            self:Init()
        else
            self.InitializationRequested = true
        end
    end

    if type == nil then
        table.insert(self.GlobalListeners, listener)
    elseif name == nil then
        if self.TypeListeners[type] == nil then
            self.TypeListeners[type] = {listener}
        else
            table.insert(self.TypeListeners[type], listener)
        end
    else
        local listeners = self.ObjectListeners[type]
        if listeners == nil then
            self.ObjectListeners[type] = {[name] = {listener}}
        else
            if listeners[name] == nil then
                listeners[name] = {listener}
            else
                table.insert(listeners[name], listener)
            end
        end
    end
end

TooltipData = {}

function TooltipData:Create(data)
    local tt = {
        Data = data
    }
    setmetatable(tt, {__index = self})
    return tt
end

function TooltipData:GetElement(type)
    for i,element in pairs(self.Data) do
        if element.Type == type then
            return element
        end
    end
end

function TooltipData:GetElements(type)
    local elements = {}
    for i,element in pairs(self.Data) do
        if element.Type == type then
            table.insert(elements, element)
        end
    end

    return elements
end

function TooltipData:RemoveElements(type)
    for i=#self.Data,1,-1 do
        if self.Data[i].Type == type then
            table.remove(self.Data, i)
        end
    end
end

function TooltipData:RemoveElement(ele)
    for i,element in pairs(self.Data) do
        if element == ele then
            table.remove(self.Data, i)
            break
        end
    end
end

function TooltipData:AppendElement(ele)
    table.insert(self.Data, ele)
end

function TooltipData:AppendElementAfter(ele, appendAfter)
    for i,element in pairs(self.Data) do
        if element == appendAfter then
            table.insert(self.Data, i+1, ele)
            return
        end
    end

    table.insert(self.Data, ele)
end

function RegisterListener(...)
    local args = {...}
    if #args == 1 then
        TooltipHooks:RegisterListener(nil, nil, args[1])
    elseif #args == 2 then
        TooltipHooks:RegisterListener(args[1], nil, args[2])
    else
        TooltipHooks:RegisterListener(args[1], args[2], args[3])
    end
end

local function OnSessionLoaded()
    TooltipHooks.SessionLoaded = true
    if TooltipHooks.InitializationRequested then
        TooltipHooks:Init()
    end
end

Ext.RegisterListener("SessionLoaded", OnSessionLoaded)
