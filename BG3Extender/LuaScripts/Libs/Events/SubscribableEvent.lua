local _I = Ext._Internal

--- @class SubscriptionData
--- @field Handler fun()
--- @field Index number
--- @field Priority number
--- @field Once boolean
--- @field Options table
--- @field Prev SubscriptionData
--- @field Next SubscriptionData


--- @class SubscribableEvent
--- @field First SubscriptionData
--- @field NextIndex number
--- @field IdPrefix number
--- @field Name string
--- @field PendingDeletions number[]
--- @field EnterCount number
local SubscribableEvent = {}

---@param name string
---@param idPrefix number|nil
---@return SubscribableEvent
function SubscribableEvent:Instantiate(name, idPrefix)
    return {
        First = nil,
        NextIndex = 1,
        IdPrefix = idPrefix or Ext.Math.Random(1, 0xfffffff),
        Name = name,
        PendingDeletions = {},
        EnterCount = 0
    }
end

function SubscribableEvent:Subscribe(handler, opts)
    opts = opts or {}
    local index = self.NextIndex
    self.NextIndex = self.NextIndex + 1

    local sub = {
        Handler = handler,
        Index = index,
        Priority = opts.Priority or 100,
        Once = opts.Once or false,
        Options = opts
    }

    self:DoSubscribe(sub)
    return index | (self.IdPrefix << 32)
end

---@param node SubscriptionData
---@param sub SubscriptionData
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

---@param sub SubscriptionData
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

---@param node SubscriptionData
function SubscribableEvent:RemoveNode(node)
    if node.Prev ~= nil then
        node.Prev.Next = node.Next
    end

    if node.Next ~= nil then
        node.Next.Prev = node.Prev
    end

    if self.First == node then
        self.First = node.Next
    end

    node.Prev = nil
    node.Next = nil
end

function SubscribableEvent:Unsubscribe(id)
    if id >> 32 ~= self.IdPrefix then
        local evt = _I.EventManager.EventsById[id >> 32]
        if evt == nil then
            Ext.Log.PrintWarning("Attempted to remove subscriber ID " .. id .. " for event '" .. self.Name .. "', but the subscription is for a different event!")
        else
            Ext.Log.PrintWarning("Attempted to remove subscriber ID " .. id .. " for event '" .. self.Name .. "', but the subscription is for event '" .. evt.Name .. "'!")
        end
        return
    end

    local handlerIndex = id & 0xffffffff
    if self.EnterCount == 0 then
        self:DoUnsubscribe(handlerIndex)
    else
        table.insert(self.PendingDeletions, handlerIndex)
    end
end

function SubscribableEvent:DoUnsubscribe(handlerIndex)
    local cur = self.First
    while cur ~= nil do
        if cur.Index == handlerIndex then
            self:RemoveNode(cur)
            return
        end

        cur = cur.Next
    end
    
    Ext.Log.PrintWarning("Attempted to remove subscriber index " .. handlerIndex .. " for event '" .. self.Name .. "', but no such subscriber exists (maybe it was removed already?)")
end

function SubscribableEvent:ProcessUnsubscriptions()
    if self.EnterCount == 0 and #self.PendingDeletions > 0 then
        for i,handlerIndex in pairs(self.PendingDeletions) do
            self:DoUnsubscribe(handlerIndex)
        end

        self.PendingDeletions = {}
    end
end

if Ext.Config.ProfilerEnabled then

    -- Separate profiler-enabled version for perf reasons
    function SubscribableEvent:Dispatch(event, handler)
        local startTime = Ext.Timer.MicrosecTime()
        local ok, result = xpcall(handler, debug.traceback, event)
        local took = Ext.Timer.MicrosecTime() - startTime
        if not ok then
            Ext.Log.PrintError("Error while dispatching event " .. self.Name .. ": ", result)
        else
            if _I.Profiler:ShouldLikelyReport(took) and self.Name ~= "DoConsoleCommand" then
                local source, line = Ext.Types.GetFunctionLocation(handler)
                _I.Profiler:Report(took, "Dispatching event " .. self.Name .. " (" .. source .. ":" .. line .. ")")
            end
        end
    end

else

    function SubscribableEvent:Dispatch(event, handler)
        local ok, result = xpcall(handler, debug.traceback, event)
        if not ok then
            Ext.Log.PrintError("Error while dispatching event " .. self.Name .. ": ", result)
        end
    end

end

function SubscribableEvent:Throw(event)
    self.EnterCount = self.EnterCount + 1

    local cur = self.First
    while cur ~= nil do
        if event.Stopped then
            break
        end

        self:Dispatch(event, cur.Handler)

        if cur.Once then
            local last = cur
            cur = last.Next
            self:RemoveNode(last)
        else
            cur = cur.Next
        end
    end

    self.EnterCount = self.EnterCount - 1
    self:ProcessUnsubscriptions()
end

return Class.Create(SubscribableEvent)
