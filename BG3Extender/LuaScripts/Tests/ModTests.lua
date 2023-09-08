function TestModLoaded()
    -- Check game base mod
    AssertEquals(Ext.Mod.IsModLoaded("28ac9ce2-2aba-8cda-b3b5-6e922f71b6b8"), true)
    -- Check game dependency
    AssertEquals(Ext.Mod.IsModLoaded("ed539163-bb70-431b-96a7-f5b2eda5376b"), true)
    -- Check nonexistent mod
    AssertEquals(Ext.Mod.IsModLoaded("01010101-0202-0303-0404-050505050505"), false)
end

function TestModInfo()
    local shared = Ext.Mod.GetMod("ed539163-bb70-431b-96a7-f5b2eda5376b")
    AssertEquals(shared.Info.Name, "Shared")
    AssertEquals(shared.Info.ModuleUUID, "ed539163-bb70-431b-96a7-f5b2eda5376b")
end

function TestBaseMod()
    local gustav = Ext.Mod.GetBaseMod()
    AssertEquals(gustav.Info.ModuleUUID, "28ac9ce2-2aba-8cda-b3b5-6e922f71b6b8")
end

function TestModManager()
    local modMgr = Ext.Mod.GetModManager()
    AssertEquals(modMgr.BaseModule.Info.ModuleUUID, "28ac9ce2-2aba-8cda-b3b5-6e922f71b6b8")
end

RegisterTests("Mod", {
    "TestModLoaded",
    "TestModInfo",
    "TestBaseMod",
    "TestModManager"
})
