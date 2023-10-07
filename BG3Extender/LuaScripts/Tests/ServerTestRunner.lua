Ext.Utils.Include(nil, "builtin://Tests/TestHelpers.lua")
Ext.Utils.Include(nil, "builtin://Tests/ModTests.lua")
Ext.Utils.Include(nil, "builtin://Tests/StaticDataTests.lua")
Ext.Utils.Include(nil, "builtin://Tests/StatTests.lua")
Ext.Utils.Include(nil, "builtin://Tests/ECSTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/ResourceTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/CharacterTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/CharacterComponentTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/ItemComponentTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/BoostComponentTests.lua")

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
    if not Ext.Types.Validate(obj) then
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
Ext.RegisterConsoleCommand("se_entitytest", function ()
    _P("Starting entity iteration test ... this may take a while")
    local counters = {}

    for i,entity in ipairs(Ext.Entity.GetAllEntities()) do
        for name,component in pairs(entity:GetAllComponents()) do
            DebugValidate(component, entity, counters)
            if name == "ServerCharacter" then
                if DebugValidate(component.Character, entity, counters) then
                    DebugValidate(component.Character.Template, entity, counters)
                    DebugValidate(component.Character.StatusManager, entity, counters)
                    for i,status in pairs(component.Character.StatusManager.Statuses) do
                        DebugValidate(status, entity, counters)
                    end
                end
            end
            if name == "ServerItem" then
                if DebugValidate(component.Item, entity, counters) then
                    DebugValidate(component.Item.Template, entity, counters)
                    DebugValidate(component.Item.StatusManager, entity, counters)
                    for i,status in pairs(component.Item.StatusManager.Statuses) do
                        DebugValidate(status, entity, counters)
                    end
                end
            end
        end
    end

    _P("Error stats:")
    for name,count in pairs(counters) do
        _P(name .. ": " .. count)
    end
    _P("Done.")
end)
