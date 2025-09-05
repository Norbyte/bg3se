local _DEBUG = Ext.Debug.IsDeveloperMode()

local _format = string.format

local NEWLINE = "\r\n"

---@type {Specific:table<string,string>, Misc:string[], Entity:string}
local _CustomEntries = Ext.Utils.Include(nil, "builtin://Libs/HelpersGenerator/CustomEntries.lua")

---@type table<string,{Before:string?, After:string?, Replace:string?}>
local _CustomTypeEntries = Ext.Utils.Include(nil, "builtin://Libs/HelpersGenerator/CustomTypeEntries.lua")

---@type table<string,{Before:string?, After:string?, Replace:string?, ReplaceReturnValue:string?}>

local _CustomFunctionExtras = Ext.Utils.Include(nil, "builtin://Libs/HelpersGenerator/CustomFunctionExtras.lua")

---@type table<string,table<string, string>>
local _Aliases = Ext.Utils.Include(nil, "builtin://Libs/HelpersGenerator/Aliases.lua")

---@type {Regular:table<string, table<string, {Params:{name:string, arg:string, description:string}, Return:{}}>>, Field:table<string, table<string, string>>}
local _FuncData = Ext.Utils.Include(nil, "builtin://Libs/HelpersGenerator/FunctionFixes.lua")

local _TypeAliasReplace = {
    ComponentHandle = "userdata",
    EntityHandle = "userdata",
    int16 = "integer",
    int32 = "integer",
    int64 = "integer",
    int8 = "integer",
    uint16 = "integer",
    uint32 = "integer",
    uint64 = "integer",
    uint8 = "integer",
    UserId = "integer",
    NetId = "integer",
    IggyInvokeDataValue = "boolean|number|string",
}

local _TypeReplace = {
    STDString = "string",
    STDWString = "string",
    CString = "string",
    bool = "boolean",
    double = "number",
    float = "number",
}

local _TypeAliasRemove = {
    STDString = true,
    STDWString = true,
    CString = true,
    bool = true,
    double = true,
    float = true,
}

--- @class GenerateIdeHelpersGenerator
local Generator = {}

function Generator.Trim(s)
    return s:gsub("^%s*(.-)%s*$", "%1")
end

function Generator.RTrim(s)
    return s:gsub("^(.-)%s*$", "%1")
end

Generator.ValueKindToLua = {
    Boolean = "boolean",
    Integer = "number",
    Float = "number",
    String = "string",
    Enumeration = "string",
    Any = "any"
}

local startingText = [[--- @meta
--- @diagnostic disable

--- Table that contains every ModTable entry for active mods.
Mods = {}

--- Special global value that contains the current mod UUID during load
--- @type FixedString
ModuleUUID = "UUID"

--- Table that gets stored in the save, unique for each ModTable entry.
--- @type table
PersistentVars = {}

--- @alias OsirisEventType string|"before"|"after"|"beforeDelete"|"afterDelete"
--- @alias i16vec2 int16[]
--- @alias YesNo "Yes"|"No"
]]

