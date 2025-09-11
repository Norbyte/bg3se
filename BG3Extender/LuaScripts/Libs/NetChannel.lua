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

function NetChannel:IsBinary()
    return Ext.Net.Version() >= 2
end

function NetChannel:Stringify(msg)
    return Ext.Json.Stringify(msg, {Binary = self:IsBinary()})
end

function NetChannel:DoSendToServer(message, handler)
    local msg = self:Stringify(message)
    Ext.Net.PostMessageToServer(self.Channel, msg, self.Module, handler, nil, self:IsBinary())
end

function NetChannel:SendToServer(message)
    self:DoSendToServer(message, nil)
end

function NetChannel:RequestToServer(message, handler)
    local replyHandler = function (reply)
        handler(Ext.Json.Parse(reply))
    end
    self:DoSendToServer(message, replyHandler)
end

function NetChannel:DoSendToClient(message, user, handler)
    local msg = self:Stringify(message)
    if type(user) == "number" then
        Ext.Net.PostMessageToUser(user, self.Channel, msg, self.Module, handler, nil, self:IsBinary())
    else
        Ext.Net.PostMessageToClient(user, self.Channel, msg, self.Module, handler, nil, self:IsBinary())
    end
end

function NetChannel:SendToClient(message, user)
    self:DoSendToClient(message, user, nil)
end

function NetChannel:RequestToClient(message, user, handler)
    local replyHandler = function (reply)
        handler(Ext.Json.Parse(reply))
    end
    self:DoSendToClient(message, user, replyHandler)
end

function NetChannel:Broadcast(message, excludeCharacter)
    local msg = self:Stringify(message)
    Ext.Net.BroadcastMessage(self.Channel, msg, excludeCharacter, self.Module, nil, nil, self:IsBinary())
end

function NetChannel:OnMessage(e)
    local request = Ext.Json.Parse(e.Payload, e.Binary)
    if e.RequestId then
        if self.RequestHandler then
            local ok, ret = xpcall(self.RequestHandler, debug.traceback, request, e.UserID)
            if ok then
                local reply = self:Stringify(ret)
                if Ext.IsServer() then
                    Ext.Net.PostMessageToUser(e.UserID, e.Channel, reply, e.Module, nil, e.RequestId, self:IsBinary())
                else
                    Ext.Net.PostMessageToServer(e.Channel, reply, e.Module, nil, e.RequestId, self:IsBinary())
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
