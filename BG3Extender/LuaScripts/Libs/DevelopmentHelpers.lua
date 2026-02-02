Ext.RegisterConsoleCommand("se_dyntest", function ()
    _P(" --- STARTING TESTS --- ")

    local tests = {
        "TestCharacterEnumeration",
        "TestCharacterProperties",
        "TestCharacterTemplateProperties"
    }

    for i,test in ipairs(tests) do
        RunTest(test, _G[test])
    end

    _P(" --- FINISHING TESTS --- ")
end)

local function DebugValidate(obj, entity, counters)
    if obj ~= nil and not Ext.Types.Validate(obj) then
        local name = Ext.Types.GetObjectType(obj)
        _PE("Validation failed: " .. tostring(entity) .. ", component " .. name)
        if counters[name] == nil then
            counters[name] = 1
        else
            counters[name] = counters[name] + 1
        end
        return false
    else
        return true
    end
end

local function ValidateServerCharacter(component, entity, counters)
    DebugValidate(component.Template, entity, counters)

    DebugValidate(component.AiActionMachine, entity, counters)
    for i,layer in pairs(component.AiActionMachine.CachedActions) do
        DebugValidate(layer, entity, counters)
    end
    for i,layer in pairs(component.AiActionMachine.Layers) do
        DebugValidate(layer.State, entity, counters)
        DebugValidate(layer.SyncState, entity, counters)
    end

    DebugValidate(component.AiBehaviourMachine, entity, counters)
    for i,layer in pairs(component.AiBehaviourMachine.CachedStates) do
        DebugValidate(layer, entity, counters)
    end
    for i,layer in pairs(component.AiBehaviourMachine.Layers) do
        DebugValidate(layer, entity, counters)
    end

    DebugValidate(component.AiMovementMachine, entity, counters)
    for i,layer in pairs(component.AiMovementMachine.CachedStates) do
        DebugValidate(layer, entity, counters)
    end
    for i,layer in pairs(component.AiMovementMachine.Layers) do
        DebugValidate(layer, entity, counters)
    end

    DebugValidate(component.AiSteeringMachine, entity, counters)
    for i,layer in pairs(component.AiSteeringMachine.CachedStates) do
        DebugValidate(layer, entity, counters)
    end
    for i,layer in pairs(component.AiSteeringMachine.Layers) do
        DebugValidate(layer, entity, counters)
    end

    DebugValidate(component.OsirisController, entity, counters)
    for i,task in pairs(component.OsirisController.Tasks) do
        DebugValidate(task, entity, counters)
    end
                    
    DebugValidate(component.PlayerData, entity, counters)
    DebugValidate(component.StatusManager, entity, counters)
    for i,status in pairs(component.StatusManager.Statuses) do
        DebugValidate(status, entity, counters)
    end
end

local function ValidateClientCharacter(component, entity, counters)
    DebugValidate(component.Template, entity, counters)

    DebugValidate(component.InputController, entity, counters)
    if component.InputController ~= nil then
        for i,task in pairs(component.InputController.Tasks) do
            DebugValidate(task, entity, counters)
        end
    end
                    
    DebugValidate(component.PlayerData, entity, counters)
    DebugValidate(component.StatusManager, entity, counters)
    for i,status in pairs(component.StatusManager.Statuses) do
        DebugValidate(status, entity, counters)
    end
end

local function ValidateServerItem(component, entity, counters)
    DebugValidate(component.Template, entity, counters)
    DebugValidate(component.StatusManager, entity, counters)
    for i,status in pairs(component.StatusManager.Statuses) do
        DebugValidate(status, entity, counters)
    end
end

-- Iterate through every entity class and consistency check all components
local function ValidateEntities()
    _P("Validating all entity components ...")
    local counters = {}

    for i,entity in ipairs(Ext.Entity.GetAllEntities()) do
        for name,component in pairs(entity:GetAllComponents()) do
            local ok = DebugValidate(component, entity, counters)
            if ok then
                if name == "ServerCharacter" then
                    ValidateServerCharacter(component, entity, counters)
                end
                if name == "ClientCharacter" then
                    ValidateClientCharacter(component, entity, counters)
                end
                if name == "ServerItem" then
                    ValidateServerItem(component, entity, counters)
                end
            end
        end
    end

    _P("Object validation error stats:")
    for name,count in pairs(counters) do
        _P(name .. ": " .. count)
    end
    _P("Done.")
