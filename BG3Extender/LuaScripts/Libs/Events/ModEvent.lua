local SubscribableEvent = Ext.CoreLib("Events/SubscribableEvent")

local ModEvent = {}

function ModEvent:Instantiate(mod, event)
	return {
		Mod = mod,
        EventName = event,
        Event = SubscribableEvent:New(mod .. "." .. event),
        Registered = false
	}
end

return Class.Create(ModEvent)
