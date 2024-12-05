--- @class NetChannel
--- @field RequestHandler fun(string, number): string
--- @field MessageHandler fun(string, number)
--- @field Module string
--- @field Channel string
local NetChannel = {}

---@param module string
---@param channel string
---@return NetChannel
function NetChannel:Instantiate(module, channel)
	return {
        Module = module,
        Channel = channel
	}
end


function NetChannel:SetHandler(handler)
	self.MessageHandler = handler
end

function NetChannel:SetRequestHandler(handler)
	self.RequestHandler = handler
end

function NetChannel:SendToServer(message)
    Ext.Net.PostMessageToServer(self.Channel, Ext.Json.Stringify(message), self.Module)
end

function NetChannel:RequestToServer(message, handler)
    Ext.Net.PostMessageToServer(self.Channel, Ext.Json.Stringify(message), self.Module, handler)
end

function NetChannel:SendToClient(message, user)
    local msg = Ext.Json.Stringify(message)
    if type(user) == "number" then
        Ext.Net.PostMessageToUser(user, self.Channel, msg, self.Module)
    else
        Ext.Net.PostMessageToClient(user, self.Channel, msg, self.Module)
    end
end

function NetChannel:RequestToClient(message, user, handler)
    local msg = Ext.Json.Stringify(message)
    if type(user) == "number" then
        Ext.Net.PostMessageToUser(user, self.Channel, msg, self.Module, handler)
    else
        Ext.Net.PostMessageToClient(user, self.Channel, msg, self.Module, handler)
    end
end

function NetChannel:Broadcast(message, excludeCharacter)
    Ext.Net.BroadcastMessage(self.Channel, Ext.Json.Stringify(message), excludeCharacter, self.Module)
end

function NetChannel:OnMessage(e)
    local request = Ext.Json.Parse(e.Payload)
    if e.RequestId then
        if self.RequestHandler then
			local ok, ret = xpcall(self.RequestHandler, debug.traceback, request, e.UserID)
			if ok then
                local reply = Ext.Json.Stringify(ret)
                if Ext.IsServer() then
                    Ext.Net.PostMessageToUser(e.UserID, e.Channel, reply, e.Module, nil, e.RequestId)
                else
                    Ext.Net.PostMessageToServer(e.Channel, reply, e.Module, nil, e.RequestId)
                end
            else
				_PE("Error during request dispatch for module " .. e.Module .. ", channel " .. e.Channel .. ": " .. ret)
			end
        else
            Ext.Log.PrintWarning("Net request received for module " .. e.Module .. ", channel " .. e.Channel .. ", but no request handler was registered!")
        end
    else
        if self.MessageHandler then
			local ok, err = xpcall(self.MessageHandler, debug.traceback, request, e.UserID)
			if not ok then
				_PE("Error during message dispatch for module " .. e.Module .. ", channel " .. e.Channel .. ": " .. err)
			end
        else
            Ext.Log.PrintWarning("Net message received for module " .. e.Module .. ", channel " .. e.Channel .. ", but no message handler was registered!")
        end
    end
end


return Class.Create(NetChannel)
