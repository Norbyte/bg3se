local EventManager = Ext.CoreLib("Events/EventManager")
local ModEventManager = Ext.CoreLib("Events/ModEventManager")
local _I = Ext._Internal

_I.EventManager = EventManager:New()
_I.EventManager:Expose()
Ext.Events = _I.EventManager:CreateEventTable()

_I.ModEventManager = ModEventManager:New()
Ext.ModEvents = _I.ModEventManager:CreateModTable()

Ext.RegisterModEvent = function (mod, event)
    _I.ModEventManager:RegisterEvent(mod, event)
end
