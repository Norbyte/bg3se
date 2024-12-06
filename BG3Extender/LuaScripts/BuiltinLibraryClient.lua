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

-- FIXME - should be called from global load finalizer
-- _I.ModEventManager:FinishedLoading()
