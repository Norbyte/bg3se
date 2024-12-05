--- @type NetChannel
local NetChannel = Ext.CoreLib("NetChannel")

--- @class NetworkManager
local NetworkManager = {
    --- @type table<string, table<string, NetChannel>>
    Channels = {}
}

function NetworkManager:CreateNetModule(ext)
	local net = {}
	setmetatable(net, {
		__index = ext.Net,
	})
	net.CreateChannel = function (module, channel, messageHandler, requestHandler)
		local ch = self:AddChannel(module, channel)
        ch.MessageHandler = messageHandler
        ch.RequestHandler = requestHandler
        return ch
	end
	return net
end

--- @param module string
--- @param channel string
function NetworkManager:AddChannel(module, channel)
    if not Ext.Mod.IsModLoaded(module) then
        error("Creating network channel for nonexistent mod " .. module)
    end

    local ch = NetChannel:New(module, channel)
    self:RegisterChannel(ch)
    return ch
end

--- @param channel NetChannel
function NetworkManager:RegisterChannel(channel)
    if self.Channels[channel.Module] == nil then
        self.Channels[channel.Module] = {}
    end

    if self.Channels[channel.Module][channel.Channel] ~= nil then
        error("Channel '" .. channel.Channel .. "' for module " .. channel.Module .. " already registered")
    end

    self.Channels[channel.Module][channel.Channel] = channel
end


function NetworkManager:RegisterEvents()
	Ext.Events.NetModMessage:Subscribe(function (e)
		self:MessageReceived(e)
	end)
end


--- @param e LuaNetMessageEvent
function NetworkManager:MessageReceived(e)
    if not e.Module then
        error("Mod message event dispatched without a module")
    end

    if not self.Channels[e.Module] or not self.Channels[e.Module][e.Channel] then
        Ext.Log.PrintWarning("Net message received for module " .. e.Module .. ", channel " .. e.Channel .. ", but no such channel was registered!")
        return
    end

    local channel = self.Channels[e.Module][e.Channel]
    channel:OnMessage(e)
end

return NetworkManager
