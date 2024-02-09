Ext.RegisterConsoleCommand("se_dyntest", function ()
    Ext.Utils.Print(" --- STARTING TESTS --- ")

    local tests = {
        "TestCharacterEnumeration",
        "TestCharacterProperties",
        "TestCharacterTemplateProperties"
    }

    for i,test in ipairs(tests) do
        RunTest(test, _G[test])
    end

    Ext.Utils.Print(" --- FINISHING TESTS --- ")
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

-- Iterate through every entity class and consistency check all components
local function ValidateEntities()
    _P("Validating all entity components ...")
    local counters = {}

    for i,entity in ipairs(Ext.Entity.GetAllEntities()) do
        for name,component in pairs(entity:GetAllComponents()) do
            DebugValidate(component, entity, counters)
            if name == "ServerCharacter" then
                if DebugValidate(component, entity, counters) then
                    DebugValidate(component.Template, entity, counters)
                    DebugValidate(component.StatusManager, entity, counters)
                    for i,status in pairs(component.StatusManager.Statuses) do
                        DebugValidate(status, entity, counters)
                    end
                end
            end
            if name == "ServerItem" then
                if DebugValidate(component, entity, counters) then
                    DebugValidate(component.Template, entity, counters)
                    DebugValidate(component.StatusManager, entity, counters)
                    for i,status in pairs(component.StatusManager.Statuses) do
                        DebugValidate(status, entity, counters)
                    end
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

local function ValidateTemplateList(templates)
    for key,tmpl in pairs(templates) do
        if not Ext.Types.Validate(tmpl) then
            _PE("Validation failed: Template " .. key)
        end
    end
end

local function ValidateTemplates()
    _P("Validating all root templates ...")
    ValidateTemplateList(Ext.Template.GetAllRootTemplates())
    ValidateTemplateList(Ext.Template.GetAllLocalTemplates())
    ValidateTemplateList(Ext.Template.GetAllCacheTemplates())
    ValidateTemplateList(Ext.Template.GetAllLocalCacheTemplates())
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
    ReserializeTemplateList(Ext.Template.GetAllLocalTemplates())
    ReserializeTemplateList(Ext.Template.GetAllCacheTemplates())
    ReserializeTemplateList(Ext.Template.GetAllLocalCacheTemplates())
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

Ext.RegisterConsoleCommand("se_entitytest", function ()
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

Ext.RegisterConsoleCommand("se_deepfry", function ()
    Ext.Debug.SetEntityRuntimeCheckLevel(0)
    ValidateTemplates()
    ValidateStaticData()
    ValidateResources()
    ValidateEntities()
    ReserializeTemplates()
    ReserializeStaticData()
    ReserializeResources()
    ReserializeEntities()
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
