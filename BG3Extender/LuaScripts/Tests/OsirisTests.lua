function TestOsirisCallSubscribers()
    local regOk = false
    local regOk2 = false
    Ext.Osiris.RegisterListener("SetCanGossip", 2, "before", function (a, b) 
        regOk = (a == Osi.GetHostCharacter() and b == 1)
    end)
    Ext.Osiris.RegisterListener("SetCanGossip", 2, "after", function (a, b) 
        regOk2 = (a == Osi.GetHostCharacter() and b == 1)
    end)
    Osi.SetCanGossip(Osi.GetHostCharacter(), 1)
    AssertEquals(regOk, true)
    AssertEquals(regOk2, true)
end

function TestOsirisDBSubscribers()
    local beforeInsertOk = false
    local afterInsertOk = false
    local beforeDeleteOk = false
    local afterDeleteOk = false
    Ext.Osiris.RegisterListener("DB_Players", 1, "before", function (a) 
        beforeInsertOk = (a == Osi.GetHostCharacter())
    end)
    Ext.Osiris.RegisterListener("DB_Players", 1, "after", function (a) 
        afterInsertOk = (a == Osi.GetHostCharacter())
    end)
    Ext.Osiris.RegisterListener("DB_Players", 1, "beforeDelete", function (a) 
        beforeDeleteOk = (a == Osi.GetHostCharacter())
    end)
    Ext.Osiris.RegisterListener("DB_Players", 1, "afterDelete", function (a) 
        afterDeleteOk = (a == Osi.GetHostCharacter())
    end)

    Osi.DB_Players(Osi.GetHostCharacter())
    AssertEquals(beforeInsertOk, true)
    AssertEquals(afterInsertOk, true)
    AssertEquals(beforeDeleteOk, false)
    AssertEquals(afterDeleteOk, false)

    Osi.DB_Players:Delete(Osi.GetHostCharacter())
    AssertEquals(beforeDeleteOk, true)
    AssertEquals(afterDeleteOk, true)
end

function TestOsirisUserQuerySubscribers()
    local regOk = false
    local regOk2 = false
    Ext.Osiris.RegisterListener("QRY_CampNight_MeetsRequirements", 1, "before", function (flag) 
        regOk = (flag == "woof")
    end)
    Ext.Osiris.RegisterListener("QRY_CampNight_MeetsRequirements", 1, "after", function (flag) 
        regOk2 = (flag == "woof")
    end)
    Osi.QRY_CampNight_MeetsRequirements("woof")
    AssertEquals(regOk, true)
    AssertEquals(regOk2, true)
end

RegisterTests("Stats", {
    "TestOsirisCallSubscribers",
    "TestOsirisDBSubscribers",
    "TestOsirisUserQuerySubscribers"
})