end

local function ValidateAppliedMaterial(mat)
    if Ext.Types.Validate(mat) then
        Ext.Types.Validate(mat.Material)
    end
end

local function ValidateRenderable(r)
    if Ext.Types.Validate(r) then
        if r.ActiveMaterial ~= nil then
            ValidateAppliedMaterial(r.ActiveMaterial)
        end
        for i,obj in pairs(r.AppliedMaterials) do
            ValidateAppliedMaterial(obj)
        end
        for i,obj in pairs(r.AppliedOverlayMaterials) do
            ValidateAppliedMaterial(obj)
        end
    end
end

local function ValidateVisual(visual)
    if Ext.Types.Validate(visual) then
        for i,slot in pairs(visual.SkeletonSlots) do
            Ext.Types.Validate(slot)
            Ext.Types.Validate(slot.Skeleton)
            if slot.Skeleton.SkeletonContent ~= nil then
                Ext.Types.Validate(slot.Skeleton.SkeletonContent)
            end
            if slot.Skeleton.Physics ~= nil then
                Ext.Types.Validate(slot.Skeleton.Physics)
            end
        end

        for i,obj in pairs(visual.ObjectDescs) do
            ValidateRenderable(obj.Renderable)
        end

        for i,obj in pairs(visual.Attachments) do
            if obj.Visual ~= nil then
                ValidateVisual(obj.Visual)
            end
        end
    end
end

local function ValidateVisuals()
    local counters = {}
    for i,entity in ipairs(Ext.Entity.GetAllEntitiesWithComponent("Visual")) do
        if entity.Visual ~= nil and entity.Visual.Visual ~= nil then
            ValidateVisual(entity.Visual.Visual)
        end
    end
end

local function ValidateEffectComponent(comp)
    Ext.Types.Validate(comp)
    if Ext.Types.IsA(comp, "aspk::FxBaseComponent") then
        for i,prop in pairs(comp.Properties) do
            Ext.Types.Validate(prop)
        end

        for i,module in pairs(comp.Modules) do
            Ext.Types.Validate(module)
        end
    end
end

local function ValidateEffect(effect)
    if Ext.Types.Validate(effect) then
        for i,input in pairs(effect.Inputs) do
            Ext.Types.Validate(input)
        end

        for i,comp in pairs(effect.Components) do
            ValidateEffectComponent(comp)
        end
    end
end

local function ValidateEffects()
    local counters = {}
    for i,entity in ipairs(Ext.Entity.GetAllEntitiesWithComponent("Effect")) do
        if entity.Effect.Timeline ~= nil then
            ValidateEffect(entity.Effect.Timeline)
        end
    end
end

local function TryToReserializeObject(obj)
    local serializer = function ()
        local serialized = Ext.Types.Serialize(obj)
        Ext.Types.Unserialize(obj, serialized)
        local serialized2 = Ext.Types.Serialize(obj)
        return Ext.DumpExport(serialized) == Ext.DumpExport(serialized2)
    end

    local ok, err = xpcall(serializer, debug.traceback)
    if not ok then
        return err
    elseif not err then
        return "Mismatch"
    else
        return nil
    end
end

-- Iterate through every entity class and perform an unserialization -> serialization -> unserialization test
-- to check if the serialization logic is symmetric
local function ReserializeEntities()
    _P("Re-serializing entity components ... this is VERY SLOW!")
    local counters = {}

    for i,entity in ipairs(Ext.Entity.GetAllEntities()) do
        for name,component in pairs(entity:GetAllComponents()) do
            DebugValidate(component, entity, counters)
            if name ~= "ServerCharacter" and name ~= "ServerItem" then
                local reason = TryToReserializeObject(component)
                if reason ~= nil then
                    _PE("Serialization failed: " .. tostring(entity) .. ", component " .. name)
                    _PE(reason)
                    if counters[name] == nil then
                        counters[name] = 1
                    else
                        counters[name] = counters[name] + 1
                    end
                end
            end
        end
    end

    _P("Serialization error stats:")
    for name,count in pairs(counters) do
        _P(name .. ": " .. count)
    end
    _P("Done.")
