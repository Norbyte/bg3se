local _I = Ext._Internal

_I._PublishedEvents = {
    "KeyInput",
    "MouseButtonInput",
    "MouseWheelInput",
    "ControllerAxisInput",
    "ControllerButtonInput",
    "ViewportResized"
}

Ext.IsClient = function ()
    return true
end

Ext.IsServer = function ()
    return false
end

_I._DoStartup()

-- Test runner helper
if Ext.Debug.IsDeveloperMode() then
    Ext.Utils.LoadTestLibrary()
    Ext.Utils.Include(nil, "builtin://Libs/DevelopmentHelpers.lua")
end

function _C()
    local controlled = Ext.Entity.GetAllEntitiesWithComponent("ClientControl")
    for _,entity in pairs(controlled) do
        if entity.ClientCharacter and entity.ClientCharacter.ReservedUserID == 1 then
            return entity
        end
    end

    return nil
end

-- FIXME - should be called from global load finalizer
-- _I.ModEventManager:FinishedLoading()
