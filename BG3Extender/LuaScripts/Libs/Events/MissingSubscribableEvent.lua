local MissingSubscribableEvent = {}

function MissingSubscribableEvent:Instantiate(name)
	return {
		Name = name
	}
end

function MissingSubscribableEvent:Subscribe(handler, opts)
    Ext.Utils.PrintError("Attempted to subscribe to nonexistent event: " .. self.Name)
end

function MissingSubscribableEvent:Unsubscribe(handlerIndex)
    Ext.Utils.PrintError("Attempted to unsubscribe from nonexistent event: " .. self.Name)
end

function MissingSubscribableEvent:Throw(event)
    Ext.Utils.PrintError("Attempted to throw nonexistent event: " .. self.Name)
end

return Class.Create(MissingSubscribableEvent)