end

local function ValidateUseActions(key, actions)
    for i,action in pairs(actions) do
        if not Ext.Types.Validate(action) then
            _PE("Validation failed: Template " .. key .. ", action " .. tostring(action))
        end
    end
end

local function ValidateTemplateList(templates)
    for key,tmpl in pairs(templates) do
        if not Ext.Types.Validate(tmpl) then
            _PE("Validation failed: Template " .. key)
        else
            if Ext.Types.IsA(tmpl, "CharacterTemplate") then
                ValidateUseActions(key, tmpl.OnDeathActions)
            elseif Ext.Types.IsA(tmpl, "ItemTemplate") then
                ValidateUseActions(key, tmpl.OnUsePeaceActions)
                ValidateUseActions(key, tmpl.OnDestroyActions)
            end
        end
    end
end

local function ValidateTemplates()
    _P("Validating all root templates ...")
    ValidateTemplateList(Ext.Template.GetAllRootTemplates())
    if Ext.IsServer() then
        ValidateTemplateList(Ext.Template.GetAllLocalTemplates())
        ValidateTemplateList(Ext.Template.GetAllCacheTemplates())
        ValidateTemplateList(Ext.Template.GetAllLocalCacheTemplates())
    end
end

local function ReserializeTemplateList(templates)
    for key,tmpl in pairs(templates) do
        local reason = TryToReserializeObject(tmpl)
        if reason ~= nil then
            _PE("Serialization failed: Template " .. key)
            _PE(reason)
        end
    end
end

local function ReserializeTemplates()
    _P("Serializing all root templates ...")
    ReserializeTemplateList(Ext.Template.GetAllRootTemplates())
    if Ext.IsServer() then
        ReserializeTemplateList(Ext.Template.GetAllLocalTemplates())
        ReserializeTemplateList(Ext.Template.GetAllCacheTemplates())
        ReserializeTemplateList(Ext.Template.GetAllLocalCacheTemplates())
    end
end

local function ValidateStaticData()
    _P("Validating all static data resources ...")
    for i,cls in pairs(Ext.Enums.ExtResourceManagerType) do
        if type(i) == "number" and cls ~= "Max" then
            for j,guid in pairs(Ext.StaticData.GetAll(cls)) do
                local resource = Ext.StaticData.Get(guid, cls)
                Ext.Types.Validate(resource)
            end
        end
    end
end

local function ReserializeStaticData()
    _P("Re-serializing all static data resources ...")
    for i,cls in pairs(Ext.Enums.ExtResourceManagerType) do
        if type(i) == "number" and cls ~= "Max" then
            for j,guid in pairs(Ext.StaticData.GetAll(cls)) do
                local resource = Ext.StaticData.Get(guid, cls)
                local reason = TryToReserializeObject(resource)
                if reason ~= nil then
                    _PE("Serialization failed: Static data type " .. tostring(cls) .. ", guid " .. guid)
                    _PE(reason)
                end
            end
        end
    end
end

local function ValidateResources()
    _P("Validating all resources ...")
    for i,cls in pairs(Ext.Enums.ResourceBankType) do
        if type(i) == "number" and cls ~= "Sentinel" then
            for j,guid in pairs(Ext.Resource.GetAll(cls)) do
                local resource = Ext.Resource.Get(guid, cls)
                if resource ~= nil then
                    Ext.Types.Validate(resource)
                end
            end
        end
    end
end

local function ReserializeResources()
    _P("Re-serializing all resources ...")
    for i,cls in pairs(Ext.Enums.ResourceBankType) do
        if type(i) == "number" and cls ~= "Sentinel" then
            for j,guid in pairs(Ext.Resource.GetAll(cls)) do
                local resource = Ext.Resource.Get(guid, cls)
                local reason = TryToReserializeObject(resource)
                if reason ~= nil then
                    _PE("Serialization failed: Resource type " .. tostring(cls) .. ", guid " .. guid)
                    _PE(reason)
                end
            end
        end
    end
