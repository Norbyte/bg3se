Ext.Require("TestHelpers.lua")
Ext.Require("ResourceTests.lua")
Ext.Require("StatTests.lua")
Ext.Require("ComponentTests.lua")
Ext.Require("CharacterTests.lua")
Ext.Require("CharacterComponentTests.lua")
Ext.Require("ItemComponentTests.lua")
Ext.Require("BoostComponentTests.lua")

Ext.RegisterConsoleCommand("dyntest", function ()
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

Ext.RegisterListener("OnDealDamage", function (functor, caster, target, position, isFromItem, spellId, originator, hit, damageSums, hitWith)
    --Ext.Utils.PrintWarning("Functor is: " .. tostring(functor))
    --for k,v in pairs(functor) do
    --    Ext.Utils.Print(k .. ": " .. Ext.Json.Stringify(v, true, true, true))
    --end
    --Ext.Utils.PrintWarning("--- end functor dump ---")

    if hitWith == "Projectile" and tostring(caster) ~= tostring(target) then
        --Ext.Utils.PrintWarning(" --- OnDealDamage --- ")
        --Ext.Dump({functor, caster, target, position, isFromItem, spellId, originator, hit, damageSums, hitWith})
    end


end)

Ext.RegisterListener("OnExecuteFunctor", function (fun, args)
    --Ext.Dump(args)
    --Mods.xargs = args
end)

function RunTestSuite()
    Ext.Utils.Print(" --- STARTING TESTS --- ")

    local tests = {
        -- "TestStatAttributes",
        -- "TestStatAttributeReassignment",
        
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
        "TestResourceLayout",

        "TestEntityFunctions",
        "TestComponentFunctions",
        
        "TestCharacterEnumeration",
        "TestCharacterProperties",
        "TestCharacterTemplateProperties",

        "TestCharacterComponentDump",
        "TestAnubisComponent",
        "TestAddedSpellsComponent",
        "TestAvailableLevelComponent",
        "TestBaseHpComponent",
        "TestCanBeLootedComponent",
        "TestCanDeflectProjectilesComponent",
        "TestCanDoActionsComponent",
        "TestCanInteractComponent",
        "TestCanModifyHealthComponent",
        "TestCanMoveComponent",
        "TestCanSenseComponent",
        "TestCanSpeakComponent",
        "TestCanTravelComponent",
        "TestCombatParticipantComponent",
        "TestConcentrationComponent",
        "TestDarknessComponent",
        "TestDataComponent",
        "TestDisplayNameComponent",
        "TestDualWieldingComponent",
        "TestFleeCapabilityComponent",
        "TestGameObjectVisualComponent",
        "TestGenderComponent",
        "TestHealthComponent",
        "TestHotbarContainerComponent",
        "TestIconComponent",
        "TestLevelComponent",
        "TestLootComponent",
        "TestMovementComponent",
        "TestNetComponent",
        "TestPathingComponent",
        "TestRaceComponent",
        "TestRelationComponent",
        "TestSenseComponent",
        "TestServerDisplayNameListComponent",
        "TestServerExperienceGaveOutComponent",
        "TestServerIconListComponent",
        "TestServerOsirisTagComponent",
        "TestServerRaceTagComponent",
        "TestServerSafePositionComponent",
        "TestServerToggledPassivesComponent",
        "TestServerTriggerStateComponent",
        "TestSightComponent",
        "TestSpellBookComponent",
        "TestSpellBookPreparesComponent",
        "TestSpellContainerComponent",
        "TestStatsComponent",
        "TestStatusImmunitiesComponent",
        "TestSurfacePathInfluencesComponent",
        "TestTransformComponent",

        "TestItemComponentDump",
        "TestItemCanBeInInventoryComponent",
        "TestItemCombatParticipantComponent",
        "TestItemDataComponent",
        "TestCanBeInInventoryComponent",
        "TestItemDisplayNameComponent",
        "TestItemEquipableComponent",
        "TestItemGameObjectVisualComponent",
        "TestItemInventorySlotComponent",
        "TestItemBoostsComponent",
        "TestItemStatusImmunitiesComponent",
        "TestItemUseComponent",
        "TestItemValueComponent",
        "TestItemWeaponComponent",
        "TestItemWieldingComponent",
        "TestItemComponent",
        "TestItemTemplate",

        "TestBoostDump",
        "TestBoostInfoComponent",
        "TestBoostAdvantageComponent",
        "TestBoostServerReplicationDependencyComponent"
    }

    for i,test in ipairs(tests) do
        RunTest(test, _G[test])
    end

    Ext.Utils.Print(" --- FINISHING TESTS --- ")
end


Ext.RegisterConsoleCommand("selftest", function ()
    Ext.Utils.Print(" --- STARTING TESTS --- ")

    RunTestSuite()

    Ext.Utils.Print(" --- FINISHING TESTS --- ")
end)

Ext.RegisterListener("GameStateChanged", function (fromState, toState)
    _D(fromState .. " -> " .. toState)
    if toState == "Running" then
        local args = Ext.Utils.GetCommandLineParams()
        for i,arg in pairs(args) do
            if arg == "selftest=1" then
                Ext.Utils.PrintWarning(" --- RUNNING TEST SUITE --- ")
                Ext.Utils.PrintWarning(" --- RUNNING TEST SUITE --- ")
                Ext.Utils.PrintWarning(" --- RUNNING TEST SUITE --- ")
                RunTestSuite()
            end
        end
    end
end)
