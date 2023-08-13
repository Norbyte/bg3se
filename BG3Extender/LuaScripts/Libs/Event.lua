local _I = Ext._Internal

local SubscribableEvent = {}

function SubscribableEvent:New(name)
	local o = {
		First = nil,
		NextIndex = 1,
		Name = name
	}
	setmetatable(o, self)
    self.__index = self
    return o
end

function SubscribableEvent:Subscribe(handler, opts)
	opts = opts or {}
	local index = self.NextIndex
	self.NextIndex = self.NextIndex + 1

	local sub = {
		Handler = handler,
		Index = index,
		Priority = opts.Priority or 100,
		Options = opts
	}

	self:DoSubscribe(sub)
	return index
end

function SubscribableEvent:DoSubscribeBefore(node, sub)
	sub.Prev = node.Prev
	sub.Next = node
	
	if node.Prev ~= nil then
		node.Prev.Next = sub
	else
		self.First = sub
	end

	node.Prev = sub
end

function SubscribableEvent:DoSubscribe(sub)
	if self.First == nil then 
		self.First = sub
		return
	end

	local cur = self.First
	local last

	while cur ~= nil do
		last = cur
		if sub.Priority > cur.Priority then
			self:DoSubscribeBefore(cur, sub)
			return
		end

		cur = cur.Next
	end

	last.Next = sub
	sub.Prev = last
end

function SubscribableEvent:RemoveNode(node)
	if node.Prev ~= nil then
		node.Prev.Next = node.Next
	end

	if node.Next ~= nil then
		node.Next.Prev = node.Prev
	end

	node.Prev = nil
	node.Next = nil
end

function SubscribableEvent:Unsubscribe(handlerIndex)
	local cur = self.First
	while cur ~= nil do
		if cur.Index == handlerIndex then
			self:RemoveNode(cur)
			return true
		end

		cur = cur.Next
	end
	
	Ext.Utils.PrintWarning("Attempted to remove subscriber ID " .. handlerIndex .. " for event '" .. self.Name .. "', but no such subscriber exists (maybe it was removed already?)")
	return false
end

function SubscribableEvent:Throw(event)
	local cur = self.First
	while cur ~= nil do
        local ok, result = xpcall(cur.Handler, debug.traceback, event)
        if not ok then
            Ext.Utils.PrintError("Error while dispatching event " .. self.Name .. ": ", result)
        end

		cur = cur.Next
	end
end

local MissingSubscribableEvent = {}

function MissingSubscribableEvent:New(name)
	local o = {
		Name = name
	}
	setmetatable(o, self)
    self.__index = self
    return o
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

_I._Events = {}

Ext.Events = {}
setmetatable(Ext.Events, {
	__index = function (self, event)
		return _I._Events[event] or MissingSubscribableEvent:New(event)
	end,

	__newindex = function (self, k, v)
		error("Cannot write to Ext.Events directly!")
	end
})

_I._ThrowEvent = function (event)
	Ext.Events[event.Name]:Throw(event)
end

_I._RegisterEngineEvent = function (event)
	_I._Events[event] = SubscribableEvent:New(event)
end
