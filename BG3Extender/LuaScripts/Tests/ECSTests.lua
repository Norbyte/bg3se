local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestECSFetch()
    local ent = Ext.Entity.Get(GUID_LAEZEL)
    AssertType(ent, "userdata")

    ent = Ext.Entity.Get("11111111-2222-3333-4444-123412341234")
    AssertEquals(ent, nil)
end

function TestECSComponents()
    local ent = Ext.Entity.Get(GUID_LAEZEL)
    
    local name = ent.DisplayName
    AssertEquals("Lae'zel", name.Name)

    name = ent:GetComponent("DisplayName")
    AssertEquals("Lae'zel", name.Name)

    name = ent:GetAllComponents()["DisplayName"]
    AssertEquals("Lae'zel", name.Name)
end

function TestECSReplication()
    local ent = Ext.Entity.Get(GUID_LAEZEL)
    
    local flags = ent:GetReplicationFlags("DisplayName")
    AssertEquals(flags, 0)
    
    ent:SetReplicationFlags("DisplayName", 7, 0)
    flags = ent:GetReplicationFlags("DisplayName")
    AssertEquals(flags, 7)
    
    ent:Replicate("DisplayName")
    flags = ent:GetReplicationFlags("DisplayName")
    AssertEquals(flags, 0xffffffffffffffff)
end

function TestECSFunctions()
    local ent = Ext.Entity.Get(GUID_LAEZEL)

    AssertEquals(ent:IsAlive(), true)

    AssertType(ent:GetAllComponents().Transform, "userdata")
    AssertType(ent.Transform, "userdata")
    
    AssertType(ent:GetEntityType(), "number")
    AssertType(ent:GetSalt(), "number")
    AssertType(ent:GetIndex(), "number")

    -- GetSalt and GetIndex have no deterministic outputs
end

RegisterTests("ECS", {
    "TestECSFetch",
    "TestECSComponents",
    "TestECSFunctions",
    "TestECSReplication"
})
