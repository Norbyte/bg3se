local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestBoostComponent(boostType, index, boostComponent, expect)
    local boosts = Ext.GetCharacter(GUID_LAEZEL).BoostsContainer
    local boost = boosts.Boosts[boostType][index]
    local component = boost[boostComponent]

    -- FIXME - add ==, ~= operator for entities, component handles
    -- expect.Entity = ch
    AssertEqualsProperties(expect, component)
end


function TestBoostDump()
    local boosts = Ext.GetCharacter(GUID_LAEZEL).BoostsContainer
    for ty,boosts in pairs(boosts.Boosts) do
        for i,boost in ipairs(boosts) do
            Ext.Json.Stringify(boost:GetAllComponents(), true, true)
        end
    end
end


function TestBoostInfoComponent()
    local expect = {
        -- FIXME - Cause_M = "Entity (03000001000000ac)",
        Flags_M = 1,
        -- FIXME - OwnerEntity = Ext.GetCharacter(GUID_LAEZEL)",
        Params = {
                Boost = "Disadvantage",
                Params = "Skill,Stealth",
                Params2 = ""
        },
        Passive = "",
        Type = "Advantage",
        field_18 = null,
        field_38 = -1,
        field_90 = "00000000-0000-0000-0000-000000000000"
    }
    
    TestBoostComponent("Advantage", 1, "BoostInfo", expect)
end


function TestBoostAdvantageComponent()
    local expect = {
        Ability = null,
        AdvantageType = "Skill",
        Skill = "Stealth",
        Tags = {},
        Type = "Disadvantage"
    }
    
    TestBoostComponent("Advantage", 1, "AdvantageBoost", expect)
end


function TestBoostServerReplicationDependencyComponent()
    local expect = {
        -- FIXME - Dependency = Ext.GetCharacter(GUID_LAEZEL)
    }
    
    TestBoostComponent("Advantage", 1, "ServerReplicationDependency", expect)
end
