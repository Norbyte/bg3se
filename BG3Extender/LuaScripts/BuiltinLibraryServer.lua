local _I = Ext._Internal

_I._PublishedEvents = {
    "DealDamage",
    "DealtDamage",
    "BeforeDealDamage",
    "ExecuteFunctor",
    "AfterExecuteFunctor"
}

Ext.IsClient = function ()
    return false
end

Ext.IsServer = function ()
    return true
end

_I._GetModPersistentVars = function (modTable)
    local tab = Mods[modTable]
    if tab ~= nil then
        local persistent = tab.PersistentVars
        if persistent ~= nil then
            return Ext.Json.Stringify(persistent)
        end
    end
end

_I._RestoreModPersistentVars = function (modTable, vars)
    local tab = Mods[modTable]
    if tab ~= nil then
        tab.PersistentVars = Ext.Json.Parse(vars)
    end
end

_I._DoStartup()

if Ext.Debug.IsDeveloperMode() then
    Ext.Events.GameStateChanged:Subscribe(function (e)
        if e.ToState == "Running" then
            local args = Ext.Utils.GetCommandLineParams()
            for i,arg in pairs(args) do
                if arg == "-ext:servertest" then
                    Ext.Test.RunTests()
                end
            end
        end
    end)
end

function _C()
    return Ext.Entity.Get(Osi.GetHostCharacter())
end

function _W()
    return Ext.Entity.Get(Osi.GetEquippedWeapon(Osi.GetHostCharacter()) or "00000000-0000-0000-0000-000000000000")
end

-- Test runner helper
if Ext.Debug.IsDeveloperMode() then
    Ext.Utils.LoadTestLibrary()
    Ext.Utils.Include(nil, "builtin://Libs/DevelopmentHelpers.lua")
end

-- FIXME - should be called from global load finalizer
-- _I.ModEventManager:FinishedLoading()
