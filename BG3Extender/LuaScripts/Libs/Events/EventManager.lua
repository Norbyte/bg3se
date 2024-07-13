local _I = Ext._Internal
local SubscribableEvent = Ext.CoreLib("Events/SubscribableEvent")
local MissingSubscribableEvent = Ext.CoreLib("Events/MissingSubscribableEvent")

local EventManager = {}

function EventManager:Instantiate()
	return {
		Events = {},
        NetListeners = {},
        ConsoleCommandListeners = {}
	}
end


function EventManager:ThrowEvent(event)
	self.Events[event.Name]:Throw(event)
end


function EventManager:RegisterEngineEvent(event)
	self.Events[event] = SubscribableEvent:New(event)
end


function EventManager:RegisterEvents()
	self:RegisterEngineEvent("NetMessage")
	
	for i,ev in pairs(_I._PublishedSharedEvents) do
		self:RegisterEngineEvent(ev)
	end
	
	for i,ev in pairs(_I._PublishedEvents) do
		self:RegisterEngineEvent(ev)
	end

	-- Support for Ext.RegisterConsoleCommand()
	self.Events.DoConsoleCommand:Subscribe(function (e)
		self:DoConsoleCommand(e.Command)
	end)

	-- Support for Ext.RegisterNetListener()
	self.Events.NetMessage:Subscribe(function (e)
		self:NetMessageReceived(e.Channel, e.Payload, e.UserID)
	end)
end


function EventManager:RegisterNetListener(channel, fn)
	if self.NetListeners[channel] == nil then
		self.NetListeners[channel] = {}
	end

	table.insert(self.NetListeners[channel], fn)
end


function EventManager:NetMessageReceived(channel, payload, userId)
	if self.NetListeners[channel] ~= nil then
		for i,callback in pairs(self.NetListeners[channel]) do
			local ok, err = xpcall(callback, debug.traceback, channel, payload, userId)
			if not ok then
				Ext.Utils.PrintError("Error during NetMessage dispatch: ", err)
			end
		end
	end
end


function EventManager:DoConsoleCommand(cmd)
	local params = {}
	for param in string.gmatch(cmd, "%S+") do
		table.insert(params, param)
	end

	local listeners = self.ConsoleCommandListeners[params[1]]
	if listeners ~= nil then
		for i,callback in pairs(listeners) do
			local status, result = xpcall(callback, debug.traceback, table.unpack(params))
			if not status then
				Ext.Utils.PrintError("Error during console command callback: ", result)
			end
		end
	end
end


function EventManager:RegisterConsoleCommand(cmd, fn)
	if self.ConsoleCommandListeners[cmd] == nil then
		self.ConsoleCommandListeners[cmd] = {}
	end

	table.insert(self.ConsoleCommandListeners[cmd], fn)
end


function EventManager:Expose()
    Ext.RegisterNetListener = function (channel, fn)
        self:RegisterNetListener(channel, fn)
    end

    Ext.RegisterConsoleCommand = function (cmd, fn)
        self:RegisterConsoleCommand(cmd, fn)
    end
    
    _I._ThrowEvent = function (event)
        self:ThrowEvent(event)
    end

    _I._NetMessageReceived = function (channel, payload, userId)
        self:NetMessageReceived(channel, payload, userId)
    end

    _I.DoConsoleCommand = function (cmd)
        self:DoConsoleCommand(cmd)
    end
end


function EventManager:CreateEventTable()
    local events = {}
    setmetatable(events, {
        __index = function (_, event)
            return self.Events[event] or MissingSubscribableEvent:New(event)
        end,

        __newindex = function (_, k, v)
            error("Cannot write to Ext.Events directly!")
        end
    })
    return events
end


return Class.Create(EventManager)
