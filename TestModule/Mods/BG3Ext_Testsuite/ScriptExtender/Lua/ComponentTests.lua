local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestBoostComponent(boostType, index, boostComponent, expect)
    local boosts = Ext.GetCharacter(GUID_LAEZEL).BoostsContainer
    local boost = boosts.Boosts[boostType][index]
    local component = boost[boostComponent]

    -- FIXME - add ==, ~= operator for entities, component handles
    -- expect.Entity = ch
    AssertEqualsProperties(expect, component)
end

function TestEntityFunctions()
    local char = Ext.GetCharacter(GUID_LAEZEL)

    AssertEquals(char:IsAlive(), true)

    AssertEquals(char:HasRawComponent("ls::TransformComponent"), true)
    AssertEquals(char:HasRawComponent("nonexistent"), false)

    AssertType(char:GetComponentHandles()["ls::TransformComponent"], "userdata")
    AssertType(char:GetAllComponents().Transform, "userdata")
    AssertType(char.Transform, "userdata")
    
    AssertEquals(char:GetEntityType(), 10)

    -- GetSalt and GetIndex have no deterministic outputs
end

function TestComponentFunctions()
    local char = Ext.GetCharacter(GUID_LAEZEL)
    local comp = char.Transform.Handle

    AssertEquals(comp:GetTypeName(), "ls::TransformComponent")
    AssertEquals(comp:GetType(), 526)

    AssertType(comp:Get(), "userdata")

    -- GetSalt and GetIndex have no deterministic outputs
end