end

local function ValidateFunctorGroups(groups)
    if groups == nil then return end

    for i,group in pairs(groups) do
        for j,functor in pairs(group.Functors) do
            Ext.Types.Validate(functor)
        end
    end
end

local function ValidateStats()
    _P("Validating stats entries ...")
    for i,name in pairs(Ext.Stats.GetStats("SpellData")) do
        local spell = Ext.Stats.Get(name)
        ValidateFunctorGroups(spell.OriginSpellFail)
        ValidateFunctorGroups(spell.OriginSpellProperties)
        ValidateFunctorGroups(spell.OriginSpellSuccess)
        ValidateFunctorGroups(spell.SpellFail)
        ValidateFunctorGroups(spell.SpellProperties)
        ValidateFunctorGroups(spell.SpellSuccess)
        ValidateFunctorGroups(spell.ThrowableSpellFail)
        ValidateFunctorGroups(spell.ThrowableSpellProperties)
        ValidateFunctorGroups(spell.ThrowableSpellSuccess)
        local cached = Ext.Stats.GetCachedSpell(name)
        if cached ~= nil then
            Ext.Types.Validate(Ext.Stats.GetCachedSpell(name))
        end
    end

    for i,name in pairs(Ext.Stats.GetStats("InterruptData")) do
        local int = Ext.Stats.Get(name)
        ValidateFunctorGroups(int.Failure)
        ValidateFunctorGroups(int.Properties)
        ValidateFunctorGroups(int.Success)
        Ext.Types.Validate(Ext.Stats.GetCachedInterrupt(name))
    end

    for i,name in pairs(Ext.Stats.GetStats("PassiveData")) do
        local int = Ext.Stats.Get(name)
        ValidateFunctorGroups(int.StatsFunctors)
        ValidateFunctorGroups(int.ToggleOffFunctors)
        ValidateFunctorGroups(int.ToggleOnFunctors)
        Ext.Types.Validate(Ext.Stats.GetCachedPassive(name))
    end

    for i,name in pairs(Ext.Stats.GetStats("StatusData")) do
        local int = Ext.Stats.Get(name)
        ValidateFunctorGroups(int.AuraStatuses)
        ValidateFunctorGroups(int.OnApplyFail)
        ValidateFunctorGroups(int.OnApplyFunctors)
        ValidateFunctorGroups(int.OnApplySuccess)
        ValidateFunctorGroups(int.OnRemoveFail)
        ValidateFunctorGroups(int.OnRemoveFunctors)
        ValidateFunctorGroups(int.OnRemoveSuccess)
        ValidateFunctorGroups(int.OnRollsFailed)
        ValidateFunctorGroups(int.OnSuccess)
        ValidateFunctorGroups(int.OnTickFail)
        ValidateFunctorGroups(int.OnTickSuccess)
        ValidateFunctorGroups(int.TickFunctors)
        Ext.Types.Validate(Ext.Stats.GetCachedStatus(name))
    end

    for i,name in pairs(Ext.Stats.GetStats("Weapon")) do
        local int = Ext.Stats.Get(name)
        ValidateFunctorGroups(int.WeaponFunctors)
    end
end

local function ValidateDialogManager(dialogMgr)
    Ext.Types.Validate(dialogMgr)

    for i,inst in pairs(dialogMgr.Dialogs) do
        Ext.Types.Validate(inst)
    end

    for i,flag in pairs(dialogMgr.FlagDescriptions) do
        Ext.Types.Validate(flag)
    end

    for i,dlg in pairs(dialogMgr.CachedDialogs) do
        Ext.Types.Validate(dlg)

        for j,node in pairs(dlg.Nodes) do
            Ext.Types.Validate(node)
        end
    end
end

