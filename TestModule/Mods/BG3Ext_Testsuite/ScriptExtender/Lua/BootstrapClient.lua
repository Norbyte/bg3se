Ext.Require("TestHelpers.lua")
Ext.Require("StatTests.lua")
Ext.Require("ResourceTests.lua")

Ext.RegisterConsoleCommand("resourcetest", function ()
    Ext.Utils.Print(" --- STARTING TESTS --- ")

    local tests = {
        "TestStatAttributes",
        "TestStatAttributeReassignment",
        
        "TestActionResources",
        "TestActionResourceGroupDefinitions",
        "TestClassDescriptionResources",
        "TestTagResources",
        "TestFactionResources",
        "TestRaceResources",
        "TestOriginResources",
        "TestBackgroundResources",
        "TestGodResources",
        "TestProgressionResources",
        "TestProgressionDescriptionResources",
        "TestGossipDefinitions",
        "TestEquipmentTypeResources",
        "TestFlagResources",
        "TestFeatResources",
        "TestFeatDescriptionResources",

        "TestGuidResourceLayout",

        "TestResourceEnumeration",
        "TestResourceLayout"
    }

    for i,test in ipairs(tests) do
        RunTest(test, _G[test])
    end

    Ext.Utils.Print(" --- FINISHING TESTS --- ")
end)
