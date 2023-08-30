Ext.Utils.Include(nil, "builtin://Tests/TestHelpers.lua")
--Ext.Utils.Include(nil, "builtin://Tests/ResourceTests.lua")
Ext.Utils.Include(nil, "builtin://Tests/StatTests.lua")
--Ext.Utils.Include(nil, "builtin://Tests/ComponentTests.lua")
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