local function ValidateGlobals()
    Ext.Types.Validate(Ext.Utils.GetGlobalSwitches())
    Ext.Types.Validate(Ext.Stats.GetStatsManager())
    Ext.Types.Validate(Ext.Mod.GetModManager())
    Ext.Types.Validate(Ext.Mod.GetBaseMod())

    if Ext.UI then
        Ext.Types.Validate(Ext.Input.GetInputManager())
        Ext.Types.Validate(Ext.UI.GetCursorControl())
        Ext.Types.Validate(Ext.UI.GetDragDrop(1))
    
        local helpers = Ext.UI.GetPickingHelper(1)
        if helpers ~= nil then
            Ext.Types.Validate(helpers)
        end
    end

    local dialogMgr = Ext.Utils.GetDialogManager()
    if dialogMgr ~= nil then
        ValidateDialogManager(dialogMgr)
    end
end

local WatchEntities = {}

function WatchSingletons()
    WatchEntities = {}
    for _,comp in pairs(Ext.Enums.ExtComponentType) do
        if type(comp) ~= "number" then
            local comps = Ext.Entity.GetAllEntitiesWithComponent(comp)
            if #comps == 1 then
                WatchEntities[#WatchEntities+1] = {comps[1], comp}
            end
        end
    end

    print("Set up watcher for " .. #WatchEntities .. " singletons")

    Ext.Events.Tick:Subscribe(function () 
        for _,defn in pairs(WatchEntities) do
            local comp = defn[1][defn[2]]
            if comp ~= nil then
                Ext.Types.Validate(comp)
            end
        end
    end)
end

Ext.RegisterConsoleCommand("se_watchsingletons", function ()
    WatchSingletons()
end)

Ext.RegisterConsoleCommand("se_entitytest", function ()
    Ext.Debug.SetEntityRuntimeCheckLevel(0)
    ValidateEntities()
end)

Ext.RegisterConsoleCommand("se_serializertest", function ()
    ReserializeEntities()
end)

Ext.RegisterConsoleCommand("se_roottest", function ()
    ValidateTemplates()
end)

Ext.RegisterConsoleCommand("se_rootserializertest", function ()
    ReserializeTemplates()
end)

Ext.RegisterConsoleCommand("se_resourcetest", function ()
    ValidateResources()
end)

Ext.RegisterConsoleCommand("se_resourceserializertest", function ()
    ReserializeResources()
end)

Ext.RegisterConsoleCommand("se_staticdatatest", function ()
    ValidateStaticData()
end)

Ext.RegisterConsoleCommand("se_staticdataserializertest", function ()
    ReserializeStaticData()
end)

Ext.RegisterConsoleCommand("se_statstest", function ()
    ValidateStats()
end)

Ext.RegisterConsoleCommand("se_visualtest", function ()
    Ext.Debug.SetEntityRuntimeCheckLevel(0)
    ValidateVisuals()
end)

Ext.RegisterConsoleCommand("se_effecttest", function ()
    Ext.Debug.SetEntityRuntimeCheckLevel(0)
    ValidateEffects()
end)

Ext.RegisterConsoleCommand("se_deepfry", function ()
    Ext.Debug.SetEntityRuntimeCheckLevel(0)
    ValidateGlobals()
    ValidateTemplates()
    ValidateStaticData()
    ValidateResources()
    ValidateStats()
    ValidateEntities()
    ValidateVisuals()
    ValidateEffects()
    --ReserializeTemplates()
    --ReserializeStaticData()
    --ReserializeResources()
    --ReserializeEntities()
end)

local GlobalComponentCounters = {}

Ext.RegisterConsoleCommand("se_getunmapped", function ()
    local components = {}

    for i,entity in ipairs(Ext.Entity.GetAllEntities()) do
        for idx,name in pairs(entity:GetAllComponentNames(false)) do
            if components[name] == nil then
                components[name] = 1
            else
                components[name] = components[name] + 1
            end
        end
    end

    for name,count in pairs(components) do
        if GlobalComponentCounters[name] == nil or GlobalComponentCounters[name] < count then
            GlobalComponentCounters[name] = count
        end
    end

    _P("Unmapped components:")
    for name,count in pairs(GlobalComponentCounters) do
        _P(name .. ": " .. count)
    end
end)