local dynamicTypesText = [[
--- @alias OsirisValue number|string

--- Using a DB like a function will allow inserting new values into the database (ex. `Osi.DB_IsPlayer("02a77f1f-872b-49ca-91ab-32098c443beb")`  
--- @overload fun(...:OsirisValue?)
--- @class OsiDatabase
local OsiDatabase = {}
--- Databases can be read using the Get method. The method checks its parameters against the database and only returns rows that match the query.  
--- The number of parameters passed to Get must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be returned.
--- @param ... OsirisValue?
--- @return table<integer,table<integer,OsirisValue>>
function OsiDatabase:Get(...) end
--- The Delete method can be used to delete rows from databases.  
--- The number of parameters passed to Delete must be equivalent to the number of columns in the target database.  
--- Each parameter defines an (optional) filter on the corresponding column.  
--- If the parameter is nil, the column is not filtered (equivalent to passing _ in Osiris). If the parameter is not nil, only rows with matching values will be deleted. 
--- @param ... OsirisValue?
function OsiDatabase:Delete(...) end

--- @alias OsiFunction fun(...:OsirisValue):OsirisValue?
--- @alias OsiDynamic table<string, OsiFunction|OsiDatabase>

--- @class OsiCommonDatabases
--- @field DB_IsPlayer OsiDatabase|fun(Guid:string) All player characters
--- @field DB_Origins OsiDatabase|fun(Guid:string) All origin characters
--- @field DB_Avatars OsiDatabase|fun(Guid:string) All player characters that were created in character creation, or that have an `AVATAR` tag
--- @field DB_CombatObjects OsiDatabase|fun(Guid:string, combatID:integer) All objects in combat
--- @field DB_CombatCharacters OsiDatabase|fun(Guid:string, combatID:integer) All characters in combat
--- @field DB_Dialogs OsiDatabase|fun(Guid:string, dialog:string)|fun(GUID1:string, GUID2:string, dialog:string)|fun(GUID1:string, GUID2:string, GUID3:string, dialog:string)|fun(GUID1:string, GUID2:string, GUID3:string, GUID4:string, dialog:string) All registered dialogs for objects, the most common being the version with a single character

--- The Osi table contains databases as well as calls, queries, events, and custom PROC / QRY defintions, as long as they are used in a script.  
--- @type OsiCommonDatabases|OsiDynamic
Osi = {}
]]

--- @return GenerateIdeHelpersGenerator
function Generator:New()
    local o = {}
    setmetatable(o, self)
    o.Intrinsics = {}
    o.Builtins = {}
    o.Enumerations = {}
    o.Classes = {}
    o.Components = {}
    o.Modules = {}
    o.NativeClasses = {}
    o.NativeModules = {}
    o.Text = startingText
    self.__index = self
    return o
end

function Generator:LoadNativeData()
    self.NativeClasses = {}
    self.NativeModules = {}

    local status, res = xpcall(function () 
        local libsJson = Ext.Utils.Include(nil, 'builtin://Libs/IdeHelpersNativeData.lua')
        local libs = Ext.Json.Parse(libsJson)

        return libs
    end, debug.traceback)

    if status == true then
        self.NativeClasses = res.classes
    
        for name,mod in pairs(res.modules) do
            self.NativeModules[name] = mod
        end
    else
        Ext.Log.PrintWarning("Unable to load native class data; IDE helpers will not include annotations from C++ code")
        Ext.Log.PrintWarning(res)
    end
end

local _ModuleToClassField = {
    ["Module_Stats.DeltaMod"] = "Module_Stats",
    ["Module_Stats.EquipmentSet"] = "Module_Stats",
    ["Module_Stats.ItemColor"] = "Module_Stats",
    ["Module_Stats.ItemCombo"] = "Module_Stats",
    ["Module_Stats.ItemComboPreview"] = "Module_Stats",
    ["Module_Stats.ItemComboProperty"] = "Module_Stats",
    ["Module_Stats.ItemGroup"] = "Module_Stats",
    ["Module_Stats.ItemSet"] = "Module_Stats",
    ["Module_Stats.NameGroup"] = "Module_Stats",
    ["Module_Stats.SkillSet"] = "Module_Stats",
    ["Module_Stats.TreasureCategory"] = "Module_Stats",
    ["Module_Stats.TreasureTable"] = "Module_Stats",
    ["Module_Stats.Math"] = "Module_Stats",
    ["Module_ServerSurface.Action"] = "Module_ServerSurfaceAction",
}

--- @param opts GenerateIdeHelpersOptions
function Generator:GenerateExtraData(opts)
    self:EmitLine("--#region ExtraData")
    self:EmitEmptyLine()

    local baseKeys = {
        "NPC max combat turn time", "ThrowDistanceMin", "ThrowDistanceMax", "ThrowStrengthCapMultiplier", "ThrowWeightMultiplier", "Telekinesis Range", "End Of Combat SightRange Multiplier", "Sneak Damage Multiplier", "Infectious Disease Depth", "Infectious Disease Radius", "Haste Speed Modifier", "Slow Speed Modifier", "Ally Joins Ally SightRange Multiplier", "Surface Distance Evaluation", "Once Per Combat Spell Realtime Cooldown", "HintDuration", "Projectile Terrain Offset", "Surface Clear Owner Time", "Decaying Touch Damage Modifier", "FirstItemTypeShift", "SecondItemTypeShift", "PickpocketGoldValuePerPoint", "PickpocketWeightPerPoint", "PickpocketExperienceLevelsPerPoint", "PersuasionAttitudeBonusPerPoint", "AbilityBaseValue", "AbilityCharCreationBonus", "AbilityLevelGrowth", "AbilityBoostGrowth", "AbilityGrowthDamp", "AbilitySoftCap", "WitsGrowthDamp", "VitalityStartingAmount", "VitalityExponentialGrowth", "VitalityLinearGrowth", "VitalityToDamageRatio", "VitalityToDamageRatioGrowth", "ExpectedDamageBoostFromAbilityPerLevel", "ExpectedDamageBoostFromSpellSchoolPerLevel", "ExpectedDamageBoostFromWeaponSkillPerLevel", "ExpectedConGrowthForArmorCalculation", "FirstVitalityLeapLevel", "FirstVitalityLeapGrowth", "SecondVitalityLeapLevel", "SecondVitalityLeapGrowth", "ThirdVitalityLeapLevel", "ThirdVitalityLeapGrowth", "FourthVitalityLeapLevel", "FourthVitalityLeapGrowth", 
        "DamageBoostFromAbility", "MonsterDamageBoostPerLevel", "PhysicalArmourBoostFromAbility", "MagicArmourBoostFromAbility", "VitalityBoostFromAbility", "DodgingBoostFromAbility", "HealToDamageRatio", "ArmorToVitalityRatio", "ArmorRegenTimer", "ArmorRegenConstGrowth", "ArmorRegenPercentageGrowth", "ArmorAfterHitCooldown", "MagicArmorRegenTimer", "MagicArmorRegenConstGrowth", "MagicArmorRegenPercentageGrowth", "MagicArmorAfterHitCooldown", "ArmorHeadPercentage", "ArmorUpperBodyPercentage", "ArmorLowerBodyPercentage", "ArmorShieldPercentage", "ArmorHandsPercentage", "ArmorFeetPercentage", "ArmorAmuletPercentage", "ArmorRingPercentage", "CharacterBaseMemoryCapacity", "CharacterBaseMemoryCapacityGrowth", "CharacterAbilityPointsPerMemoryCapacity", "CombatSkillCap", "CombatSkillLevelGrowth", "CombatSkillNpcGrowth", "CombatSkillDamageBonus", "CombatSkillCritBonus", "CombatSkillCritMultiplierBonus", "CombatSkillAccuracyBonus", "CombatSkillDodgingBonus", "CombatSkillReflectionBonus", "NumStartingCivilSkillPoints", "CivilSkillCap", "CivilSkillLevelGrowth", "CivilPointOffset", "SavethrowLowChance", "SavethrowHighChance", "SavethrowBelowLowPenalty", "SavethrowPenaltyCap", "CriticalBonusFromWits", 
        "InitiativeBonusFromWits", "WeaponAccuracyPenaltyPerLevel", "WeaponAccuracyPenaltyCap", "ShieldAPCost", "WeaponWeightLight", "WeaponWeightMedium", "WeaponWeightHeavy", "WeaponWeightGiant", "HighGroundThreshold", "LowGroundAttackRollPenalty", "HighGroundAttackRollBonus", "LowGroundMeleeRange", "HighGroundMeleeRange", "HighGroundRangeMultiplier", "SneakDefaultAPCost", "BlindRangePenalty", "RangeBoostedGlobalCap", "SurfaceDurationFromHitFloorReaction", "SurfaceDurationFireIgniteOverride", "SurfaceDurationFromCharacterBleeding", "SurfaceDurationAfterDecay", "SmokeDurationAfterDecay", "DualWieldingAPPenalty", "DualWieldingDamagePenalty", "ChanceToSetStatusOnContact", "SkillPhysArmorBonusBase", "SkillPhysArmorBonusPerPoint", "SkillPhysArmorBonusMax", "SkillMagicArmorBonusBase", "SkillMagicArmorBonusPerPoint", "SkillMagicArmorBonusMax", "SkillVitalityBonusBase", "SkillVitalityBonusPerPoint", "SkillVitalityBonusMax", "SpellSchoolDamageToPhysicalArmorPerPoint", "SpellSchoolDamageToMagicArmorPerPoint", "SpellSchoolArmorRestoredPerPoint", "SpellSchoolVitalityRestoredPerPoint", "SpellSchoolHighGroundBonusPerPoint", "SpellSchoolFireDamageBoostPerPoint", "SpellSchoolPoisonAndEarthDamageBoostPerPoint", "SpellSchoolAirDamageBoostPerPoint", 
        "SpellSchoolWaterDamageBoostPerPoint", "SpellSchoolPhysicalDamageBoostPerPoint", "SpellSchoolSulfuricDamageBoostPerPoint", "SpellSchoolLifeStealPerPoint", "SpiritVisionFallbackRadius", "AiCoverProjectileTurnMemory", "CarryWeightBase", "CarryWeightPerStr", "DeflectProjectileRange", "CleaveRangeOverride", "FleeDistance", "GlobalGoldValueMultiplier", "PriceBarterCoefficient", "PriceAttitudeCoefficient", "PriceRoundToFiveAfterAmount", "PriceRoundToTenAfterAmount", "GMCharacterAbilityCap", "GMCharacterArmorCap", "GMCharacterResistanceMin", "GMCharacterResistanceMax", "GMCharacterAPCap", "GMCharacterSPCap", "GMItemLevelCap", "GMItemAbilityCap", "GMItemArmorMin", "GMItemArmorMax", "GMItemResistanceMin", "GMItemResistanceMax", "TraderDroppedItemsPercentage", "TraderDroppedItemsCap", "TraderDonationsRequiredAttitude", "TeleportUnchainDistance", "FlankingElevationThreshold", "DefaultDC", "FallDamageBaseDamage", "FallDamageMinimumDistance", "FallDamageMaximumDistance", "FallDamageDamageType", "FallDamagePronePercent", "FallDamageMultiplierHugeGargantuan", "FallImpactPushDistance", "FallImpactConstant", "FallImpactMinWeight", "LethalHP", "SpinningSpeed", "ItemWeightLight", "ItemWeightMedium", "ItemWeightHeavy", 
        "WisdomTierHigh", "CoinPileMediumThreshold", "CoinPileBigThreshold", "DarknessHeightOffset", "DarknessGracePeriod", "DarknessGraceFrames", "SightConePreviewMaxDistanceFromPlayerSq", "SpellMoveFloodMaxSourceDistance", "UnarmedRange", "ActiveRollRerollInspirationCost", "PickpocketWeightSquaredThreshold", "PickpocketPriceSquaredThreshold", "JoinAllyInCombatRadius", "AbilityMaxValue", "PassiveRollDelay", "PickpocketingMaxPrice", "PickpocketingWeightModifierConstant", "PickpocketingPriceModifierConstant", "PickpocketingPriceDC", "PickpocketingMinimumDC", "FollowDistance", "SneakFollowDistance", "SeekHiddenRange", "SeekHiddenTimeout", "DamageTerrainOffset", "SurfaceOnMoveDistanceMultiplier", "SurfaceOnEnterDistanceMultiplier", "GameplayLightsFadeTime", "MaxPickingDistance", "LearnSpellCostPerLevel", "SavantLearnSpellCostPerLevel", "MaxShortRestPoints", "DualWieldingPlayersDefaultOn", "DualWieldingNPCsDefaultOn", "MaximumXPCap", "CombatCameraEndDelay", "DamagingSurfacesThreshold", "FollowThroughDamagingSurfaceDistance", "CameraTakeControlTimer", "CameraEnableTakeControlOnEndTurn", "CombatCameraRangedAttackWait", "CCTurnPauseTime", "BaseSpellDifficultyCheck", "GlobalBaseACModifier", "RollStreamDialogDebtRange", 
        "RollStreamPlayerSpellDebtRange", "RollStreamNPCSpellDebtRange", "RollStreamPlayerRandomCastDebtRange", "RollStreamNPCRandomCastDebtRange", "RollStreamSuccessDebtConsumeMultiplier", "RollStreamFailDebtConsumeMultiplier", "ContainerCloseDistance", "CharacterInteractionHeightMin", "MaxPickUpMultiplier", "PointAndClickPostDelay", "PortraitClickSpamThreshold", "CombatCameraStatusEndTurnWait", "Disarm_MaxDistance", "Disarm_ThrowAngle", "Disarm_RandomConeAngle", "EncumberedMultiplier", "HeavilyEncumberedMultiplier", "CarryLimitMultiplier", "ShoveCurveConstant", "ShoveDistanceMax", "ShoveDistanceMin", "ShoveMultiplier", "DragCheckMultiplier", "DragCurveConstant", "DragDistanceMax", "ForceFunctorFallbackStrength", "ExhaustedRest_HealthPercent", "ExhaustedRest_ResourcePercent", "ActiveRollPartyNearbyDistance", "ActiveRollDisableIgnoreRange", "CannotActTimeout", "AutoSuccessCastingInActiveRoll", "SplatterDistanceThreshold", "SplatterDirtPerDistance", "SplatterBloodPerDistance", "SplatterBloodPerAttack", "SplatterMaxBloodLimit", "SplatterMaxDirtLimit", "SplatterSweatDelta", "PickupObjectMultiplier", "MoveObjectMultiplier", "MoveObjectRangeCheckMultiplier", "MoveObjectRangeCurveConstant", "MoveObjectRangeDistanceMax", 
        "ReactTimeThreshold", "AISwarmMoveGroupMinDistance", "ScaleChangeSpeed", "SizeChangeWeightMultiplier", "PickpocketInteractionRange", "DangerousLevelGap", "FallMinDamagePathfindingCost", "FallMaxDamagePathfindingCost", "FallDeadPathfindingCost", "PerformerZoneJoinRadius", "PerformerZoneSilenceRadius", "MaxPerformerZones", "PerformerZoneMaxPlayersByType", "FootstepMixdownTime", "FootstepMixWaitTime", "AmbienceMixDownDelayTime", "DefaultInstrumentStowedVisuals", "HoverStateMixLeaveDelayTime", "HoverStateMixEnterDelayTime", "Minimum SightRange When Calculating End Of Combat Range", "InterruptZoneRange", "CameraFlyingZoomDistanceExtra", "CameraFlyingZoomDistanceRemap", "InterruptNearbyDistance", "PingCooldownTime", "SoundFootstepGroupMinSize", "SoundFootstepGroupMaxRadius", "SoundFootstepGroupBoundRadius", "PingMarkerLifeTime", "EscortStragglerDistance", "DefaultUnifiedTutorialsLifeTime", "ForceFunctorConeAngle", "SplitScreenMaxSoundListenerDistance", "TutorialHotbarSlotRemovedTimeout", "CamListener_DistPercentFromCam_Close", "CamListener_DistPercentFromCam_Medium", "CamListener_DistPercentFromCam_Far", "CamListener_ZoomOut_Close", "CamListener_ZoomOut_Medium", "CamListener_ZoomOut_Far", "InitiativeDie", "PassiveSkillScoreAdvMod", 
        "TransferredEvidenceOnGroundRadius", "PointLightVerticalLimit", "CombatCameraDeathAnimationWait", "MoveToTargetCloseEnoughMin", "MoveToTargetCloseEnoughMax"
    }
    
    local extraData = Ext.Stats.GetStatsManager().ExtraData
    if opts.UseBaseExtraData then
        self:EmitComment("@class BaseExtraData")

        local keysWithWhitespace = {}

        for _,k in ipairs(baseKeys) do
            if string.find(k, "%s") then
                keysWithWhitespace[#keysWithWhitespace+1] = k
            else
                self:EmitFieldComment(k .. " number")
            end
        end
        if #keysWithWhitespace > 0 then
            self:EmitLine("local BaseExtraData = {")
            for _,k in ipairs(keysWithWhitespace) do
                self:EmitLine("\t[\"" .. k .. "\"] = " .. tostring(extraData[k] or 0) .. ",")
            end
            self:EmitLine("}")
        else
            self:EmitLine("local BaseExtraData = {}")
        end
        self:EmitEmptyLine()
        self:EmitEmptyLine()
        self:EmitComment("@class ExtraData:BaseExtraData")
        self:EmitLine("Ext.ExtraData = {}")
    else
        local baseKeyDict = {}
        for _,v in pairs(baseKeys) do
            baseKeyDict[v] = true
        end
        local newKeys = {}

        for k,v in pairs(extraData) do
            if not baseKeyDict[k] then
                newKeys[#newKeys+1] = k
            end
        end
        table.sort(newKeys)

        if not opts.GenerateExtraDataAsClass then
            self:EmitLine("Ext.ExtraData = {")
            for _,k in ipairs(baseKeys) do
                self:EmitLine(string.format("\t[\"%s\"] = %s,", k, extraData[k]))
            end
    
    
            for _,k in ipairs(newKeys) do
                self:EmitLine(string.format("\t[\"%s\"] = %s,", k, extraData[k]))
            end
    
            self:EmitLine("}")
        else
            self:EmitComment("@class BaseExtraData")

            local keysWithWhitespace = {}

            for _,k in ipairs(baseKeys) do
                if string.find(k, "%s") then
                    keysWithWhitespace[#keysWithWhitespace+1] = k
                else
                    self:EmitFieldComment(k .. " number")
                end
            end
            if #keysWithWhitespace > 0 then
                self:EmitLine("local BaseExtraData = {")
                for _,k in ipairs(keysWithWhitespace) do
                    self:EmitLine("\t[\"" .. k .. "\"] = " .. tostring(extraData[k] or 0) .. ",")
                end
                self:EmitLine("}")
            else
                self:EmitLine("local BaseExtraData = {}")
            end

            self:EmitEmptyLine()
            self:EmitEmptyLine()

            keysWithWhitespace = {}

            self:EmitComment("@class ExtraData:BaseExtraData")
            for _,k in ipairs(newKeys) do
                if string.find(k, "%s") then
                    keysWithWhitespace[#keysWithWhitespace+1] = k
                else
                    self:EmitFieldComment(k .. " number")
                end
            end
            if #keysWithWhitespace > 0 then
                self:EmitLine("Ext.ExtraData = {")
                for _,k in ipairs(keysWithWhitespace) do
                    self:EmitLine("\t[\"" .. k .. "\"] = " .. tostring(extraData[k] or 0) .. ",")
                end
                self:EmitLine("}")
            else
                self:EmitLine("Ext.ExtraData = {}")
            end
            self:EmitEmptyLine()
        end

    end
    self:EmitEmptyLine()
    self:EmitLine("--#endregion")
    self:EmitEmptyLine()
end

--- @param opts GenerateIdeHelpersOptions
function Generator:Build(opts)
    if opts.OsiDynamics then
        self.Text = self.Text .. dynamicTypesText
    end

    local types = Ext.Types.GetAllTypes()
    local sortedTypes = {}

    for i,typeName in ipairs(types) do
        table.insert(sortedTypes, typeName)
    end

    table.sort(sortedTypes)

    local moduleToClassField = {}
    local removeModuleFromExt = {}

    for i,typeName in ipairs(sortedTypes) do
        local type = Ext.Types.GetTypeInfo(typeName)
        if typeName == "EntityHandle" then
        -- EntityHandle generation handled separately
        elseif type.Kind == "Object" then
            if type.ComponentName ~= "" then
                table.insert(self.Components, type)
            end

            table.insert(self.Classes, type)
        elseif type.Kind == "Module" then
            local index = #self.Modules+1
            if _ModuleToClassField[typeName] then
                local targetTypeName = _ModuleToClassField[typeName]
                if moduleToClassField[targetTypeName] == nil then
                    moduleToClassField[targetTypeName] = {}
                end
                removeModuleFromExt[typeName] = index
                table.insert(moduleToClassField[targetTypeName], type)
            end
            self.Modules[index] = type
        elseif type.Kind == "Enumeration" then
            table.insert(self.Enumerations, type)
        elseif type.Kind == "Boolean" or type.Kind == "Integer" or type.Kind == "Float" or type.Kind == "String" or type.Kind == "Any" then
            table.insert(self.Intrinsics, type)
        elseif typeName == "vec2" or typeName == "vec3" or typeName == "vec4" or typeName == "ivec2" or typeName == "ivec3" or typeName == "mat3" or typeName == "mat3x4" or typeName == "mat4x3" or typeName == "mat4" or typeName == "Version" then
            table.insert(self.Builtins, type)
        end
    end

    for i,type in ipairs(self.Intrinsics) do
        self:EmitIntrinsicType(type)
    end

    for i,type in ipairs(self.Builtins) do
        self:EmitBuiltinType(type)
    end

    self:EmitEmptyLine()
    self:EmitEmptyLine()

    self:EmitEntityClass()

    self:EmitEmptyLine()
    self:EmitEmptyLine()

    if opts.AddAliasEnums then
        for i,type in ipairs(self.Enumerations) do
            self:EmitEnumeration(type)
        end
    end

    self:EmitEmptyLine()
    self:EmitEmptyLine()

    for i,type in ipairs(self.Classes) do
        self:EmitClass(type)
        self:EmitEmptyLine()
        self:EmitEmptyLine()
    end

    self:EmitEmptyLine()
    self:EmitEmptyLine()

    local modifierLists = Ext.Stats.GetStatsManager().ModifierLists.Values
    for _,modifierList in ipairs(modifierLists) do
        self:EmitModifierList(modifierList)
        self:EmitEmptyLine()
        self:EmitEmptyLine()
    end

    self:EmitEmptyLine()
    self:EmitEmptyLine()

    for i,type in ipairs(self.Modules) do
        self:EmitModule(type, moduleToClassField[type.TypeName])
        self:EmitEmptyLine()
        self:EmitEmptyLine()
    end

    local modules = {}
    for i,v in pairs(self.Modules) do
        if not removeModuleFromExt[v.TypeName] then
            modules[#modules+1] = v
        end
    end

    self.Modules = modules
    
    self:EmitExt("Client")
    self:EmitEmptyLine()
    self:EmitExt("Server")
    self:EmitEmptyLine()
    self:EmitExt(nil, true)

    self:GenerateExtraData(opts)
end

function Generator:MakeTypeName(type)
    -- Replace "_" with capitalization (mainly used for stats class names)
    type = string.gsub(type, "_[%a%d]", function (ns)
        return string.upper(string.sub(ns, 2))
    end)
    -- Replace namespace "::" with capitalization
    type = string.gsub(type, "[%a%d]+::", function (ns)
        return string.upper(string.sub(ns, 1, 1)) .. string.sub(ns, 2, -3)
    end)
    -- Replace template classes "T<V>" with underscore ("T_V")
    type = string.gsub(type, "<[%a%d]+>", function (n)
        return "_" .. string.sub(n, 2, -2)
    end)
    if _TypeReplace[type] then
        return _TypeReplace[type]
    end
    return type
end

local function _TableIsNullOrEmpty(tbl)
    return tbl == nil or tbl[1] == nil
end

function Generator:MakeTypeSignature(cls, type, forceExpand, nativeDefn, overrides, missingFuncData)
    if _TypeReplace[type.TypeName] then
        return _TypeReplace[type.TypeName]
    end

    if overrides and overrides.ReplaceSignature then
        return overrides.ReplaceSignature
    end

    if type.IsBuiltin and forceExpand ~= true then
        return type.TypeName
    elseif type.Kind == "Any" then
        return "any"
    elseif type.Kind == "Nullable" then
        return self:MakeTypeSignature(cls, type.ParentType) .. "?"
    elseif type.Kind == "Array" then
        if type.ElementType.Kind == "Array" or type.ElementType.Kind == "Map" then
            return self:MakeTypeSignature(nil, type.ElementType) .. "[]"
        else
            return self:MakeTypeName(type.ElementType.TypeName) .. "[]"
        end
    elseif type.Kind == "Map" then
        return "table<" .. self:MakeTypeName(type.KeyType.TypeName) .. ", " .. self:MakeTypeSignature(nil, type.ElementType) .. ">"
    elseif type.Kind == "Function" then
        local args = {}
        local retval = {}

        local clsName = ""
        if cls ~= nil then
            clsName = self:MakeTypeSignature(nil, cls)
            table.insert(args, "self:" .. clsName)
        end

        missingFuncData = missingFuncData or {}
        if _FuncData.Regular[clsName] and _FuncData.Regular[clsName][type.TypeName] then
            missingFuncData = _FuncData.Regular[clsName][type.TypeName]
        end
        
        if _TableIsNullOrEmpty(type.Params) and missingFuncData.Params then
            for i,data in ipairs(missingFuncData.Params) do
                table.insert(args, data.name .. ":" .. data.arg)
            end
        else
            for i,arg in ipairs(type.Params) do
                if nativeDefn ~= nil then
                    table.insert(args, nativeDefn.params[i].name .. ":" .. self:MakeTypeSignature(cls, arg))
                else
                    table.insert(args, "a" .. i .. ":" .. self:MakeTypeSignature(cls, arg))
                end
            end
        end

        if overrides and overrides.ReplaceReturnValue then
            table.insert(retval, overrides.ReplaceReturnValue)
        elseif not _TableIsNullOrEmpty(missingFuncData.Return) then
            for i,arg in ipairs(missingFuncData.Return) do
                table.insert(retval, arg)
            end
        else
            for i,arg in ipairs(type.ReturnValues) do
                table.insert(retval, self:MakeTypeSignature(cls, arg))
            end
        end

        local fun = "fun(" .. table.concat(args, ", ") .. ")"
        if #retval > 0 then
            fun = fun .. ":" .. table.concat(retval, ", ")
        end

        return fun
    else
        return self:MakeTypeName(type.TypeName)
    end
end

function Generator:EmitEmptyLine()
    self.Text = self.Text .. NEWLINE
end

function Generator:EmitLine(text, addNewLine)
    self.Text = self.Text .. text .. NEWLINE
    if addNewLine then
        self.Text = self.Text .. NEWLINE
    end
end

function Generator:EmitComment(text)
    self.Text = self.Text .. "--- " .. text .. NEWLINE
end

function Generator:EmitFieldComment(text)
    self.Text = self.Text .. "--- @field " .. text .. NEWLINE
end

function Generator:EmitMultiLineComment(text)
    for line in text:gmatch("([^\n]+)") do
       self:EmitComment(self.RTrim(line))
    end
end

function Generator:EmitAlias(name, definition)
    if not _TypeAliasRemove[name] then
        local def = _TypeAliasReplace[name] or definition
        self:EmitComment("@alias " .. self:MakeTypeName(name) .. " " .. def)
    end
end

function Generator:EmitIntrinsicType(type)
    self:EmitAlias(type.TypeName, self.ValueKindToLua[type.Kind])
end

function Generator:EmitBuiltinType(type)
    self:EmitAlias(type.TypeName, self:MakeTypeSignature(nil, type, true))
end

function Generator:EmitEnumeration(type)
    local decl = "string"

    local orderedKeys = {}
    for key,value in pairs(type.EnumValues) do
        table.insert(orderedKeys, key)
    end

    table.sort(orderedKeys)
    
    for _,key in pairs(orderedKeys) do
        decl = _format("%s|\"%s\"", decl, key)
    end
    self:EmitAlias(type.TypeName, decl)
end

function Generator:EmitEntityClass()
    self:EmitLine(_CustomEntries.Entity)

    for _,type in ipairs(self.Components) do
        self:EmitFieldComment(type.ComponentName .. " " .. self:MakeTypeName(type.TypeName) .. "?")
    end
end

function Generator:FindNativeClass(type)
    if type.NativeName ~= nil then
        local name = string.gsub(type.NativeName, "struct ", '')
        name = string.gsub(name, "class ", '')
        return self.NativeClasses[name]
    end

    return nil
end

function Generator:FindNativeMethod(fname, nativeCls)
    local nativeMethod = nil
    if nativeCls ~= nil then
        nativeMethod = nativeCls.methods[fname]
        if nativeMethod == nil then
            nativeMethod = nativeCls.methods["Lua" .. fname]
        end
    end

    return nativeMethod
end

function Generator:FindNativeFunction(fname, nativeNs)
    local nativeMethod = nil
    if nativeNs ~= nil then
        nativeMethod = nativeNs.functions[fname]
    end

    return nativeMethod
end

function Generator:MethodNeedsFullSignature(nativeMethod)
    if nativeMethod == nil then
        return false
    end

    if #self.Trim(nativeMethod.description) > 0 then
        return true
    end

    for i,fun in pairs(nativeMethod.params) do
        if #self.Trim(nativeMethod.description) > 0 then
            return true
        end
    end

    return false
end

function Generator:EmitMethod(type, fname, nativeDefn, overrides)
    local nativeMethod = self:FindNativeMethod(fname, nativeDefn)

    if self:MethodNeedsFullSignature(nativeMethod) then
        self:EmitFullMethodSignature(type, fname, type.Methods[fname], nativeMethod, overrides)
    else
        local clsName = self:MakeTypeSignature(nil, type)
        local fieldOverride = _FuncData.Field[clsName] and _FuncData.Field[clsName][fname]
        if fieldOverride then
            self:EmitFieldComment(fname .. " " .. fieldOverride)
        else
            self:EmitFieldComment(fname .. " " .. self:MakeTypeSignature(type, type.Methods[fname],  false, nativeMethod, overrides))
        end
    end
end

function Generator:EmitModuleFunction(type, fname, nativeDefn, overrides)
    local nativeFunc = self:FindNativeFunction(fname, nativeDefn)

    if nativeFunc == nil then
        local typeSig = ""
        local clsName = self:MakeModuleTypeName(type)
        if _FuncData.Field[clsName] and _FuncData.Field[clsName][fname] then
            typeSig = _FuncData.Field[clsName][fname]
        else
            local missingFuncData = _FuncData.Regular[clsName] and _FuncData.Regular[clsName][fname] or nil
            typeSig = self:MakeTypeSignature(nil, type.Methods[fname], nil, nil, overrides, missingFuncData)
        end
        self:EmitFieldComment(fname .. " " .. typeSig)
    else
        self:EmitFullMethodSignature(type, fname, type.Methods[fname], nativeFunc, overrides)
    end
end

function Generator:EmitFullMethodSignature(cls, funcName, fun, nativeMethod, overrides)
    local argDescs = {}
    local args = {}
    local overloads = {}

    local clsName
    if cls.Kind == "Module" then
        clsName = self:MakeModuleTypeName(cls)
    else
        clsName = self:MakeTypeName(cls.TypeName)
    end

    local helpersClsName = clsName:gsub("%.", "")

    local missingFuncData = {}
    if _FuncData.Regular[clsName] and _FuncData.Regular[clsName][funcName] then
        missingFuncData = _FuncData.Regular[clsName][funcName]
    end

    if missingFuncData.Params and (_TableIsNullOrEmpty(fun.Params) or missingFuncData.Override) then
        if missingFuncData.Overload then
            overloads = missingFuncData.Overload
        end
        for i,data in ipairs(missingFuncData.Params) do
            table.insert(argDescs, "--- @param " .. data.name .. " " .. data.arg or any .. " " ..  self.Trim(data.description or ""))
            table.insert(args, data.name)
        end
    else
        for i,arg in ipairs(fun.Params) do
            table.insert(argDescs, "--- @param " .. nativeMethod.params[i].name .. " " .. self:MakeTypeSignature(cls, arg) .. " " ..  self.Trim(nativeMethod.params[i].description))
            table.insert(args, nativeMethod.params[i].name)
        end
    end

    if not _TableIsNullOrEmpty(missingFuncData.Return) then
        for i,arg in ipairs(missingFuncData.Return) do
            table.insert(argDescs, "--- @return " .. arg)
        end
    else
        for i,arg in ipairs(fun.ReturnValues) do
            table.insert(argDescs, "--- @return " .. self:MakeTypeSignature(cls, arg))
        end
    end

    local fun = "function " .. helpersClsName

    if cls.Kind ~= "Module" then
        fun = fun .. ":"
    else
        fun = fun .. "."
    end

    fun = fun .. funcName .. "(" .. table.concat(args, ", ") .. ") end"
    local desc = table.concat(argDescs, NEWLINE)

    if missingFuncData.Description then
        desc = missingFuncData.Description .. NEWLINE .. desc
    end

    local overloadsLen = #overloads
    if overloadsLen > 0 then
        local text = ""
        for i=1,overloadsLen do
            text = text .. overloads[i]
        end

        -- Put overloads above the params/return annotations
        text = text .. "\n" .. desc
        desc = text
    end

    local funcDesc = self.Trim(nativeMethod.description)
    if nativeMethod.implementation_file ~= nil and #funcDesc > 0 then
        funcDesc = funcDesc .. NEWLINE .. "Location: " .. nativeMethod.implementation_file .. ":" .. nativeMethod.implementation_line
    end

    if #funcDesc > 0 then
        self:EmitMultiLineComment(funcDesc)
    end

    if desc ~= "" then
        self.Text = self.Text .. desc .. NEWLINE .. fun
    else
        self.Text = self.Text .. fun
    end
    if overrides and overrides.After then
        self.Text = self.Text .. NEWLINE .. overrides.After
    end
    self.Text = self.Text .. NEWLINE .. NEWLINE
end

local serverEventParamsPattern = "esv::lua::(%a+)Event"
local clientEventParamsPattern = "ecl::lua::(%a+)Event"
local bothContextEventParamsPattern = "(%a+)Event"

---@type table<integer, {Type:string, Event:string, Context:string|"server"|"client"|"any"}>
local eventTypeGenerationData = {
}

---Event name to index in eventTypeGenerationData
---@type table<string, integer>
local eventTypeGenerationDataIndex = {}

function Generator:EmitClassEventData(type)
    if type.ParentType == nil or type.ParentType.TypeName ~= "lua::EventBase" then
        return
    end

    local name = self:MakeTypeName(type.TypeName)
    local context = "any"
    local _,_,eventName = string.find(name, serverEventParamsPattern)
    if not eventName then
        _,_,eventName = string.find(name, clientEventParamsPattern)
        if eventName then
            context = "client"
        else
            _,_,eventName = string.find(name, bothContextEventParamsPattern)
        end
    else
        context = "server"
    end
    if eventName then
        eventName = eventName:gsub("^Lua", "")
        local lastIndex = eventTypeGenerationDataIndex[eventName]
        if lastIndex == nil then
            lastIndex = #eventTypeGenerationData+1
        else
            local lastData = eventTypeGenerationData[lastIndex]
            name = lastData.Type .. "|" .. name
            context = "any"
        end
        eventTypeGenerationData[lastIndex] = {Type = name, Event = eventName, Context = context}
        eventTypeGenerationDataIndex[eventName] = lastIndex
    end
end

function Generator:EmitModifier(attribute, modifier)
    local typename = modifier.Name
    if typename == "FixedString" or typename == "Guid" or typename == "TranslatedString" then
        typename = "string"
    elseif typename == "ConstantInt" then
        typename = "integer"
    elseif typename == "ConstantFloat" then
        typename = "number"
    elseif typename == "TargetConditions" or typename == "UseConditions" then
        typename = "StatsConditions"
    elseif typename == "RollConditions" then
        typename = "StatsRollConditions"
    elseif typename == "Requirements" then
        typename = "StatsRequirements"
    end
    self:EmitFieldComment(attribute.Name .. " " .. typename)
end

function Generator:EmitModifierList(modifierList)
    local name = self:MakeTypeName(modifierList.Name) .. ":StatsObject"
    self:EmitComment("@class " .. name)

    local modifiers = Ext.Stats.GetStatsManager().ModifierValueLists
    for _,attr in ipairs(modifierList.Attributes.Values) do
        local modifier = modifiers.Values[attr.EnumerationIndex+1]
        self:EmitModifier(attr, modifier)
    end
end

function Generator:EmitClass(type)
    local name = self:MakeTypeName(type.TypeName)
    local nameWithParent = name
    
    if type.ParentType ~= nil then
        nameWithParent = nameWithParent .. ":" .. self:MakeTypeName(type.ParentType.TypeName)
    end

    local nativeDefn = self:FindNativeClass(type)

    local customText = _CustomTypeEntries[name]
    if customText and customText.Replace then
        self:EmitLine(customText.Replace)
        return
    end

    --TODO Hacky output fixes [_CustomTypeEntries.Before(EmitClass)]. May not be required later on.
    if customText and customText.Before then
        self:EmitLine(customText.Before)
    end

    self:EmitComment("@class " .. nameWithParent)

    local sortedMembers = {}
    for fname,ftype in pairs(type.Members) do table.insert(sortedMembers, fname) end
    table.sort(sortedMembers)

    for i,fname in ipairs(sortedMembers) do
        local typeSig = self:MakeTypeSignature(type, type.Members[fname])
        local fieldOverride = _FuncData.Field[name] and _FuncData.Field[name][fname]
        if _Aliases[name] then
            if _Aliases[name][fname] then
                typeSig = _Aliases[name][fname]
            end
        elseif fieldOverride then
            typeSig = fieldOverride
        end
        self:EmitFieldComment(fname .. " " .. typeSig)
    end

    if customText and customText.Fields then
        for i,v in ipairs(customText.Fields) do
            self:EmitFieldComment(v)
        end
    end

    local sortedMethods = {}
    for fname,ftype in pairs(type.Methods) do table.insert(sortedMethods, fname) end
    table.sort(sortedMethods)

    local basicMethodSigs = {}
    local extendedMethodSigs = {}

    for i,fname in ipairs(sortedMethods) do
        local nativeMethod = self:FindNativeMethod(fname, nativeDefn)
        if self:MethodNeedsFullSignature(nativeMethod) then
            table.insert(extendedMethodSigs, fname)
        else
            table.insert(basicMethodSigs, fname)
        end
    end
    
    for i,fname in ipairs(basicMethodSigs) do
        local overrides = _CustomFunctionExtras[name.."."..fname]
        self:EmitMethod(type, fname, nativeDefn, overrides)
    end

    if #extendedMethodSigs > 0 then
        self:EmitLine('local ' .. name .. ' = {}')
        self:EmitLine("")
        
        for i,fname in ipairs(extendedMethodSigs) do
            local overrides = _CustomFunctionExtras[name.."."..fname]
            self:EmitMethod(type, fname, nativeDefn, overrides)
        end
    end

    self:EmitClassEventData(type)

     --TODO Hacky output fixes [_CustomTypeEntries.After(EmitClass)]. May not be required later on.
    if customText and customText.After then
        self:EmitLine(customText.After)
    end
end


function Generator:MakeModuleTypeName(type)
    local name = type.NativeName:gsub("%.", "")
    if type.ModuleRole ~= "Both" then
        name = type.ModuleRole .. name
    end

    return "Ext_" .. name
end

---@param moduleToClassField table?
function Generator:EmitModule(type, moduleToClassField)
    local helpersModuleName = self:MakeModuleTypeName(type)
    local nativeModuleName = type.NativeName
    if type.ModuleRole ~= "Both" then
        nativeModuleName = type.ModuleRole .. nativeModuleName
    end

    --TODO Hacky output fixes [_CustomTypeEntries.Before(EmitModule)]. May not be required later on.
    local customText = _CustomTypeEntries[helpersModuleName]
    if customText and customText.Before then
        self:EmitLine(customText.Before)
    end

    self:EmitComment("@class " .. helpersModuleName)
    local nativeDefn = self.NativeModules[nativeModuleName]

    if moduleToClassField then
        table.sort(moduleToClassField, function(a,b) return a.TypeName < b.TypeName end)
        for _,vType in pairs(moduleToClassField) do
            local vName = string.gsub(vType.NativeName, type.NativeName .. ".", "")
            local typeSig = self:MakeModuleTypeName(vType)
            local fieldOverride = _FuncData.Field[helpersModuleName] and _FuncData.Field[helpersModuleName][vName]
            if fieldOverride then
                typeSig = fieldOverride
            end
            self:EmitFieldComment(vName .. " " .. typeSig)
        end
    end

    if customText and customText.Fields then
        for i,v in ipairs(customText.Fields) do
            self:EmitFieldComment(v)
        end
    end

    local sortedFuncs = {}
    for fname,ftype in pairs(type.Methods) do table.insert(sortedFuncs, fname) end
    table.sort(sortedFuncs)

    local basicFuncSigs = {}
    local extendedFuncSigs = {}

    for i,fname in ipairs(sortedFuncs) do
        if self:FindNativeFunction(fname, nativeDefn) ~= nil then
            table.insert(extendedFuncSigs, fname)
        else
            table.insert(basicFuncSigs, fname)
        end
    end
    
    for i,fname in ipairs(basicFuncSigs) do
        local overrides = _CustomFunctionExtras[helpersModuleName.."."..fname]
        self:EmitModuleFunction(type, fname, nativeDefn, overrides)
    end

    self:EmitLine('local ' .. helpersModuleName .. ' = {}')
    --TODO Hacky output fixes [_CustomTypeEntries.After(EmitModule)]. May not be required later on.
    if customText and customText.After then
        self:EmitLine(customText.After)
    end
    self:EmitEmptyLine()
    
    for i,fname in ipairs(extendedFuncSigs) do
        --TODO Hacky output fixes [_CustomFunctionExtras(EmitModule)]. May not be required later on.
        local replaceText = nil
        local afterText = nil

        local overrides = _CustomFunctionExtras[helpersModuleName.."."..fname]
        if overrides then
            if overrides.Before then
                self:EmitLine(overrides.Before, true)
            end
            afterText = overrides.After
            replaceText = overrides.Replace
        end

        if replaceText then
            self:EmitLine(replaceText, true)
            if afterText then
                self:EmitLine(afterText, true)
            end
        else
            self:EmitModuleFunction(type, fname, nativeDefn, overrides)
        end
    end
end

local function GenerateSubscriptionEvents(self)
    for _,k in pairs(Ext._Internal._PublishedSharedEvents) do
        if not eventTypeGenerationDataIndex[k] then
            if _DEBUG then
                Ext.Log.PrintWarning("Found unregistered event, assuming empty event", k)
            end
            eventTypeGenerationData[#eventTypeGenerationData+1] = {Type="LuaEmptyEvent", Event = k, Context = "any"}
        end
    end
    table.sort(eventTypeGenerationData, function(a,b) return a.Event < b.Event end)
    for _,v in ipairs(eventTypeGenerationData) do
        if v.Context == "server" then
            self:EmitComment("🔨**Server-Only**🔨  ")
        elseif v.Context == "client" then
            self:EmitComment("🔧**Client-Only**🔧  ")
        else
            self:EmitComment("🔨🔧**Server/Client**🔧🔨  ")
        end
        self:EmitComment(_format("@type SubscribableEvent<%s>  ", v.Type))
        self:EmitLine(_format('Ext.Events.%s = {}', v.Event))
    end
end

local _restrictedKeys = {
    ["end"] = true,
}

function Generator:GenerateEnum(enumName)
    self:EmitEmptyLine()
    self:EmitComment("@enum " .. enumName)
    self:EmitLine("Ext_Enums." .. enumName .. " = {")

    local enum = Ext.Enums[enumName]
    local bitfield = Ext.Types.GetTypeInfo(enumName).IsBitfield

    local sortedKeys = {}
    for label,key in pairs(enum) do
        if type(label) ~= "number" then
            table.insert(sortedKeys, {key = bitfield and key.__Value or key.Value, label = label})
        end
    end
    table.sort(sortedKeys, function (a, b) 
        return a.key < b.key
    end)
        
    for _,val in ipairs(sortedKeys) do
        if string.find(val.label, "%s") then
            self:EmitLine(string.format("\t[\"%s\"] = %s,", val.label, val.key))
        else
            self:EmitLine(string.format("\t%s = %s,", val.label, val.key))
        end
    end
        
    for _,val in ipairs(sortedKeys) do
        self:EmitLine(string.format("\t[%s] = \"%s\",", val.key, val.label))
    end

    self:EmitLine("}")
end

function Generator:GenerateEnums()
    self:EmitLine("--#region Generated Enums")
    self:EmitEmptyLine()
    self:EmitComment("@class Ext_Enums")
    self:EmitLine("local Ext_Enums = {}", true)

    local enumNames = {}
    for k,enum in pairs(Ext.Enums) do
        enumNames[#enumNames+1] = tostring(k)
    end
    table.sort(enumNames)

    for _,enumName in ipairs(enumNames) do
        self:GenerateEnum(enumName)
    end
    self:EmitEmptyLine()
    self:EmitLine("--#endregion")
    self:EmitEmptyLine()
end

function Generator:GenerateSystems()
    self:EmitEmptyLine()
    self:EmitComment("@class Ext_System")
    
    local types = Ext.Types.GetAllTypes()
    for _,name in pairs(types) do
        local type = Ext.Types.GetTypeInfo(name)
        if type.SystemName ~= "" then
            self:EmitFieldComment(type.SystemName .. " " .. self:MakeTypeName(type.TypeName))
        end
    end
    
    self:EmitEmptyLine()
end

function Generator:EmitExt(role, declareGlobal)
    if declareGlobal then
        self:GenerateEnums()
        self:GenerateSystems()
    end

    self:EmitComment("@class Ext" .. (role or ""))

    local aliases = {}
    for i,mod in ipairs(self.Modules) do
        if role == nil or mod.ModuleRole == "Both" or mod.ModuleRole == role then
            local helpersModuleName = self:MakeModuleTypeName(mod)
            if aliases[mod.NativeName] ~= nil then
                aliases[mod.NativeName] = aliases[mod.NativeName] .. "|" .. helpersModuleName
            else
                aliases[mod.NativeName] = helpersModuleName
            end
        end
    end

    local emitted = {}
    for i,mod in ipairs(self.Modules) do
        if role == nil or mod.ModuleRole == "Both" or mod.ModuleRole == role then
            local helpersModuleName = self:MakeModuleTypeName(mod)
            if emitted[mod.NativeName] == nil then
                self:EmitFieldComment(mod.NativeName .. " " .. (aliases[mod.NativeName] or helpersModuleName))
                emitted[mod.NativeName] = true
            end
            if mod.ModuleRole ~= "Both" then
                self:EmitFieldComment(mod.ModuleRole .. mod.NativeName .. " " .. helpersModuleName)
            end
        end
    end

    if declareGlobal then
        self:EmitFieldComment("Enums Ext_Enums")
        self:EmitFieldComment("System Ext_System")

        self:EmitLine("Ext = {Events = {}}")
        self:EmitEmptyLine()
        self:EmitEmptyLine()
        if _CustomEntries.Specific then
            for k,v in pairs(_CustomEntries.Specific) do
                self:EmitLine(v)
                if k == "SubscribableEventType" then
                    self:EmitLine("--#region Extender Events")
                    GenerateSubscriptionEvents(self)
                    self:EmitLine("--#endregion")
                end
                self:EmitEmptyLine()
            end
            self:EmitEmptyLine()
        end
        if _CustomEntries.Misc then
            for _,v in ipairs(_CustomEntries.Misc) do
                self:EmitLine(v)
            end
        end
    end
end

--Ext.Types.GenerateIdeHelpers("ExtIdeHelpers_New.lua")
--Ext.Types.GenerateIdeHelpers("ExtIdeHelpers_New.lua", {UseBaseExtraData=true})

--- @class GenerateIdeHelpersOptions
local _DefaultOpts = {
    AddAliasEnums = true,
    UseBaseExtraData = false,
    GenerateExtraDataAsClass = false,
    OsiDynamics = true
}

--- @param outputPath? string
--- @param opts? GenerateIdeHelpersOptions
Ext.Types.GenerateIdeHelpers = function (outputPath, opts)
    local options = {}
    if type(opts) == "table" then
        options = opts
    end
    setmetatable(options, {__index = _DefaultOpts})
    eventTypeGenerationData = {}
    eventTypeGenerationDataIndex = {}
    local gen = Generator:New()
    gen:LoadNativeData()
    gen:Build(options)
    if outputPath then
        Ext.IO.SaveFile(outputPath, gen.Text)
    end

    return gen.Text
end
